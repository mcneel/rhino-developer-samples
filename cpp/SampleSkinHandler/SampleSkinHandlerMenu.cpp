/////////////////////////////////////////////////////////////////////////////
// HippoPlugInApp.cpp

#include "StdAfx.h"
#include "SampleSkinHandlerPlugIn.h"
#include "../SampleSkin/Resource.h"

void CSampleSkinHandlerPlugIn::OnInitPlugInMenuPopups(WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(wParam);
  UNREFERENCED_PARAMETER(lParam);
  // TODO
}

BOOL CSampleSkinHandlerPlugIn::OnPlugInMenuCommand(WPARAM wParam)
{
  switch (wParam)
  {
  case ID_FILE_NEW:
    RhinoApp().RunMenuScript(L"! _New");
    break;
  case ID_FILE_OPEN:
    RhinoApp().RunMenuScript(L"! _Open");
    break;
  case ID_FILE_SAVE:
    RhinoApp().RunMenuScript(L"! _Save");
    break;
  case ID_FILE_SAVEAS:
    RhinoApp().RunMenuScript(L"! _SaveAs");
    break;
  case ID_FILE_EXIT:
    RhinoApp().RunMenuScript(L"! _Exit");
    break;
  }

  return TRUE;
}