#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHelixAroundCurve command
//

#pragma region SampleHelixAroundCurve command

class CCommandSampleHelixAroundCurve : public CRhinoCommand
{
public:
  CCommandSampleHelixAroundCurve() = default;
  UUID CommandUUID() override
  {
    // {460D9F8C-33C0-43EC-9B54-9AE8128F7D9E}
    static const GUID SampleHelixAroundCurveCommand_UUID =
    { 0x460D9F8C, 0x33C0, 0x43EC, { 0x9B, 0x54, 0x9A, 0xE8, 0x12, 0x8F, 0x7D, 0x9E } };
    return SampleHelixAroundCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleHelixAroundCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleHelixAroundCurve object
static class CCommandSampleHelixAroundCurve theSampleHelixAroundCurveCommand;

CRhinoCommand::result CCommandSampleHelixAroundCurve::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  {
    CRhinoGetObject go;
    go.SetCommandPrompt(L"Select curve");
    go.SetGeometryFilter(CRhinoGetObject::curve_object);
    go.EnableSubObjectSelect(FALSE);
    go.GetObjects(1, 1);
    if (go.CommandResult() != CRhinoCommand::success)
      return CRhinoCommand::cancel;

    const CRhinoObjRef& objref = go.Object(0);
    const ON_Curve* curve = objref.Curve();
    if (nullptr == curve)
      return CRhinoCommand::failure;

    CArgsRhinoGetSpiral args;
    args.SetAroundCurve(TRUE);

    double t, t0, t1, tmid;
    objref.CurveParameter(&t);
    curve->GetDomain(&t0, &t1);
    curve->GetNormalizedArcLengthPoint(0.5, &tmid);

    args.SetRail(curve);

    // flip rail and t's if necessary
    if (t > tmid)
    {
      args.Rail()->Reverse();
      double d = t1; t1 = t0; t0 = d;
    }

    args.SetStartDir(curve->TangentAt(t0));
    args.SetStartPoint(curve->PointAt(t0));
    args.SetEndDir(curve->TangentAt(t1));
    args.SetEndPoint(curve->PointAt(t1));

    double d = ON_UNSET_VALUE;
    curve->GetLength(&d);
    args.SetLength(d);

    args.SetPitchMode(FALSE);

    if (args.PitchMode())
      args.SetTurns(args.Length() / 2.4);
    else
      args.SetPitch(args.Length() / 12);

    args.SetFirstRadius(0.5);
    args.SetSecondRadius(0.5);

    ON_NurbsCurve helix;

    bool rc = RhinoCreateSpiral(args, helix);
    if (rc)
    {
      doc->AddCurveObject(helix);
      doc->Redraw();
    }

    return rc 
      ? CRhinoCommand::success 
      : CRhinoCommand::failure;
  }
}

#pragma endregion

//
// END SampleHelixAroundCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
