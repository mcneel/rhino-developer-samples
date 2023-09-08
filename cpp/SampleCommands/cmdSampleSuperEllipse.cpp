#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSuperEllipse command
//

#pragma region SampleSuperEllipse command

class CCommandSampleSuperEllipse : public CRhinoCommand
{
public:
  CCommandSampleSuperEllipse() = default;
  UUID CommandUUID() override
  {
    // {4B580B15-17C2-4FFE-9E52-EC24CB720916}
    static const GUID SampleSuperEllipseCommand_UUID =
    { 0x4B580B15, 0x17C2, 0x4FFE,{ 0x9E, 0x52, 0xEC, 0x24, 0xCB, 0x72, 0x09, 0x16 } };
    return SampleSuperEllipseCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSuperEllipse"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSuperEllipse object
static class CCommandSampleSuperEllipse theSampleSuperEllipseCommand;

CRhinoCommand::result CCommandSampleSuperEllipse::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Create the NURBS "Super Ellipse", 
  // where: (x / a) ^ n + (y / b) ^ n = 1
  CArgsRhinoGetEllipse args;
  args.SetMarkFoci(FALSE);
  args.SetDeformable(FALSE);
  args.SetDeformablePointCount(12);
  args.SetShowDeformableOption(FALSE);

  ON_Ellipse ellipse;
  CRhinoCommand::result rc = RhinoGetEllipse(args, ellipse);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_NurbsCurve nurb;
  if (!ellipse.GetNurbForm(nurb) || nurb.CVCount() != 9)
    return CRhinoCommand::failure;

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Exponent");
  gn.SetDefaultNumber(2.0);
  gn.SetLowerLimit(0.0);
  gn.GetNumber();
  if (rc != CRhinoCommand::success)
    return rc;

  double n = gn.Number();

  // Adjust weights.
  double q = pow(0.5, 1.0 / n);
  double w = (q - 0.5) / (1.0 - q);

  for (int i = 1; i < 9; i++)
  {
    if (i % 2 == 0) continue;
    ON_3dPoint P;
    nurb.GetCV(i, P);
    P *= w;
    nurb.SetCV(i, P);
    nurb.SetWeight(i, w);
  }

  // If ellipse's plane is the world xy, compare it to 
  // a sample of points from the algebraic definition.
  if (ellipse.Center().IsZero() && ellipse.Plane() == ON_Plane::World_xy) 
  {
    double max_dev = 0.0;
    double max_var = 0.0;
    ON_Interval quad(0.0, ON_PI / 2.0);
    double a = ellipse.Radius(0);
    double b = ellipse.Radius(1);
    for (int i = 1; i < 1000; i++) 
    {
      ON_3dPoint P = nurb.PointAt(quad.ParameterAt(0.001*(double)i));
      double c = 1.0 - pow(P[0] / a, n);
      double y = ellipse.Radius(1)*pow(c, 1.0 / n);
      double t;
      ON_3dPoint E(P[0], y, 0);
      nurb.GetClosestPoint(E, &t);
      double d = E.DistanceTo(nurb.PointAt(t));
      if (d > max_dev)
        max_dev = d;
      double ax = P[0] / a;
      double bx = P[1] / b;
      d = fabs(pow(ax, n) + pow(bx, n) - 1.0);
      if (d > max_var)
        max_var = d;
      if (i % 20 == 0) 
        doc->AddPointObject(E);
    }
    RhinoApp().Print(L"Max deviation = %g\n", max_dev);
    RhinoApp().Print(L"Max deviation from 1 = %g\n", max_var);
  }

  doc->AddCurveObject(nurb);
  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSuperEllipse command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
