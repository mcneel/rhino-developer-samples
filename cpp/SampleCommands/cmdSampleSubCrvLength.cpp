#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSubCrvLength command
//

class CCommandSampleSubCrvLength : public CRhinoCommand
{
public:
  CCommandSampleSubCrvLength() {}
  ~CCommandSampleSubCrvLength() {}
  UUID CommandUUID()
  {
    // {CE2E3AA0-7CA9-43EA-BB51-647A9DA9018A}
    static const GUID SampleSubCrvLengthCommand_UUID =
    { 0xCE2E3AA0, 0x7CA9, 0x43EA, { 0xBB, 0x51, 0x64, 0x7A, 0x9D, 0xA9, 0x01, 0x8A } };
    return SampleSubCrvLengthCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSubCrvLength"; }
  const wchar_t* LocalCommandName() { return L"SampleSubCrvLength"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSubCrvLength object
static class CCommandSampleSubCrvLength theSampleSubCrvLengthCommand;

CRhinoCommand::result CCommandSampleSubCrvLength::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to measure");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& ref = go.Object(0);
  const ON_Curve* crv = ref.Curve();
  if (!crv)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"First point on curve");
  gp.Constrain(*crv);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  double t0;
  if (!crv->GetClosestPoint(gp.Point(), &t0))
    return CRhinoCommand::nothing;

  gp.SetCommandPrompt(L"Second point on curve");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  double t1;
  if (!crv->GetClosestPoint(gp.Point(), &t1))
    return CRhinoCommand::nothing;

  ON_Interval dom;
  if (t0 < t1)
    dom.Set(t0, t1);
  else
    dom.Set(t1, t0);

  double len;
  if (crv->GetLength(&len, 0.0, &dom))
    RhinoApp().Print(L"Subcurve length = %f.\n", len);
  else
    RhinoApp().Print(L"Unable to calculate length of subcurve.\n");

  return CRhinoCommand::success;
}

//
// END SampleSubCrvLength command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
