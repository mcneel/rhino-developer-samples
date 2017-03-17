#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleNurbsCurveLength command
//

#pragma region SampleNurbsCurveLength command

class CCommandSampleNurbsCurveLength : public CRhinoCommand
{
public:
  CCommandSampleNurbsCurveLength() {}
  ~CCommandSampleNurbsCurveLength() {}
  UUID CommandUUID()
  {
    // {758DBFB-FE78-43CF-B20A-EB6E9118D74A}
    static const GUID SampleNurbsCurveLengthCommand_UUID =
    { 0x758DBFB, 0xFE78, 0x43CF,{ 0xB2, 0x0A, 0xEB, 0x6E, 0x91, 0x18, 0xD7, 0x4A } };
    return SampleNurbsCurveLengthCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleNurbsCurveLength"; }
  const wchar_t* LocalCommandName() const { return L"SampleNurbsCurveLength"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleNurbsCurveLength object
static class CCommandSampleNurbsCurveLength theSampleNurbsCurveLengthCommand;

CRhinoCommand::result CCommandSampleNurbsCurveLength::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  ON_3dPointArray points;
  points.Append(ON_3dPoint(0.0, 0.0, 0.0));
  points.Append(ON_3dPoint(5.0, 0.0, 0.0));
  points.Append(ON_3dPoint(5.0, 5.0, 0.0));
  points.Append(ON_3dPoint(0.0, 5.0, 0.0));
  points.Append(ON_3dPoint(0.0, 10.0, 0.0));
  points.Append(ON_3dPoint(5.0, 10.0, 0.0));
  points.Append(ON_3dPoint(5.0, 15.0, 0.0));
  points.Append(ON_3dPoint(0.0, 15.0, 0.0));
  ON_BezierCurve bezier(points);

  double length = ON_UNSET_VALUE;

  // Test 0
  ON_NurbsCurve* nc0 = ON_NurbsCurve::New(bezier);
  if (nc0->GetLength(&length))
    RhinoApp().Print(L"nc0->GetLength = %g\n", length);
  else
  {
    ON_MassProperties mp;
    if (nc0->LengthMassProperties(mp, true, false, false, false))
      RhinoApp().Print(L"nc0->LengthMassProperties = %g\n", fabs(mp.m_mass));
  }
  delete nc0; // Don't leak...

  // Test 1
  ON_NurbsCurve nc1;
  if (bezier.GetNurbForm(nc1))
  {
    const ON_Curve* nc1_ptr = &nc1;
    if (nc1_ptr->GetLength(&length))
      RhinoApp().Print(L"nc1.GetLength = %g\n", length);
    else
    {
      ON_MassProperties mp;
      if (nc1.LengthMassProperties(mp, true, false, false, false))
        RhinoApp().Print(L"nc1.LengthMassProperties = %g\n", fabs(mp.m_mass));
    }
  }

  // Test 2
  ON_NurbsCurve nc2(bezier);
  const ON_Curve* nc2_ptr = &nc2;
  if (nc2_ptr->GetLength(&length))
    RhinoApp().Print(L"nc2.GetLength = %g\n", length);
  else
  {
    ON_MassProperties mp;
    if (nc2.LengthMassProperties(mp, true, false, false, false))
      RhinoApp().Print(L"nc2.LengthMassProperties = %g\n", fabs(mp.m_mass));
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleNurbsCurveLength command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
