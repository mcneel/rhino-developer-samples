#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleEvaluateCurve command
//

class CCommandSampleEvaluateCurve : public CRhinoCommand
{
public:
  CCommandSampleEvaluateCurve() {}
  ~CCommandSampleEvaluateCurve() {}
  UUID CommandUUID()
  {
    // {8D70F1E8-BEED-48E9-AF0D-5E63B9855EE4}
    static const GUID SampleEvaluateCurveCommand_UUID =
    { 0x8D70F1E8, 0xBEED, 0x48E9, { 0xAF, 0x0D, 0x5E, 0x63, 0xB9, 0x85, 0x5E, 0xE4 } };
    return SampleEvaluateCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleEvaluateCurve"; }
  const wchar_t* LocalCommandName() { return L"SampleEvaluateCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleEvaluateCurve object
static class CCommandSampleEvaluateCurve theSampleEvaluateCurveCommand;

CRhinoCommand::result CCommandSampleEvaluateCurve::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve for evaluation");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* crv = go.Object(0).Curve();
  if (0 == crv)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Pick point on curve for evaluation");
  gp.Constrain(*crv);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  double t = 0.0;
  if (0 == gp.PointOnCurve(&t))
    return CRhinoCommand::nothing;

  CRhinoGetInteger gi;
  gi.SetCommandPrompt(L"Number of derivatives to calculate");
  gi.SetDefaultInteger(1);
  gi.SetLowerLimit(1);
  gi.SetUpperLimit(4);
  gi.GetNumber();
  if (gi.CommandResult() != CRhinoCommand::success)
    return gi.CommandResult();

  int der_count = gi.Number();

  int v_count = 3 * (der_count + 1);
  ON_SimpleArray<double> v_array(v_count);
  v_array.SetCount(v_count);
  int v_stride = 3;

  if (crv->Evaluate(t, der_count, v_stride, v_array.Array()))
  {
    RhinoApp().Print(L"Failed to evaluate curve.\n");
  }
  else
  {
    const double* v = v_array.Array();
    int i;
    for (i = 0; i < v_count; i++)
    {
      RhinoApp().Print(L"Derivative i = %f,%f,%f", v[0], v[1], v[2]);
      v += v_stride;
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleEvaluateCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
