#include "StdAfx.h"
#include "App.h"

const COLORREF CApp::DEFULLT_OBJECT_COLOR = RGB(255,0,0);

CApp::CApp(void)
: m_default_object_color(ON_Color::UnsetColor)
{
}

CApp::~CApp(void)
{
}

CApp& CApp::App()
{
  static CApp app;
  return app;
}

#define DefaultObjectColorKey L"DefaultObjectColor"

void CApp::LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc )
{
  COLORREF color = m_default_object_color;
  if (pc.LoadProfileColor(lpszSection, DefaultObjectColorKey, &color) && color!= m_default_object_color)
    m_default_object_color = color;
}

void CApp::SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc )
{
  if (m_default_object_color == ON_Color::UnsetColor || m_default_object_color == DEFULLT_OBJECT_COLOR)
    pc.SaveProfileString(lpszSection, DefaultObjectColorKey, (const wchar_t*)NULL);
  else
    pc.SaveProfileColor(lpszSection, DefaultObjectColorKey, m_default_object_color);
}

#define DOC_COLOR_KEY L"CApp::ObjectColor{46F3AD57-D6B6-42A5-8B95-BB45227219B6}"

COLORREF CApp::ObjectColor(CRhinoDoc* doc)
{
  // Check to see if the document contains a color override
  ON_wString s;
  if (doc ==nullptr || !doc->GetUserString(DOC_COLOR_KEY, s))
    return DefaultObjectColor(); // No override found so use the default color
  // Make sure the string is not an empty string
  s.TrimLeftAndRight();
  if (s.IsEmpty())
    return DefaultObjectColor();
  // Convert the string to a COLORREF
  COLORREF color = _wtol(s);
  return color;
}

void CApp::SetObjectColor(CRhinoDoc* doc, COLORREF color)
{
  // Delete the document user string if setting to the default color
  if (doc && (color == DefaultObjectColor() || color == ON_Color::UnsetColor))
    doc->SetUserString(DOC_COLOR_KEY, nullptr);
  else if (doc)
  {
    // Convert the COLORREF to a string and write the string
    // to the document
    wchar_t s[10];
    memset(s, 0, sizeof(s));
    swprintf(s, _countof(s), L"%d", color);
    doc->SetUserString(DOC_COLOR_KEY, s);
  }
}

COLORREF CApp::DefaultObjectColor()
{
  return m_default_object_color == ON_Color::UnsetColor ? DEFULLT_OBJECT_COLOR : m_default_object_color;
}

void CApp::SetDefaultObjectColor(COLORREF color)
{
  if (color == DEFULLT_OBJECT_COLOR)
    m_default_object_color = ON_Color::UnsetColor;
  else
    m_default_object_color = color;
}
