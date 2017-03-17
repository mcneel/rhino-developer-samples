#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePlaceCameraTarget command
//

#pragma region SamplePlaceCameraTarget command

class CCommandSamplePlaceCameraTarget : public CRhinoCommand
{
public:
  CCommandSamplePlaceCameraTarget() {}
  ~CCommandSamplePlaceCameraTarget() {}
  UUID CommandUUID()
  {
    // {AD9E6A0-A112-4708-8366-D50AEAE17AED}
    static const GUID SamplePlaceCameraTargetCommand_UUID =
    { 0xAD9E6A0, 0xA112, 0x4708, { 0x83, 0x66, 0xD5, 0x0A, 0xEA, 0xE1, 0x7A, 0xED } };
    return SamplePlaceCameraTargetCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePlaceCameraTarget"; }
  const wchar_t* LocalCommandName() const { return L"SamplePlaceCameraTarget"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CRhinoGet::result PickCameraTargetPoint(ON_3dPoint& pt, bool bGetCamera);
};

// The one and only CCommandSamplePlaceCameraTarget object
static class CCommandSamplePlaceCameraTarget theSamplePlaceCameraTargetCommand;

CRhinoCommand::result CCommandSamplePlaceCameraTarget::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoView* rhino_view = RhinoApp().ActiveView();
  if (0 == rhino_view)
    return CRhinoCommand::failure;

  const ON_3dmView& view = rhino_view->ActiveViewport().View();
  ON_3dPoint camera = view.m_vp.CameraLocation();
  ON_3dPoint target = view.TargetPoint();

  bool bCameraChanged = false, bTargetChanged = false;

  CRhinoGet::result res = PickCameraTargetPoint(camera, true);
  if (res == CRhinoGet::point)
    bCameraChanged = true;
  else if (res == CRhinoGet::nothing)
    bCameraChanged = false;
  else
    return CRhinoCommand::cancel;

  res = PickCameraTargetPoint(target, false);
  if (res == CRhinoGet::point)
    bTargetChanged = true;
  else if (res == CRhinoGet::nothing)
    bTargetChanged = false;
  else
    return CRhinoCommand::cancel;

  if (!bCameraChanged && !bTargetChanged)
    return CRhinoCommand::nothing;

  if (camera.DistanceTo(target) <= ON_ZERO_TOLERANCE)
  {
    RhinoApp().Print(L"Camera and target locations cannot be the same.\n");
    return CRhinoCommand::nothing;
  }

  if (view.m_vp.Projection() == ON::parallel_view)
  {
    if (bCameraChanged && bTargetChanged)
      rhino_view->ActiveViewport().SetTargetAndCameraLocation(target, camera, true);
    else if (bTargetChanged)
      rhino_view->ActiveViewport().SetTarget(target, true);
    else if (bCameraChanged)
      rhino_view->ActiveViewport().SetCameraLocation(camera, true);
  }
  else
  {
    rhino_view->ActiveViewport().SetTargetAndCameraLocation(target, camera, true);
  }

  rhino_view->Redraw();

  return CRhinoCommand::success;
}

CRhinoGet::result CCommandSamplePlaceCameraTarget::PickCameraTargetPoint(ON_3dPoint& pt, bool bGetCamera)
{
  ON_wString wPrompt = (bGetCamera) ? L"New camera location" : L"New target location";

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(wPrompt);
  gp.SetDefaultPoint(pt);
  CRhinoGet::result res = gp.GetPoint();

  if (res == CRhinoGet::point)
  {
    ON_3dPoint point = gp.Point();
    if ((point - pt).IsTiny())
      res = CRhinoGet::nothing;
    else
      pt = point;
  }

  return res;
}

#pragma endregion

//
// END SamplePlaceCameraTarget command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
