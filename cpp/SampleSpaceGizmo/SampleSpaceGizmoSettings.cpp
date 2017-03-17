#include "stdafx.h"
#include "SampleSpaceGizmoSettings.h"

CSampleSpaceGizmoSettings::CSampleSpaceGizmoSettings()
{
  Defaults();
}

void CSampleSpaceGizmoSettings::LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  Defaults();
  pc.LoadProfileInt(lpszSection, L"rotation_scale", &m_rotation_scale);
  pc.LoadProfileInt(lpszSection, L"translation_scale", &m_translation_scale);
  pc.LoadProfileBool(lpszSection, L"rotate_parallel_view", &m_rotate_parallel_view);
  pc.LoadProfileBool(lpszSection, L"swap_pan_and_zoom", &m_swap_pan_and_zoom);
  pc.LoadProfileBool(lpszSection, L"allow_tilt", &m_allow_tilt);
  pc.LoadProfileBool(lpszSection, L"swap_zoom_with_dolly", &m_swap_zoom_with_dolly);
  pc.LoadProfileBool(lpszSection, L"rotate_about_camera", &m_rotate_about_camera);
}

void CSampleSpaceGizmoSettings::SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  pc.SaveProfileInt(lpszSection, L"rotation_scale", m_rotation_scale);
  pc.SaveProfileInt(lpszSection, L"translation_scale", m_translation_scale);
  pc.SaveProfileBool(lpszSection, L"rotate_parallel_view", m_rotate_parallel_view);
  pc.SaveProfileBool(lpszSection, L"swap_pan_and_zoom", m_swap_pan_and_zoom);
  pc.SaveProfileBool(lpszSection, L"allow_tilt", m_allow_tilt);
  pc.SaveProfileBool(lpszSection, L"swap_zoom_with_dolly", m_swap_zoom_with_dolly);
  pc.SaveProfileBool(lpszSection, L"rotate_about_camera", m_rotate_about_camera);
}

void CSampleSpaceGizmoSettings::Defaults()
{
  m_rotation_scale = 2048;
  m_translation_scale = 2048;
  m_rotate_parallel_view = FALSE;
  m_swap_pan_and_zoom = FALSE;
  m_allow_tilt = TRUE;
  m_swap_zoom_with_dolly = FALSE;
  m_rotate_about_camera = FALSE;
}