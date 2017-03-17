#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN Sample2View command
//

class CCommandSample2View : public CRhinoCommand
{
public:
  CCommandSample2View() {}
  ~CCommandSample2View() {}
  UUID CommandUUID() override
  {
    // {541D4662-584-47FC-817D-CE1439767139}
    static const GUID Sample2ViewCommand_UUID =
    { 0x541D4662, 0x584, 0x47FC, { 0x81, 0x7D, 0xCE, 0x14, 0x39, 0x76, 0x71, 0x39 } };
    return Sample2ViewCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"Sample2View"; }
  const wchar_t* LocalCommandName() { return L"Sample2View"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&) override;
};

// The one and only CCommandSample2View object
static class CCommandSample2View theSample2ViewCommand;

CRhinoCommand::result CCommandSample2View::RunCommand(const CRhinoCommandContext& context)
{
  ON_3dmView views[2];
  double def_size = 15.0;
  ON_BoundingBox bbox;
  bbox.m_min.Set(-def_size, -def_size, -def_size);
  bbox.m_max.Set(def_size, def_size, def_size);
  ON_3dPoint target = ON_origin;
  const CRhinoAppViewSettings& view_settings = RhinoApp().AppSettings().ViewSettings();

  // top view
  {
    views[0].m_name = L"Top";
    views[0].SetTargetPoint(target);
    ON_3dVector dir(0.0, 0.0, -100.0);
    views[0].m_vp.SetCameraLocation(views[0].TargetPoint() - dir);
    views[0].m_vp.SetCameraDirection(dir);
    views[0].m_vp.SetCameraUp(ON_yaxis);
    views[0].m_vp.SetProjection(ON::parallel_view);
    views[0].m_vp.SetScreenPort(0, 100, 100, 0, 0, 1);
    views[0].m_vp.Extents(atan(12.0 / view_settings.m_camera_lense_length), bbox);
    views[0].m_cplane.m_plane = ON_xy_plane;
    views[0].m_position.m_wnd_left = 0.0;
    views[0].m_position.m_wnd_right = 0.5;
    views[0].m_position.m_wnd_top = 0.0;
    views[0].m_position.m_wnd_bottom = 1.0;
  }

  // perspective view
  {
    views[1].m_name = L"Perspective";
    views[1].SetTargetPoint(target);
    ON_3dVector dir(-43.30, 75.00, -50.00);
    views[1].m_vp.SetCameraLocation(views[1].TargetPoint() - dir);
    views[1].m_vp.SetCameraDirection(dir);
    views[1].m_vp.SetCameraUp(ON_zaxis);
    views[1].m_vp.SetProjection(ON::perspective_view);
    views[1].m_vp.SetScreenPort(0, 100, 100, 0, 0, 1);
    views[1].m_vp.Extents(atan(12.0 / view_settings.m_camera_lense_length), bbox);
    views[1].m_cplane.m_plane = ON_xy_plane;
    views[1].m_position.m_wnd_left = 0.5;
    views[1].m_position.m_wnd_right = 1.0;
    views[1].m_position.m_wnd_top = 0.0;
    views[1].m_position.m_wnd_bottom = 1.0;
  }

  context.m_doc.ReplaceModelViews(2, views);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END Sample2View command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
