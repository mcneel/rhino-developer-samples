#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleIntersectCrv command
//

class CCommandSampleIntersectCrv : public CRhinoCommand
{
public:
  CCommandSampleIntersectCrv() = default;
  ~CCommandSampleIntersectCrv() = default;
  UUID CommandUUID() override
  {
    // {AAD79EBF-40AE-4170-9D90-A363B2A3B62A}
    static const GUID SampleIntersectCrvCommand_UUID =
    { 0xAAD79EBF, 0x40AE, 0x4170, { 0x9D, 0x90, 0xA3, 0x63, 0xB2, 0xA3, 0xB6, 0x2A } };
    return SampleIntersectCrvCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleIntersectCrv"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleIntersectCrv object
static class CCommandSampleIntersectCrv theSampleIntersectCrvCommand;

CRhinoCommand::result CCommandSampleIntersectCrv::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select two curves to intersect");
  go.SetGeometryFilter(ON::curve_object);
  go.GetObjects(2, 2);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curveA = go.Object(0).Curve();
  const ON_Curve* curveB = go.Object(1).Curve();
  if (nullptr == curveA || nullptr == curveB)
    return CRhinoCommand::failure;

  const double tolerance = doc->AbsoluteTolerance();

  ON_SimpleArray<ON_X_EVENT> ccx_events;
  int ccx_count = curveA->IntersectCurve(curveB, ccx_events, tolerance, tolerance);
  if (0 == ccx_count)
  {
    RhinoApp().Print(L"No intersections found.\n");
    return CRhinoCommand::success;
  }

  if (1 == ccx_count)
    RhinoApp().Print(L"Found 1 intersections.\n");
  else
    RhinoApp().Print(L"Found %d intersections.\n", ccx_count);

  for (int i = 0; i < ccx_events.Count(); i++)
  {
    const ON_X_EVENT& ccx = ccx_events[i];
    if (ccx.IsPointEvent())
    {
      CRhinoPointObject* point_obj = doc->AddPointObject(ccx.m_A[0]);
      if (point_obj)
        point_obj->Select(true);
    }
    else
    {
      ON_Interval interval(ccx.m_a[0], ccx.m_a[1]);
      ON_Curve* trim = ON_TrimCurve(*curveA, interval);
      if (trim)
      {
        CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
        curve_obj->SetCurve(trim);
        if (doc->AddObject(curve_obj))
          curve_obj->Select(true);
        else
          delete curve_obj;
      }
    }
  }

  doc->Redraw();

  return CRhinoCommand::success;
}

//
// END SampleIntersectCrv command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
