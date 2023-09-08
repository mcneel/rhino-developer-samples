#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleClosedCurveContainment command
//

class CRhGetPlanarClosedCurve : public CRhinoGetObject
{
public:
  CRhGetPlanarClosedCurve(double tolerance);
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;
private:
  double m_tolerance;
};

CRhGetPlanarClosedCurve::CRhGetPlanarClosedCurve(double tolerance)
  : m_tolerance(tolerance)
{
}

bool CRhGetPlanarClosedCurve::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
{
  UNREFERENCED_PARAMETER(object);
  UNREFERENCED_PARAMETER(component_index);
  bool rc = false;
  if (geometry)
  {
    const ON_Curve* curve = ON_Curve::Cast(geometry);
    if (curve && curve->IsClosed())
    {
      if (curve->IsPlanar(nullptr, m_tolerance))
        rc = true;
    }
  }
  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleClosedCurveContainment : public CRhinoCommand
{
public:
  CCommandSampleClosedCurveContainment() = default;
  UUID CommandUUID() override
  {
    // {FEB60412-2EB0-45BB-A360-4D2C5248E702}
    static const GUID SampleClosedCurveContainmentCommand_UUID =
    { 0xFEB60412, 0x2EB0, 0x45BB, { 0xA3, 0x60, 0x4D, 0x2C, 0x52, 0x48, 0xE7, 0x02 } };
    return SampleClosedCurveContainmentCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleClosedCurveContainment"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleClosedCurveContainment object
static class CCommandSampleClosedCurveContainment theSampleClosedCurveContainmentCommand;

CRhinoCommand::result CCommandSampleClosedCurveContainment::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhGetPlanarClosedCurve go(doc->AbsoluteTolerance());
  go.SetCommandPrompt(L"Select two closed planar curves for containment test");
  go.GetObjects(2, 2);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curve0 = go.Object(0).Curve();
  const ON_Curve* curve1 = go.Object(1).Curve();
  if (nullptr == curve0 || nullptr == curve1)
    return CRhinoCommand::failure;

  const double tolerance = doc->AbsoluteTolerance();

  ON_Plane plane0, plane1;
  if (!curve0->IsPlanar(&plane0, tolerance) || !curve1->IsPlanar(&plane1, tolerance))
    return CRhinoCommand::failure;

  if (!curve0->IsInPlane(plane1, tolerance) || !curve1->IsInPlane(plane0, tolerance))
  {
    RhinoApp().Print(L"Curves are not co-planar.\n");
    return CRhinoCommand::success;
  }

  const int rc = RhinoPlanarClosedCurveContainmentTest(*curve0, *curve1, plane0, tolerance);
  if (rc == 0)
    RhinoApp().Print(L"The regions bounded by the curves are disjoint.\n");
  else if (rc == 1)
    RhinoApp().Print(L"The two curves intersect.\n");
  else if (rc == 2)
    RhinoApp().Print(L"The region bounded by the first curve is inside of the second curve.\n");
  else
    RhinoApp().Print(L"The region bounded by the second curve is inside of the first curve.\n");

  return CRhinoCommand::success;
}


//
// END SampleClosedCurveContainment command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
