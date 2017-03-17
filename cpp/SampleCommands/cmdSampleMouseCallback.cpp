#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMouseCallback command
//

class CSampleMouseCallback : public CRhinoMouseCallback
{
public:
  CSampleMouseCallback() {}
  ~CSampleMouseCallback() {}
  BOOL OnMouseDown(CRhinoView&, CRhinoMouseCallback::mouse_button, UINT, CPoint);
  BOOL OnMouseMove(CRhinoView&, UINT, CPoint);
  BOOL OnMouseUp(CRhinoView&, CRhinoMouseCallback::mouse_button, UINT, CPoint);
  BOOL OnMouseDblClk(CRhinoView&, CRhinoMouseCallback::mouse_button, UINT, CPoint);
};

// The one and only CSampleMouseCallback object
static class CSampleMouseCallback theSampleMouseCallback;

BOOL CSampleMouseCallback::OnMouseDown(CRhinoView&, CRhinoMouseCallback::mouse_button, UINT, CPoint)
{
  TRACE0("CSampleMouseCallback::OnMouseDown\n");
  return FALSE;
}

BOOL CSampleMouseCallback::OnMouseMove(CRhinoView&, UINT, CPoint)
{
  TRACE0("CSampleMouseCallback::OnMouseMove\n");
  return FALSE;
}

BOOL CSampleMouseCallback::OnMouseUp(CRhinoView&, CRhinoMouseCallback::mouse_button, UINT, CPoint)
{
  TRACE0("CSampleMouseCallback::OnMouseUp\n");
  return FALSE;
}

BOOL CSampleMouseCallback::OnMouseDblClk(CRhinoView&, CRhinoMouseCallback::mouse_button, UINT, CPoint)
{
  TRACE0("CSampleMouseCallback::OnMouseDblClk\n");
  return FALSE;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleMouseCallback command

class CCommandSampleMouseCallback : public CRhinoCommand
{
public:
  CCommandSampleMouseCallback() {}
  ~CCommandSampleMouseCallback() {}
  UUID CommandUUID()
  {
    // {A1EE82B3-D53-462C-AAD4-61F0DFA47520}
    static const GUID SampleMouseCallbackCommand_UUID =
    { 0xA1EE82B3, 0xD53, 0x462C,{ 0xAA, 0xD4, 0x61, 0xF0, 0xDF, 0xA4, 0x75, 0x20 } };
    return SampleMouseCallbackCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMouseCallback"; }
  const wchar_t* LocalCommandName() const { return L"SampleMouseCallback"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMouseCallback object
static class CCommandSampleMouseCallback theSampleMouseCallbackCommand;

CRhinoCommand::result CCommandSampleMouseCallback::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  theSampleMouseCallback.Enable(theSampleMouseCallback.IsEnabled() ? FALSE : TRUE);
  ON_wString str;
  str.Format(L"%s is %s.\n", EnglishCommandName(), theSampleMouseCallback.IsEnabled() ? L"enabled" : L"disabled");
  RhinoApp().Print(str);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMouseCallback command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
