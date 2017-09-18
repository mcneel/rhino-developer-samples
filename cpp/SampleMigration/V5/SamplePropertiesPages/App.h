#pragma once

class CApp
{
private:
  // Call CApp::App() to get access to the one and only application object
  CApp();
  ~CApp();

public:
  // Provides access to the one and only application object
  static CApp& App();
  // Default object color, if the application color is not set then this
  // color is returned
  static const COLORREF DEFULLT_OBJECT_COLOR;
  // Color to use when changing object color in the object properties panel
  COLORREF ObjectColor(CRhinoDoc* doc);
  // Color to use when changing object color in the object properties panel
  void SetObjectColor(CRhinoDoc* doc, COLORREF color);
  // Application specific default color value
  COLORREF DefaultObjectColor();
  // Application specific default color value, set it to ON_Color::UnsetColor 
  // to use the DEFULLT_OBJECT_COLOR value
  void SetDefaultObjectColor(COLORREF color);

  // Plug-in helpers, called from CV5PageTestPlugIn to save and load the
  // application specific default object color
  void LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
  void SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );

private:
  COLORREF m_default_object_color;
};
