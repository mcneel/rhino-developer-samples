#include "stdafx.h"
#include "SampleSpaceGizmoWnd.h"
#include "SampleSpaceGizmoPlugIn.h"
#include "SampleSpaceGizmoView.h"

IMPLEMENT_DYNAMIC(CSampleSpaceGizmoWnd, CWnd)

UINT CSampleSpaceGizmoWnd::WM_SPACEGIZMO = ::RegisterWindowMessage(L"SpaceWareMessage00");

CSampleSpaceGizmoWnd::CSampleSpaceGizmoWnd()
  : m_siHdl(0)
{
}

CSampleSpaceGizmoWnd::~CSampleSpaceGizmoWnd()
{
  if (m_siHdl)
    SiClose(m_siHdl);
}

BEGIN_MESSAGE_MAP(CSampleSpaceGizmoWnd, CWnd)
  ON_REGISTERED_MESSAGE(CSampleSpaceGizmoWnd::WM_SPACEGIZMO, &CSampleSpaceGizmoWnd::OnSpaceGizmo)
END_MESSAGE_MAP()

BOOL CSampleSpaceGizmoWnd::InitializeSpaceGizmo()
{
  if (SPW_NO_ERROR != SiInitialize())
  {
    TRACE(L"SpaceWare DLL Load Failure\n");
    return FALSE;
  }

  SiOpenData siOpenData;
  memset(&siOpenData, 0, sizeof(SiOpenData));
  SiOpenWinInit(&siOpenData, GetSafeHwnd());

  m_siHdl = SiOpen("Rhino50", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &siOpenData);
  if (0 == m_siHdl)
  {
    SiTerminate();
    TRACE(L"SpaceWare Open Failure\n");
    return FALSE;
  }

  SiSetUiMode(m_siHdl, SI_UI_NO_CONTROLS);
  SiGrabDevice(m_siHdl, SPW_FALSE);

  return TRUE;
}

