#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePtFromCrvStart command
//

class CCommandSamplePtFromCrvStart : public CRhinoCommand
{
public:
  CCommandSamplePtFromCrvStart() {}
  ~CCommandSamplePtFromCrvStart() {}
  UUID CommandUUID()
  {
    // {C76DCC59-85F2-4F96-8458-EFEEA06AF693}
    static const GUID SamplePtFromCrvStartCommand_UUID =
    { 0xC76DCC59, 0x85F2, 0x4F96, { 0x84, 0x58, 0xEF, 0xEE, 0xA0, 0x6A, 0xF6, 0x93 } };
    return SamplePtFromCrvStartCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePtFromCrvStart"; }
  const wchar_t* LocalCommandName() { return L"SamplePtFromCrvStart"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePtFromCrvStart object
static class CCommandSamplePtFromCrvStart theSamplePtFromCrvStartCommand;

CRhinoCommand::result CCommandSamplePtFromCrvStart::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const ON_Curve* crv = obj_ref.Curve();
  if (0 == crv)
    return CRhinoCommand::failure;

  double crv_length = 0.0;
  if (!crv->GetLength(&crv_length))
    return CRhinoCommand::failure;

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Length from start");
  gn.SetLowerLimit(0.0, TRUE);
  gn.SetUpperLimit(crv_length, TRUE);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  // Cook up a normalized arc length parameter, 
  // where 0.0 <= s <= 1.0.
  double length = fabs(gn.Number());
  double s = 0.0;
  if (length == 0.0)
    s = 0.0;
  else if (length == crv_length)
    s = 1.0;
  else
    s = length / crv_length;

  // Get the parameter of the point on the curve that is a 
  // prescribed arc length from the start of the curve.
  double t = 0.0;
  if (crv->GetNormalizedArcLengthPoint(s, &t))
  {
    ON_3dPoint pt = crv->PointAt(t);
    context.m_doc.AddPointObject(pt);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SamplePtFromCrvStart command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
