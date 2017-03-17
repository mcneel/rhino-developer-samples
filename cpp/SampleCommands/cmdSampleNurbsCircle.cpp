#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleNurbsCircle command
//

#pragma region SampleNurbsCircle command

class CCommandSampleNurbsCircle : public CRhinoCommand
{
public:
  CCommandSampleNurbsCircle() {}
  ~CCommandSampleNurbsCircle() {}
  UUID CommandUUID()
  {
    // {BAC17D98-C5DE-4886-A060-7BC3B58F3965}
    static const GUID SampleNurbsCircleCommand_UUID =
    { 0xBAC17D98, 0xC5DE, 0x4886, { 0xA0, 0x60, 0x7B, 0xC3, 0xB5, 0x8F, 0x39, 0x65 } };
    return SampleNurbsCircleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleNurbsCircle"; }
  const wchar_t* LocalCommandName() const { return L"SampleNurbsCircle"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleNurbsCircle object
static class CCommandSampleNurbsCircle theSampleNurbsCircleCommand;

CRhinoCommand::result CCommandSampleNurbsCircle::RunCommand(const CRhinoCommandContext& context)
{
  // Adds a degree = 2 NURBS circle

  int dimension = 3;
  ON_BOOL32 bIsRational = TRUE;
  int degree = 2;
  int cv_count = 7;
  int knot_count = cv_count + degree - 1;

  ON_SimpleArray<ON_3dPoint> points(cv_count);
  points.SetCount(cv_count);
  points[0] = ON_3dPoint(2.500, 0.000, 0.000);
  points[1] = ON_3dPoint(5.000, 0.000, 0.000);
  points[2] = ON_3dPoint(3.750, 2.165, 0.000);
  points[3] = ON_3dPoint(2.500, 4.330, 0.000);
  points[4] = ON_3dPoint(1.250, 2.165, 0.000);
  points[5] = ON_3dPoint(0.000, 0.000, 0.000);
  points[6] = ON_3dPoint(2.500, 0.000, 0.000);

  ON_SimpleArray<double> weights(cv_count);
  weights.SetCount(cv_count);
  weights[0] = 1.0;
  weights[1] = 0.5;
  weights[2] = 1.0;
  weights[3] = 0.5;
  weights[4] = 1.0;
  weights[5] = 0.5;
  weights[6] = 1.0;

  ON_SimpleArray<double> knots(knot_count);
  weights.SetCount(knot_count);
  knots[0] = 0.000;
  knots[1] = 0.000;
  knots[2] = 0.333;
  knots[3] = 0.333;
  knots[4] = 0.667;
  knots[5] = 0.667;
  knots[6] = 1.000;
  knots[7] = 1.000;

  ON_NurbsCurve nc(dimension, bIsRational, degree + 1, cv_count);

  for (int ci = 0; ci < nc.CVCount(); ci++)
  {
    ON_4dPoint p4d(points[ci].x * weights[ci], points[ci].y * weights[ci], points[ci].z * weights[ci], weights[ci]);
    nc.SetCV(ci, p4d);
  }

  for (int ki = 0; ki < knot_count; ki++)
    nc.m_knot[ki] = knots[ki];

  if (nc.IsValid())
  {
    context.m_doc.AddCurveObject(nc);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleNurbsCircle command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