LRESULT CSampleSpaceGizmoWnd::OnSpaceGizmo(WPARAM wParam, LPARAM lParam)
{
  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view || 0 == m_siHdl)
    return TRUE;

  SiGetEventData siEventData;
  SiGetEventWinInit(&siEventData, CSampleSpaceGizmoWnd::WM_SPACEGIZMO, wParam, lParam);

  SiSpwEvent siEvent;
  if (SI_IS_EVENT == SiGetEvent(m_siHdl, 0, &siEventData, &siEvent))
  {
    switch (siEvent.type)
    {
    case SI_MOTION_EVENT:
    {
      long &Tx = siEvent.u.spwData.mData[SI_TX];
      long &Ty = siEvent.u.spwData.mData[SI_TY];
      long &Tz = siEvent.u.spwData.mData[SI_TZ];
      long &Rx = siEvent.u.spwData.mData[SI_RX];
      long &Ry = siEvent.u.spwData.mData[SI_RY];
      long &Rz = siEvent.u.spwData.mData[SI_RZ];

      if ((Tx == 0) && (Ty == 0) && (Tz == 0) && (Rx == 0) && (Ry == 0) && (Rz == 0))
        break;

      const CSampleSpaceGizmoSettings& settings = SampleSpaceGizmoPlugIn().SpaceGizmoSettings();
      BOOL relative_to_view = RhinoApp().AppSettings().ViewSettings().m_rotate_to_view;

      double rotation_scale = settings.m_rotation_scale;
      double translation_scale = settings.m_translation_scale;
      BOOL rotate_parallel_view = settings.m_rotate_parallel_view;
      BOOL swap_pan_and_zoom = settings.m_swap_pan_and_zoom;
      BOOL allow_tilt = settings.m_allow_tilt;

      double X, Y, Z, A, B, C;

      X = -Tx / translation_scale;

      if (!swap_pan_and_zoom)
      {
        Y = Ty / translation_scale;
        Z = Tz / translation_scale;
      }
      else
      {
        Z = -Ty / translation_scale;
        Y = Tz / translation_scale;
      }

      A = Rx / rotation_scale;
      B = Ry / rotation_scale;
      C = Rz / rotation_scale;

      CRhinoViewport& active_vp = view->ActiveViewport();
      ON_3dPoint camLoc = active_vp.VP().CameraLocation();
      ON_3dPoint target = active_vp.Target();

      int port_left, port_right, port_bottom, port_top;
      active_vp.VP().GetScreenPort(&port_left, &port_right, &port_bottom, &port_top);
      //int height = port_top - port_bottom;
      //int width = port_right - port_left;

      double frus_left, frus_right, frus_bottom, frus_top, frus_near;
      active_vp.VP().GetFrustum(&frus_left, &frus_right, &frus_bottom, &frus_top, &frus_near);

      // pan
      {
        double dx, dy, s;

        dx = X * 0.5; // using arbitary fudge factors
        dx *= (frus_right - frus_left);

        dy = Y*0.5; // using arbitary fudge factors
        dy *= (frus_top - frus_bottom);

        if (active_vp.VP().Projection() == ON::perspective_view)
        {
          s = target.DistanceTo(camLoc) / active_vp.VP().FrustumNear();
          dx *= s;
          dy *= s;
        }

        ON_3dVector dolly_vector = dx * active_vp.VP().CameraX() - dy * active_vp.VP().CameraY();
        active_vp.SetTarget(active_vp.Target() + dolly_vector);
        camLoc = camLoc + dolly_vector;
        active_vp.m_v.m_vp.SetCameraLocation(camLoc);
      }

      // zoom
      if (settings.m_swap_zoom_with_dolly && (active_vp.VP().Projection() == ON::perspective_view))
        active_vp.InOutDolly(-Z);
      else
        active_vp.Magnify(1.0 - Z, 0);

      // Camera and target positions may have changed above, so we need to re-get them...
      camLoc = active_vp.VP().CameraLocation();
      target = active_vp.Target();

      // rotate
      if (rotate_parallel_view || active_vp.VP().Projection() == ON::perspective_view)
      {
        ON_Xform rot1, rot2, rot;
        BOOL swapRotate = settings.m_rotate_about_camera;
        ON_3dPoint center = swapRotate ? camLoc : target;

        double d = target.DistanceTo(camLoc);
        if (!relative_to_view)
          rot1.Rotation(-B, ON_zaxis, center);
        else
          rot1.Rotation(-B, active_vp.VP().CameraY(), center);
        rot2.Rotation(-A, active_vp.VP().CameraX(), center);

        rot = rot1 * rot2;
        ON_3dVector up = active_vp.VP().CameraY();
        ON_3dVector dir = -active_vp.VP().CameraZ();

        up = rot * up;
        active_vp.m_v.m_vp.SetCameraUp(up);

        if (swapRotate)
        {
          target = rot * target;
          active_vp.m_v.SetTargetPoint(target);
          active_vp.m_v.m_vp.SetCameraDirection(target - camLoc);
        }
        else
        {
          dir = rot * dir;
          active_vp.m_v.m_vp.SetCameraUp(up);
          active_vp.m_v.m_vp.SetCameraDirection(dir);

          ON_3dPoint target_point = active_vp.Target();

          ON_3dPoint  loc = target_point - d*dir;
          active_vp.m_v.m_vp.SetCameraLocation(loc);
        }

        // tilt
        if (allow_tilt)
        {
          //double r = (width >= height) ? width : height;
          rot.Rotation(C, active_vp.VP().CameraZ(), camLoc);
          up = active_vp.VP().CameraY();
          up = rot * up;
          active_vp.m_v.m_vp.SetCameraUp(up);
        }
      }

      static_cast<CSampleSpaceGizmoView*>(view)->SetInDynamicViewChange(true);
      static_cast<CSampleSpaceGizmoView*>(view)->UpdateTransforms();
      view->Redraw();
      break;
    }

    case SI_ZERO_EVENT:
      if (static_cast<CSampleSpaceGizmoView*>(view)->InDynamicViewChange())
      {
        static_cast<CSampleSpaceGizmoView*>(view)->SetInDynamicViewChange(false);
        view->Redraw();
      }
      break;

    case SI_BUTTON_EVENT:
      break;

    default:
      break;
    }
  }

  return TRUE;
}
