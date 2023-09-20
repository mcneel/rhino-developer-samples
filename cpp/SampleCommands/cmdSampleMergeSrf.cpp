#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMergeSrf command
//

#pragma region SampleMergeSrf command

class CCommandSampleMergeSrf : public CRhinoCommand
{
public:
  CCommandSampleMergeSrf() = default;
  UUID CommandUUID() override
  {
    // {5B71802C-96CE-4ABD-BCA2-318BFCED7A22}
    static const GUID SampleMergeSrfCommand_UUID =
    { 0x5B71802C, 0x96CE, 0x4ABD, { 0xBC, 0xA2, 0x31, 0x8B, 0xFC, 0xED, 0x7A, 0x22 } };
    return SampleMergeSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMergeSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMergeSrf object
static class CCommandSampleMergeSrf theSampleMergeSrfCommand;

CRhinoCommand::result CCommandSampleMergeSrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select a pair of surfaces to merge");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(false);
  go.EnableReferenceObjectSelect(false);
  go.GetObjects(2, 2);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoObjRef objref[2];
  const ON_Brep* brep[2] = { nullptr, nullptr };
  ON_2dPoint picked_point[2] = { ON_2dPoint::UnsetPoint, ON_2dPoint::UnsetPoint };

  for (int i = 0; i < 2; i++)
  {
    objref[i] = go.Object(i);
    brep[i] = objref[i].Brep();
    if (nullptr == brep[i])
      return CRhinoCommand::failure;
    objref[i].SurfaceParameter(&picked_point[i][0], &picked_point[i][1]);
  }

  double tol = doc->AbsoluteTolerance();
  double angtol = doc->AngleToleranceRadians();

  CRhinoCommand::result rc = CRhinoCommand::failure;
  ON_Brep* out_brep = RhinoMergeSrf(brep[0], brep[1], tol, angtol, &picked_point[0], &picked_point[1]);
  if (nullptr != out_brep)
  {
    // Make sure to allow creased surfaces
    CRhinoKeepKinkySurfaces keep_kinky_surfaces;

    CRhinoBrepObject* brep_obj = new CRhinoBrepObject();
    brep_obj->SetBrep(out_brep);
    out_brep = nullptr;

    if (doc->ReplaceObject(objref[0], brep_obj))
    {
      doc->DeleteObject(objref[1]);
      rc = CRhinoCommand::success;
    }
    else
    {
      delete brep_obj;
      rc = CRhinoCommand::failure;
    }
  }

  doc->Redraw();

  return rc;
}

#pragma endregion

//
// END SampleMergeSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
