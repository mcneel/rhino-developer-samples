#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMouseCallback command
//

class CSampleMouseCallback : public CRhinoMouseCallback
{
public:
  CSampleMouseCallback() = default;
  ~CSampleMouseCallback() = default;

  void OnMouseDown(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted) override;
  void OnMouseMove(unsigned int viewSerialNumber, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted) override;
  void OnMouseUp(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted) override;
  virtual BOOL OnMouseDblClk(CRhinoView& view, mouse_button button, UINT flags, const ON_2iPoint& point) override;
};

// The one and only CSampleMouseCallback object
static class CSampleMouseCallback theSampleMouseCallback;

void CSampleMouseCallback::OnMouseDown(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted)
{
  TRACE0("CSampleMouseCallback::OnMouseDown\n");
}

void CSampleMouseCallback::OnMouseMove(unsigned int viewSerialNumber, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted)
{
  TRACE0("CSampleMouseCallback::OnMouseMove\n");
}

void CSampleMouseCallback::OnMouseUp(unsigned int viewSerialNumber, mouse_button button, UINT flags, const ON_2iPoint& point, bool defaultHandlerExecuted)
{
  TRACE0("CSampleMouseCallback::OnMouseUp\n");
}

BOOL CSampleMouseCallback::OnMouseDblClk(CRhinoView& view , mouse_button button, UINT flags, const ON_2iPoint& point)
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
  CCommandSampleMouseCallback() = default;
  ~CCommandSampleMouseCallback() = default;
  UUID CommandUUID() override
  {
    // {A1EE82B3-D53-462C-AAD4-61F0DFA47520}
    static const GUID SampleMouseCallbackCommand_UUID =
    { 0xA1EE82B3, 0xD53, 0x462C,{ 0xAA, 0xD4, 0x61, 0xF0, 0xDF, 0xA4, 0x75, 0x20 } };
    return SampleMouseCallbackCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMouseCallback"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleMouseCallback object
static class CCommandSampleMouseCallback theSampleMouseCallbackCommand;

CRhinoCommand::result CCommandSampleMouseCallback::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  theSampleMouseCallback.Enable(theSampleMouseCallback.IsEnabled() ? FALSE : TRUE);
  ON_wString str;
  str.Format(L"%ls is %ls.\n", EnglishCommandName(), theSampleMouseCallback.IsEnabled() ? L"enabled" : L"disabled");
  RhinoApp().Print(str);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMouseCallback command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
