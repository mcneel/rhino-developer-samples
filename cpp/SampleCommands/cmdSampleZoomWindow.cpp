#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleZoomWindow command
//

#pragma region SampleZoomWindow command

class CCommandSampleZoomRotateWindow : public CRhinoCommand
{
public:
  CCommandSampleZoomRotateWindow() {}
  ~CCommandSampleZoomRotateWindow() {}
  UUID CommandUUID()
  {
    // {BEDC408D-2585-4975-AA18-3ED78CD27DD1}
    static const GUID SampleZoomRotateWindowCommand_UUID =
    { 0xBEDC408D, 0x2585, 0x4975, { 0xAA, 0x18, 0x3E, 0xD7, 0x8C, 0xD2, 0x7D, 0xD1 } };
    return SampleZoomRotateWindowCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleZoomWindow"; }
  const wchar_t* LocalCommandName() const { return L"SampleZoomWindow"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool DollyWindow(const CRhinoViewport& vport, CRect rect, ON_3dPoint& target_point, ON_Viewport& view_out);

};

// The one and only CCommandSampleZoomRotateWindow object
static class CCommandSampleZoomRotateWindow theSampleZoomRotateWindowCommand;

CRhinoCommand::result CCommandSampleZoomRotateWindow::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Drag a window to zoom");
  gp.SetGetPointCursor(RhinoApp().m_default_cursor);
  gp.ConstrainToTargetPlane();
  gp.AcceptNothing();
  switch (gp.Get2dRectangle())
  {
  case CRhinoGet::rect2d:
    break;
  default:
    return CRhinoCommand::cancel;
  }

  CRhinoView* view = gp.View();
  if (0 == view)
    return CRhinoCommand::failure;

  CRhinoViewport& vport = view->ActiveViewport();

  if (!vport.VP().IsParallelProjection())
  {
    RhinoApp().Print(L"This sample only works in parallel-projected views.\n");
    return CRhinoCommand::nothing;
  }

  ON_3dPoint target_point = vport.Target();
  CRect pick_rect = gp.Rectangle2d();
  ON_Viewport vp_out;

  if (DollyWindow(vport, pick_rect, target_point, vp_out))
  {
    vport.m_v.SetTargetPoint(target_point);
    vp_out.SetTargetPoint(target_point);
    vport.SetVP(vp_out, false);
    view->Redraw();
  }

  return CRhinoCommand::success;
}

bool CCommandSampleZoomRotateWindow::DollyWindow(const CRhinoViewport& vport, CRect pick_rect, ON_3dPoint& target_point, ON_Viewport& vp_out)
{
  const ON_Viewport& vp_in = vport.VP();
  vp_out = vp_in;

  // screen port values
  int sleft, sright, stop, sbottom;
  if (!vp_in.GetScreenPort(&sleft, &sright, &sbottom, &stop))
    return false;

  // frustum values
  double fleft, fright, ftop, fbottom, fnear, ffar;
  if (!vp_in.GetFrustum(&fleft, &fright, &fbottom, &ftop, &fnear, &ffar))
    return false;

  // camera coordinate system
  ON_3dPoint cam_loc = vp_in.CameraLocation();
  ON_3dVector cam_z = vp_in.CameraZ();

  // capture Depth Buffer
  CRect screen_rect(sleft, stop, sright, sbottom);

  // user-specified rectangle
  CRect zoom_rect;
  zoom_rect.IntersectRect(&pick_rect, screen_rect);

  CRhinoZBuffer zbuffer(vport);
  zbuffer.ShowIsocurves(true);
  zbuffer.ShowMeshWires(true);
  zbuffer.ShowCurves(true);
  zbuffer.ShowPoints(true);
  zbuffer.ShowText(true);
  zbuffer.ShowAnnotations(false);

  ON_UUID display_mode_id = vport.View().m_display_mode_id;
  bool bSeeThrough = (display_mode_id == ON_StandardDisplayModeId::XrayShade || display_mode_id == ON_StandardDisplayModeId::Ghosted);
  if (bSeeThrough)
    zbuffer.EnableShading(false);

  bool rc = zbuffer.Capture(zoom_rect);
  if (rc && bSeeThrough && 0 == zbuffer.HitCount())
  {
    zbuffer.EnableShading(true);
    zbuffer.ShowIsocurves(false);
    zbuffer.ShowMeshWires(false);
    zbuffer.ShowCurves(false);
    zbuffer.ShowPoints(false);
    zbuffer.ShowText(false);
    rc = zbuffer.Capture(zoom_rect);
  }

  ON_Xform s2c;
  if (rc)
    rc = zbuffer.VP().GetXform(ON::screen_cs, ON::camera_cs, s2c);

  if (rc)
  {
    // dolly sideways so zoom rectangle is centered
    ON_3dPoint near_rect[4];
    vp_in.GetNearRect(near_rect[0], near_rect[1], near_rect[2], near_rect[3]);

    double port_dx = fabs(double(sright - sleft));
    double port_dy = fabs(double(stop - sbottom));

    ON_Interval lr(sleft, sright);
    double nx = lr.NormalizedParameterAt(0.5 * (pick_rect.left + pick_rect.right));

    ON_Interval bt(sbottom, stop);
    double ny = bt.NormalizedParameterAt(0.5 * (pick_rect.bottom + pick_rect.top));

    ON_3dPoint zoom_center = (1 - nx) * (1 - ny) * near_rect[0] +
      nx * (1 - ny) * near_rect[1] +
      (1 - nx) * ny * near_rect[2] +
      nx *ny * near_rect[3];

    ON_3dVector dolly_vec = zoom_center - cam_loc;
    // dolly perpendicular to cam_z
    dolly_vec -= dolly_vec * cam_z * cam_z;
    vp_out.DollyCamera(dolly_vec);

    double pick_rect_dx = fabs(double(pick_rect.right - pick_rect.left));
    double pick_rect_dy = fabs(double(pick_rect.top - pick_rect.bottom));

    // expand pick_rect to have the aspect ratio of the viewport
    double d = 1.0;
    if (pick_rect_dx / port_dx < pick_rect_dy / port_dy)
      d = pick_rect_dy / port_dy;
    else
      d = pick_rect_dx / port_dx;

    fleft *= d;
    fright *= d;
    fbottom *= d;
    ftop *= d;
    vp_out.SetFrustum(fleft, fright, fbottom, ftop, fnear, ffar);

    // target point on plane perpendicular to cam_z
    cam_loc = vp_out.CameraLocation();
    target_point = cam_loc + (target_point - cam_loc) *cam_z * cam_z;
  }

  return rc;
}


#pragma endregion

//
// END SampleZoomWindow command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
