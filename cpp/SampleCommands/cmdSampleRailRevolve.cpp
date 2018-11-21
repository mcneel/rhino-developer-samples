#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRailRevolve command
//

#pragma region SampleRailRevolve command

class CCommandSampleRailRevolve : public CRhinoCommand
{
public:
  CCommandSampleRailRevolve();
  UUID CommandUUID() override
  {
    // {A751D54C-26FB-4410-9798-410C7721C750}
    static const GUID SampleRailRevolveCommand_UUID =
    { 0xA751D54C, 0x26FB, 0x4410, { 0x97, 0x98, 0x41, 0x0C, 0x77, 0x21, 0xC7, 0x50 } };
    return SampleRailRevolveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleRailRevolve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  bool m_bSplitAtTangents;
};

// The one and only CCommandSampleRailRevolve object
static class CCommandSampleRailRevolve theSampleRailRevolveCommand;

CCommandSampleRailRevolve::CCommandSampleRailRevolve()
  : m_bSplitAtTangents(false)
{
}

CRhinoCommand::result CCommandSampleRailRevolve::RunCommand(const CRhinoCommandContext& context)
{
  const ON_Curve* profile_curve = nullptr;
  const ON_Curve* rail_curve = nullptr;
  ON_Line axis = ON_Line::UnsetLine;
  bool bSplitAtTangents = m_bSplitAtTangents;
  bool bScaleHeight = false;

  CRhinoGetObject get_profile;
  get_profile.SetCommandPrompt(L"Select profile curve");
  get_profile.SetGeometryFilter(ON::curve_object);
  get_profile.AddCommandOptionToggle(RHCMDOPTNAME(L"ScaleHeight"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bScaleHeight, &bScaleHeight);
  get_profile.AddCommandOptionToggle(RHCMDOPTNAME(L"SplitAtTangents"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bSplitAtTangents, &bSplitAtTangents);
  for (;;)
  {
    CRhinoGet::result res = get_profile.GetObjects(1, 1);
    if (res == CRhinoGet::option)
      continue;
    if (res != CRhinoGet::object)
      return CRhinoCommand::cancel;
    break;
  }

  profile_curve = get_profile.Object(0).Curve();
  if (nullptr == profile_curve)
    return CRhinoCommand::failure;

  // select rail curve
  CRhinoGetObject get_rail;
  get_rail.SetCommandPrompt(L"Select rail curve");
  get_rail.SetGeometryFilter(ON::curve_object);
  get_rail.AddCommandOptionToggle(RHCMDOPTNAME(L"ScaleHeight"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bScaleHeight, &bScaleHeight);
  get_rail.AddCommandOptionToggle(RHCMDOPTNAME(L"SplitAtTangents"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bSplitAtTangents, &bSplitAtTangents);
  get_rail.EnablePreSelect(false);
  get_rail.EnableDeselectAllBeforePostSelect(false);
  for (;;)
  {
    CRhinoGet::result res = get_rail.GetObjects(1, 1);
    if (res == CRhinoGet::option)
      continue;
    if (res != CRhinoGet::object)
      return CRhinoCommand::cancel;
    break;
  }

  rail_curve = get_rail.Object(0).Curve();
  if (nullptr == rail_curve)
    return CRhinoCommand::failure;

  // Get the revolve axis
  CArgsRhinoGetLine line_args;
  line_args.SetFirstPointPrompt(L"Start of rail revolve axis");
  line_args.SetSecondPointPrompt(L"End of rail revolve axis");
  CRhinoCommand::result rc = RhinoGetLine(line_args, axis);
  if (CRhinoCommand::success != rc)
    return rc;

  // Validate control points
  ON_NurbsCurve nurbs_curve;
  if (rail_curve->GetNurbForm(nurbs_curve))
  {
    for (int i = 0; i < nurbs_curve.CVCount(); i++)
    {
      ON_3dPoint p;
      nurbs_curve.GetCV(i, p);
      if (axis.DistanceTo(p) <= ON_SQRT_EPSILON)
      {
        RhinoApp().Print(L"Unable to revolve - control points for path curve cannot be on revolve axis.\n");
        return CRhinoCommand::failure;
      }
    }
  }

  // Create surface
  ON_NurbsSurface* nurbs_surface = ON_NurbsSurface::New();
  if (nurbs_surface != RhinoRailRevolve(profile_curve, rail_curve, axis, bScaleHeight, nurbs_surface))
  {
    delete nurbs_surface; // don't leak...
    return CRhinoCommand::failure;
  }

  // Create brep
  ON_Brep* brep = ON_Brep::New();
  brep->Create(nurbs_surface); // brep now owns nurbs_surface
  if (bSplitAtTangents)
    RhinoSplitFacesAtTangents(*brep);

  // Create runtime brep object
  CRhinoBrepObject* brep_obj = new CRhinoBrepObject();
  brep_obj->SetBrep(brep); // brep_obj now owns brep

  // Add to document and redraw
  context.m_doc.AddObject(brep_obj);
  context.m_doc.Redraw();

  // Save for next use
  m_bSplitAtTangents = bSplitAtTangents;

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRailRevolve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
