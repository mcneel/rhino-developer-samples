#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBend command
//

#pragma region SampleBend command

static CRhinoObject* RhinoBendObject(
  const CRhinoObject* obj,
  const ON_3dPoint& start_pt,
  const ON_3dPoint& end_pt,
  const ON_3dPoint& arc_pt,
  double angle = ON_UNSET_VALUE,
  bool bStraight = false,
  bool bSymmetric = false,
  bool bCopy = false
)
{
  if (0 == obj)
    return 0;

  CRhinoDoc* doc = obj->Document();
  if (0 == doc)
    return 0;

  ON_Line spine(start_pt, end_pt);
  if (spine.Length() < ON_ZERO_TOLERANCE)
    return 0;

  if (ON_IsValid(angle) && fabs(angle) < ON_ZERO_TOLERANCE)
    angle = ON_UNSET_VALUE;

  CRhinoBendSpaceMorph morph;
  morph.SetSpine(spine);
  bool bUseAngle = ON_IsValid(angle);
  morph.m_bStraightEnd = bStraight || bUseAngle;
  morph.m_bSymmetric = bSymmetric;
  morph.SetArc(arc_pt, bUseAngle ? angle : ON_UNSET_VALUE);

  CRhinoKeepKinkySurfaces keep_creases;

  return doc->MorphObject(obj, morph, true, !bCopy);
}

class CCommandSampleBend : public CRhinoCommand
{
public:
  CCommandSampleBend() {}
  ~CCommandSampleBend() {}
  UUID CommandUUID()
  {
    // {F02D2876-34A4-4522-A34A-E96A6FDE412E}
    static const GUID SampleBendCommand_UUID =
    { 0xF02D2876, 0x34A4, 0x4522, { 0xA3, 0x4A, 0xE9, 0x6A, 0x6F, 0xDE, 0x41, 0x2E } };
    return SampleBendCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleBend"; }
  const wchar_t* LocalCommandName() const { return L"SampleBend"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleBend object
static class CCommandSampleBend theSampleBendCommand;

CRhinoCommand::result CCommandSampleBend::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface to bend");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 1);
  CRhinoCommand::result rc = go.CommandResult();
  if (rc != CRhinoCommand::success)
    return go.CommandResult();

  CArgsRhinoGetLine args;
  args.SetFirstPointPrompt(L"Start of spine");
  args.SetSecondPointPrompt(L"End of spine");
  ON_Line line;
  rc = RhinoGetLine(args, line);
  if (rc != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point to bend through");
  gp.GetPoint();
  rc = gp.CommandResult();
  if (rc != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoObject* obj = RhinoBendObject(go.Object(0).Object(), line.from, line.to, gp.Point());
  if (obj)
    context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleBend command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
