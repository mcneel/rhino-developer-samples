#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshOutline command
//

#pragma region SampleMeshOutline command

class CCommandSampleMeshOutline : public CRhinoCommand
{
public:
  CCommandSampleMeshOutline() {}
  ~CCommandSampleMeshOutline() {}
  UUID CommandUUID()
  {
    // {18B2C13C-D652-4263-9F75-74BE31F35EAC}
    static const GUID SampleMeshOutlineCommand_UUID =
    { 0x18B2C13C, 0xD652, 0x4263, { 0x9F, 0x75, 0x74, 0xBE, 0x31, 0xF3, 0x5E, 0xAC } };
    return SampleMeshOutlineCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshOutline"; }
  const wchar_t* LocalCommandName() const { return L"SampleMeshOutline"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshOutline object
static class CCommandSampleMeshOutline theSampleMeshOutlineCommand;

CRhinoCommand::result CCommandSampleMeshOutline::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh objects to outline");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.EnableSubObjectSelect(FALSE);
  go.EnableGroupSelect();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoView* view = go.View();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_SimpleArray<const ON_Mesh*> InMeshes;
  int i, j;

  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Mesh* mesh = ON_Mesh::Cast(go.Object(i).Geometry());
    if (mesh)
      InMeshes.Append(mesh);
  }

  if (0 == InMeshes.Count())
    return CRhinoCommand::failure;

  ON_ClassArray<ON_SimpleArray<ON_PolylineCurve*>> OutRegions;
  bool rc = RhinoGetMeshOutline(InMeshes, view, 0.001 * context.m_doc.AbsoluteTolerance(), OutRegions);
  if (rc)
  {
    for (i = 0; i < OutRegions.Count(); i++)
    {
      for (j = 0; j < OutRegions[i].Count(); j++)
      {
        ON_PolylineCurve* crv = OutRegions[i][j];
        if (crv)
        {
          crv->RemoveShortSegments(ON_SQRT_EPSILON);
          CRhinoCurveObject* crv_obj = new CRhinoCurveObject();
          crv_obj->SetCurve(crv);
          if (context.m_doc.AddObject(crv_obj))
            crv_obj->Select();
          else
            delete crv_obj;
        }
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMeshOutline command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
