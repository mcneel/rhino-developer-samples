#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePlanarFaceLoops command
//

#pragma region SamplePlanarFaceLoops command

class CCommandSamplePlanarFaceLoops : public CRhinoCommand
{
public:
  CCommandSamplePlanarFaceLoops() {}
  ~CCommandSamplePlanarFaceLoops() {}
  UUID CommandUUID()
  {
    // {107498CF-861C-44B6-BE51-1AA2ED622A35}
    static const GUID SamplePlanarFaceLoopsCommand_UUID =
    { 0x107498CF, 0x861C, 0x44B6, { 0xBE, 0x51, 0x1A, 0xA2, 0xED, 0x62, 0x2A, 0x35 } };
    return SamplePlanarFaceLoopsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePlanarFaceLoops"; }
  const wchar_t* LocalCommandName() const { return L"SamplePlanarFaceLoops"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePlanarFaceLoops object
static class CCommandSamplePlanarFaceLoops theSamplePlanarFaceLoopsCommand;

CRhinoCommand::result CCommandSamplePlanarFaceLoops::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject gs;
  gs.SetCommandPrompt(L"Select surface");
  gs.SetGeometryFilter(CRhinoGetObject::surface_object);
  gs.EnableSubObjectSelect(FALSE);
  gs.GetObjects(1, 1);
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  const ON_Brep* brep = gs.Object(0).Brep();
  if (0 == brep)
    return CRhinoCommand::failure;

  CRhinoGetObject gc;
  gc.SetCommandPrompt(L"Select trimming curves on surface");
  gc.SetGeometryFilter(CRhinoGetObject::curve_object);
  gc.EnableGroupSelect(TRUE);
  gc.EnableSubObjectSelect(FALSE);
  gc.EnablePreSelect(FALSE);
  gc.EnableDeselectAllBeforePostSelect(false);
  gc.GetObjects(1, 0);
  if (gc.CommandResult() != CRhinoCommand::success)
    return gc.CommandResult();

  const int curve_count = gc.ObjectCount();
  ON_SimpleArray<ON_Curve*> boundary;

  ON_Brep* new_brep = brep->Duplicate();
  new_brep->DestroyMesh(ON::any_mesh, true);

  for (int i = 0; i < curve_count; i++)
  {
    const ON_Curve* curve = gc.Object(i).Curve();
    if (0 == curve)
    {
      delete new_brep;
      return CRhinoCommand::failure;
    }

    boundary.Append(const_cast<ON_Curve*>(curve));
    new_brep->NewPlanarFaceLoop(0, ON_BrepLoop::inner, boundary, true);
    boundary.Empty();
  }

  if (new_brep->IsValid())
  {
    CRhinoBrepObject* new_brep_obj = new CRhinoBrepObject();
    new_brep_obj->SetBrep(new_brep);
    context.m_doc.ReplaceObject(gs.Object(0), new_brep_obj);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePlanarFaceLoops command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
