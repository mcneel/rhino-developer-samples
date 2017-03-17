#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePatch command
//

class CSamplePatchSettings
{
public:
  CSamplePatchSettings();

public:
  bool m_bAdjustTangency;
  bool m_bAutomaticTrim;
  bool m_bPreserveEdges;
  double m_dPointSpacing;
  double m_dStiffness;
  double m_dPull;
  int m_uspans;
  int m_vspans;
  int m_min_points;
  const ON_Surface* m_pStartingSurface;
};

CSamplePatchSettings::CSamplePatchSettings()
  : m_pStartingSurface(0)
{
  m_bAdjustTangency = true;
  m_bAutomaticTrim = true;
  m_bPreserveEdges = false;
  m_dPointSpacing = 1.0;
  m_dStiffness = 1.0;
  m_dPull = 1.0;
  m_uspans = 10;
  m_vspans = 10;
  m_min_points = 200;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSamplePatch : public CRhinoCommand
{
public:
  CCommandSamplePatch() {}
  ~CCommandSamplePatch() {}
  UUID CommandUUID()
  {
    // {90F4BF06-C4EB-417B-9741-4975635E5A8D}
    static const GUID SamplePatchCommand_UUID =
    { 0x90F4BF06, 0xC4EB, 0x417B, { 0x97, 0x41, 0x49, 0x75, 0x63, 0x5E, 0x5A, 0x8D } };
    return SamplePatchCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePatch"; }
  const wchar_t* LocalCommandName() const { return L"SamplePatch"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

private:
  CSamplePatchSettings m_settings;
};

// The one and only CCommandSamplePatch object
static class CCommandSamplePatch theSamplePatchCommand;

void CCommandSamplePatch::LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  bool b = false;
  double d = 0.0;
  int i = 0;

  if (pc.LoadProfileDouble(lpszSection, L"PointSpacing", &d))
    m_settings.m_dPointSpacing = RHINO_CLAMP(d, ON_DBL_MIN, ON_DBL_MAX);

  if (pc.LoadProfileInt(lpszSection, L"USpans", &i))
    m_settings.m_uspans = RHINO_CLAMP(i, 1, 255);

  if (pc.LoadProfileInt(lpszSection, L"VSpans", &i))
    m_settings.m_vspans = RHINO_CLAMP(i, 1, 255);

  if (pc.LoadProfileDouble(lpszSection, L"Stiffness", &d))
    m_settings.m_dStiffness = RHINO_CLAMP(d, 0.0, ON_DBL_MAX);

  if (pc.LoadProfileBool(lpszSection, L"AdjustTangency", &b))
    m_settings.m_bAdjustTangency = b;

  if (pc.LoadProfileBool(lpszSection, L"AutomaticTrim", &b))
    m_settings.m_bAutomaticTrim = b;

  if (pc.LoadProfileDouble(lpszSection, L"Pull", &d))
    m_settings.m_dPull = RHINO_CLAMP(d, 0.0, ON_DBL_MAX);

  if (pc.LoadProfileBool(lpszSection, L"PreserveEdges", &b))
    m_settings.m_bPreserveEdges = b;
}

void CCommandSamplePatch::SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  pc.SaveProfileDouble(lpszSection, L"PointSpacing", m_settings.m_dPointSpacing);
  pc.SaveProfileInt(lpszSection, L"USpans", m_settings.m_uspans);
  pc.SaveProfileInt(lpszSection, L"VSpans", m_settings.m_vspans);
  pc.SaveProfileDouble(lpszSection, L"Stiffness", m_settings.m_dStiffness);
  pc.SaveProfileBool(lpszSection, L"AdjustTangency", m_settings.m_bAdjustTangency);
  pc.SaveProfileBool(lpszSection, L"AutomaticTrim", m_settings.m_bAutomaticTrim);
  pc.SaveProfileDouble(lpszSection, L"Pull", m_settings.m_dPull);
  pc.SaveProfileBool(lpszSection, L"PreserveEdges", m_settings.m_bPreserveEdges);
}

CRhinoCommand::result CCommandSamplePatch::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curves, points, point clouds and meshes to fit surface through");
  go.EnableGroupSelect();
  go.SetGeometryFilter(go.curve_object |
    go.point_object |
    go.pointset_object |
    go.edge_object |
    go.mesh_object |
    go.grip_object
  );
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  ON_SimpleArray<const ON_Geometry*> geometry(object_count);
  for (int i = 0; i < object_count; i++)
    geometry.Append(go.Object(i).Geometry());

  CSamplePatchSettings args = m_settings;
  CRhinoCommand::result rc = CRhinoCommand::failure;

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to accept settings");
  for (;;)
  {
    gs.ClearCommandOptions();
    gs.AcceptNothing();

    gs.AddCommandOptionNumber(RHCMDOPTNAME(L"PointSpacing"), &args.m_dPointSpacing, L"Sample point spacing", false, ON_DBL_MIN, ON_DBL_MAX);

    int uspans_option = -1;
    int	vspans_option = -1;
    if (!args.m_pStartingSurface)
    {
      uspans_option = gs.AddCommandOptionInteger(RHCMDOPTNAME(L"USpans"), &args.m_uspans, L"Number of U spans", 1, 255);
      vspans_option = gs.AddCommandOptionInteger(RHCMDOPTNAME(L"VSpans"), &args.m_vspans, L"Number of V spans", 1, 255);
    }

    gs.AddCommandOptionNumber(RHCMDOPTNAME(L"Stiffness"), &args.m_dStiffness, L"Stiffness", false, 0.0, ON_DBL_MAX);
    gs.AddCommandOptionToggle(RHCMDOPTNAME(L"AdjustTangency"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), args.m_bAdjustTangency, &args.m_bAdjustTangency);
    gs.AddCommandOptionToggle(RHCMDOPTNAME(L"AutomaticTrim"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), args.m_bAutomaticTrim, &args.m_bAutomaticTrim);

    int pull_option = -1;
    int preserveedges_option = -1;
    if (args.m_pStartingSurface)
    {
      pull_option = gs.AddCommandOptionNumber(RHCMDOPTNAME(L"Pull"), &args.m_dPull, L"Starting surface pull", false, 0.0, ON_DBL_MAX);
      preserveedges_option = gs.AddCommandOptionToggle(RHCMDOPTNAME(L"PreserveEdges"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), args.m_bPreserveEdges, &args.m_bPreserveEdges);
    }

    int startingsurface_option = gs.AddCommandOption(RHCMDOPTNAME(L"StartingSurface"));

    CRhinoGet::result res = gs.GetString();

    if (res == CRhinoGet::cancel)
    {
      rc = CRhinoCommand::cancel;
      break;
    }

    if (res == CRhinoGet::nothing || res == CRhinoGet::string)
    {
      rc = CRhinoCommand::success;
      break;
    }

    if (res != CRhinoGet::option)
      break;

    int index = gs.Option()->m_option_index;
    if (index == startingsurface_option)
    {
      args.m_pStartingSurface = 0;

      CRhinoGetObject go1;
      go1.SetCommandPrompt(L"Select starting surface");
      go1.SetGeometryFilter(CRhinoObject::surface_object);
      go1.AcceptNothing();
      go1.EnablePreSelect(false);
      go1.EnableDeselectAllBeforePostSelect(false);
      go1.EnableAlreadySelectedObjectSelect(true);
      go1.GetObjects(1, 1);
      if (go1.Result() == CRhinoGet::cancel)
      {
        rc = CRhinoCommand::cancel;
        break;
      }
      if (go1.Result() == CRhinoGet::object)
      {
        args.m_pStartingSurface = go1.Object(0).Surface();
      }
    }
    continue;
  }

  if (rc != CRhinoCommand::success)
    return rc;

  ON_Brep* brep = 0;
  int false_edges[] = { 0, 0, 0, 0 };
  int true_edges[] = { 1, 1, 1, 1 };

  if (args.m_pStartingSurface)
  {
    brep = CRhinoFitPatch::Fit(
      geometry,
      args.m_pStartingSurface, 0, 0,
      args.m_bAutomaticTrim, args.m_bAdjustTangency,
      args.m_dPointSpacing, args.m_dStiffness, args.m_dPull,
      args.m_bPreserveEdges ? true_edges : false_edges,
      context.m_doc.AbsoluteTolerance()
    );
  }
  else
  {
    brep = CRhinoFitPatch::Fit(
      geometry,
      0, args.m_uspans, args.m_vspans,
      args.m_bAutomaticTrim, args.m_bAdjustTangency,
      args.m_dPointSpacing, args.m_dStiffness, 0.0,
      args.m_bPreserveEdges ? true_edges : false_edges,
      context.m_doc.AbsoluteTolerance()
    );
  }

  if (brep)
  {
    CRhinoBrepObject* brep_obj = new CRhinoBrepObject();
    brep_obj->SetBrep(brep);
    context.m_doc.AddObject(brep_obj);
    context.m_doc.Redraw();
  }

  args.m_pStartingSurface = 0;
  m_settings = args;

  return CRhinoCommand::success;
}

//
// END SamplePatch command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
