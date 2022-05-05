#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRotateCamera command
//

#pragma region SampleRotateCamera command

class CCommandSampleRotateCamera : public CRhinoCommand
{
public:
  CCommandSampleRotateCamera() = default;
  UUID CommandUUID() override
  {
    // {69B049B5-9D8B-4891-B72B-3C3B84535267}
    static const GUID SampleRotateCameraCommand_UUID =
    { 0x69B049B5, 0x9D8B, 0x4891, { 0xB7, 0x2B, 0x3C, 0x3B, 0x84, 0x53, 0x52, 0x67 } };
    return SampleRotateCameraCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleRotateCamera"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRotateCamera object
static class CCommandSampleRotateCamera theSampleRotateCameraCommand;

CRhinoCommand::result CCommandSampleRotateCamera::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoView* pView = RhinoApp().ActiveView();
  if (nullptr == pView)
    return CRhinoCommand::failure;

  CRhinoViewport* pViewport = &(pView->ActiveViewport());
  if (pViewport->View().m_bLockedProjection)
    pViewport = &(pView->MainViewport());
  if (!pViewport->VP().IsPerspectiveProjection())
    return CRhinoCommand::nothing;

  const ON_Viewport& vp_in = pViewport->VP();
  ON_3dPoint camLoc;
  ON_3dVector camX, camY, camZ;
  if (!vp_in.GetCameraFrame(camLoc, camX, camY, camZ))
    return CRhinoCommand::failure;

  const CRhinoAppViewSettings& view_settings = RhinoApp().AppSettings().ViewSettings();
  double angle = 2.0 * ON_PI / view_settings.m_rotate_circle_increment;
  double target_distance = (camLoc - pViewport->Target()) * camZ;

  // Rotate about cplane x-axis
  ON_3dVector axis = pViewport->View().m_cplane.m_plane.xaxis;

  ON_Xform rot;
  rot.Rotation(angle, axis, ON_origin);
  const ON_3dVector camUp = rot * camY;
  const ON_3dVector camDir = -(rot * camZ);

  ON_3dPoint target = camLoc + target_distance * camDir;
  pViewport->SetTarget(target, true);

  ON_Viewport vp_out = vp_in;
  vp_out.SetCameraLocation(camLoc);
  vp_out.SetCameraDirection(camDir);
  vp_out.SetCameraUp(camUp);

  pViewport->SetVP(vp_out, FALSE);
  pView->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRotateCamera command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
