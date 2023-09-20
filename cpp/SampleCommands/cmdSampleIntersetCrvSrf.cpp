#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleIntersetCrvSrf command
//

#pragma region SampleIntersetCrvSrf command

class CCommandSampleIntersetCrvSrf : public CRhinoCommand
{
public:
  CCommandSampleIntersetCrvSrf() = default;
  ~CCommandSampleIntersetCrvSrf() = default;
  UUID CommandUUID() override
  {
    // {D76E082D-9105-4EBC-A7EB-24FAC63F5DE5}
    static const GUID SampleIntersetCrvSrfCommand_UUID =
    { 0xD76E082D, 0x9105, 0x4EBC, { 0xA7, 0xEB, 0x24, 0xFA, 0xC6, 0x3F, 0x5D, 0xE5 } };
    return SampleIntersetCrvSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleIntersetCrvSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleIntersetCrvSrf object
static class CCommandSampleIntersetCrvSrf theSampleIntersetCrvSrfCommand;

CRhinoCommand::result CCommandSampleIntersetCrvSrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Define plane
  ON_3dPoint plane_origin(-10, -10, 0.0);
  ON_3dPoint plane_x(10, -10, 0.0);
  ON_3dPoint plane_y(-10, 10, 0.0);

  ON_Plane plane(plane_origin, plane_x, plane_y);

  // Define plane surface
  ON_Interval domain0(0.0, plane_origin.DistanceTo(plane_x));
  ON_Interval domain1(0.0, plane_origin.DistanceTo(plane_y));

  ON_PlaneSurface plane_srf(plane);
  plane_srf.SetExtents(0, domain0, true);
  plane_srf.SetExtents(1, domain1, true);

  doc->AddSurfaceObject(plane_srf);

  // Define line
  ON_3dPoint line_start(-2.0, -6.0, -6.0);
  ON_3dPoint line_end(8.0, 4.0, 5.0);

  ON_Line line(line_start, line_end);

  // Define line curve
  ON_LineCurve line_crv(line);

  doc->AddCurveObject(line_crv);

  // Intersect curve and surface
  double tolerance = doc->AbsoluteTolerance();
  ON_SimpleArray<ON_X_EVENT> csx_events;
  int count = line_crv.IntersectSurface(&plane_srf, csx_events, tolerance, tolerance);
  for (int i = 0; i < count; i++)
  {
    ON_X_EVENT csx = csx_events[i];
    if (csx.IsPointEvent())
      doc->AddPointObject(csx.m_A[0]);
  }

  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleIntersetCrvSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
