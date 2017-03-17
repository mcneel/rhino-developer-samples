#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMoveCPlane command
//

class CSampleMoveCPlanePoint : public CRhinoGetPoint
{
public:
  CSampleMoveCPlanePoint(const ON_3dmConstructionPlane& cplane);
  ~CSampleMoveCPlanePoint() = default;

  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* pt2d) override;
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt) override;

private:
  ON_3dmConstructionPlane m_cplane;
  ON_Color m_axis_color;
};

CSampleMoveCPlanePoint::CSampleMoveCPlanePoint(const ON_3dmConstructionPlane& cplane)
  : m_cplane(cplane)
{
  const CRhinoAppSettings& app_settings = RhinoApp().AppSettings();
  m_axis_color = app_settings.LockedObjectColor();
}

void CSampleMoveCPlanePoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* pt2d)
{
  m_cplane.m_plane.CreateFromFrame(pt, m_cplane.m_plane.xaxis, m_cplane.m_plane.yaxis);
  CRhinoGetPoint::OnMouseMove(vp, flags, pt, pt2d);
}

void CSampleMoveCPlanePoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  dp.DrawConstructionPlane(m_cplane, FALSE, TRUE, FALSE, ON_UNSET_COLOR, ON_UNSET_COLOR, m_axis_color, m_axis_color);
  CRhinoGetPoint::DynamicDraw(dp, pt);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMoveCPlane : public CRhinoCommand
{
public:
  CCommandSampleMoveCPlane() = default;
  ~CCommandSampleMoveCPlane() = default;
  UUID CommandUUID() override
  {
    // {8CEFCD61-C5F5-4D9E-8D54-691FAE970E0B}
    static const GUID SampleMoveCPlaneCommand_UUID =
    { 0x8CEFCD61, 0xC5F5, 0x4D9E, { 0x8D, 0x54, 0x69, 0x1F, 0xAE, 0x97, 0x0E, 0x0B } };
    return SampleMoveCPlaneCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMoveCPlane"; }
  const wchar_t* LocalCommandName() const override { return L"SampleMoveCPlane"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMoveCPlane object
static class CCommandSampleMoveCPlane theSampleMoveCPlaneCommand;

CRhinoCommand::result CCommandSampleMoveCPlane::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoView* view = ::RhinoApp().ActiveView();
  if (!view)
    return CRhinoCommand::failure;

  ON_3dmConstructionPlane cplane = view->Viewport().ConstructionPlane();
  ON_3dPoint origin = cplane.m_plane.origin;

  CSampleMoveCPlanePoint gp(cplane);
  gp.SetCommandPrompt(L"CPlane origin");
  gp.SetBasePoint(origin);
  gp.DrawLineFromPoint(origin, TRUE);
  gp.GetPoint();

  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint pt = gp.Point();
  ON_3dVector v = origin - pt;
  if (v.IsTiny())
    return CRhinoCommand::nothing;

  cplane.m_plane.CreateFromFrame(pt, cplane.m_plane.xaxis, cplane.m_plane.yaxis);
  view->Viewport().SetConstructionPlane(cplane);
  view->Redraw();

  return CRhinoCommand::success;
}

//
// END SampleMoveCPlane command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
