#pragma once

class CSampleSpaceGizmoSettings
{
public:
  CSampleSpaceGizmoSettings();

  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void Defaults();

public:
  int m_rotation_scale;
  int m_translation_scale;
  BOOL m_rotate_parallel_view;
  BOOL m_swap_pan_and_zoom;
  BOOL m_allow_tilt;
  BOOL m_swap_zoom_with_dolly;
  BOOL m_rotate_about_camera;
};