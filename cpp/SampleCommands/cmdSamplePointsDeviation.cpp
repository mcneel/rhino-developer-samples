#include "StdAfx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePointsDeviation command
//
class CCommandSamplePointsDeviation : public CRhinoCommand
{
public:
  CCommandSamplePointsDeviation() {}
  ~CCommandSamplePointsDeviation() {}
  UUID CommandUUID()
  {
    // {416B1B0A-5413-4704-BBF2-397859B6B0C2}
    static const GUID SamplePointsDeviationCommand_UUID =
    { 0x416B1B0A, 0x5413, 0x4704, { 0xBB, 0xF2, 0x39, 0x78, 0x59, 0xB6, 0xB0, 0xC2 } };
    return SamplePointsDeviationCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"foo_SamplePointsDeviation"; }
  const wchar_t* LocalCommandName() { return L"foo_SamplePointsDeviation"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

static class CCommandSamplePointsDeviation theSamplePointsDeviationCommand;

CRhinoCommand::result CCommandSamplePointsDeviation::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject	gp;
  gp.SetCommandPrompt(L"Select point cloud");
  gp.SetGeometryFilter(CRhinoGetObject::pointset_object);
  gp.GetObjects(1, 1);
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  const ON_PointCloud* point_cloud = gp.Object(0).PointCloud();
  if (!point_cloud)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableDeselectAllBeforePostSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Brep* brep = go.Object(0).Brep();
  if (!brep)
    return CRhinoCommand::failure;

  const ON_BrepFace* face = go.Object(0).Face();
  if (!face)
    return CRhinoCommand::failure;

  bool bTrimmed = (brep->FaceIsSurface(face->m_face_index) ? false : true);

  const int point_count = point_cloud->PointCount();
  int num_GetClosestPoint = 0;
  int num_EvPoint = 0;
  int num_OnSrf = 0;
  int num_OnFace = 0;
  int num_OnTrim = 0;
  int i;

  // Start timer
  DWORD start_time = ::GetTickCount();

  for (i = 0; i < point_count; i++)
  {
    ON_3dPoint pt = point_cloud->m_P[i];

    // Calculate closest point
    double u = 0.0, v = 0.0;
    if (!face->GetClosestPoint(pt, &u, &v))
      continue;

    num_GetClosestPoint++;

    // Evaluate surface at closest point
    ON_3dPoint srf_pt;
    if (!face->EvPoint(u, v, srf_pt))
      continue;

    num_EvPoint++;

    // Determine if test point was on surface
    if (srf_pt.DistanceTo(pt) <= ON_SQRT_EPSILON)
      num_OnSrf++;

    if (bTrimmed)
    {
      // Is surface point on face (untrimmed portion)
      if (RhinoIsPointOnFace(*face, ON_SQRT_EPSILON, false, u, v))
        num_OnFace++;
      else
        num_OnTrim++;
      continue;
    }

    num_OnFace++;
  }

  // End timer
  DWORD elapsed_time = ::GetTickCount() - start_time;

  RhinoApp().Print(L"Total calulations = %d\n", point_count);
  RhinoApp().Print(L"Successful closest points = %d\n", num_GetClosestPoint);
  RhinoApp().Print(L"Successful point evaluations = %d\n", num_EvPoint);
  RhinoApp().Print(L"Test points on surface = %d\n", num_OnSrf);
  RhinoApp().Print(L"Surface points on face = %d\n", num_OnFace);
  RhinoApp().Print(L"Surface points on trim = %d\n", num_OnTrim);
  RhinoApp().Print(L"Elapsed time = %.3f seconds\n", (double)elapsed_time / 1000.0);

  return CRhinoCommand::success;
}

//
// END SamplePointsDeviation command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
