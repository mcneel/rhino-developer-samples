#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleInterpCurve command
//

class CCommandSampleInterpCurve : public CRhinoCommand
{
public:
  CCommandSampleInterpCurve() {}
  ~CCommandSampleInterpCurve() {}
  UUID CommandUUID()
  {
    // {C3805C7F-E30F-4B00-A8EA-94BAC2B4F1B8}
    static const GUID SampleInterpCurveCommand_UUID =
    { 0xC3805C7F, 0xE30F, 0x4B00, { 0xA8, 0xEA, 0x94, 0xBA, 0xC2, 0xB4, 0xF1, 0xB8 } };
    return SampleInterpCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleInterpCurve"; }
  const wchar_t* LocalCommandName() { return L"SampleInterpCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleInterpCurve object
static class CCommandSampleInterpCurve theSampleInterpCurveCommand;

CRhinoCommand::result CCommandSampleInterpCurve::RunCommand(const CRhinoCommandContext& context)
{
  ON_3dPointArray points(16);
  points.Append(ON_3dPoint(0.0, 3.12494, 0.0));
  points.Append(ON_3dPoint(7.01306, 3.31419, 0.0));
  points.Append(ON_3dPoint(8.01888, 3.34416, 0.0));
  points.Append(ON_3dPoint(9.02578, 3.37375, 0.0));
  points.Append(ON_3dPoint(10.0338, 3.40260, 0.0));
  points.Append(ON_3dPoint(11.0430, 3.43034, 0.0));
  points.Append(ON_3dPoint(12.0533, 3.45659, 0.0));
  points.Append(ON_3dPoint(13.0648, 3.48098, 0.0));
  points.Append(ON_3dPoint(14.0776, 3.50313, 0.0));
  points.Append(ON_3dPoint(15.0916, 3.52267, 0.0));
  points.Append(ON_3dPoint(16.1068, 3.53923, 0.0));
  points.Append(ON_3dPoint(17.1233, 3.55249, 0.0));
  points.Append(ON_3dPoint(18.1410, 3.56222, 0.0));
  points.Append(ON_3dPoint(19.1587, 3.56829, 0.0));
  points.Append(ON_3dPoint(20.1758, 3.57091, 0.0));
  points.Append(ON_3dPoint(30.3156, 3.45748, 0.0));

  ON_NurbsCurve* curve = RhinoInterpCurve(3, points, 0, 0, 0, 0);
  if (curve)
  {
    CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
    curve_obj->SetCurve(curve);
    context.m_doc.AddObject(curve_obj);
    context.m_doc.AddPointCloudObject(points.Count(), points.Array());
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleInterpCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
