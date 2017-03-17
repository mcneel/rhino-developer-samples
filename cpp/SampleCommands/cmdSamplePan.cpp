#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePan command
//

#pragma region SamplePan command

class CCommandSamplePan : public CRhinoCommand
{
public:
  CCommandSamplePan() {}
  ~CCommandSamplePan() {}
  UUID CommandUUID()
  {
    // {6BEC143B-7563-4F37-B2F7-B024B06486FD}
    static const GUID SamplePanCommand_UUID =
    { 0x6BEC143B, 0x7563, 0x4F37, { 0xB2, 0xF7, 0xB0, 0x24, 0xB0, 0x64, 0x86, 0xFD } };
    return SamplePanCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePan"; }
  const wchar_t* LocalCommandName() const { return L"SamplePan"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePan object
static class CCommandSamplePan theSamplePanCommand;

CRhinoCommand::result CCommandSamplePan::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  const CRhinoAppViewSettings& view_settings = RhinoApp().AppSettings().ViewSettings();
  double d = view_settings.m_pan_increment;
  if (view_settings.m_pan_reverse_keyboard)
    d = -d;

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Select pan option");
  go.AcceptNothing();

  const int down_option_index = go.AddCommandOption(RHCMDOPTNAME(L"Down"));
  const int left_option_index = go.AddCommandOption(RHCMDOPTNAME(L"Left"));
  const int right_option_index = go.AddCommandOption(RHCMDOPTNAME(L"Right"));
  const int up_option_index = go.AddCommandOption(RHCMDOPTNAME(L"Up"));
  const int in_option_index = go.AddCommandOption(RHCMDOPTNAME(L"In"));
  const int out_option_index = go.AddCommandOption(RHCMDOPTNAME(L"Out"));

  for (;;)
  {
    CRhinoGet::result res = go.GetOption();

    if (res != CRhinoGet::option)
      break;

    CRhinoView* view = go.View();
    const CRhinoCommandOption* option = go.Option();

    if (0 != view && 0 != option && 0.0 != d)
    {
      CRhinoViewport* viewport = &(view->ActiveViewport());
      if (0 != viewport && viewport->View().m_bLockedProjection)
        viewport = &(view->MainViewport());

      if (0 != viewport)
      {
        if (down_option_index == option->m_option_index)
          viewport->DownUpDolly(-d);
        else if (up_option_index == option->m_option_index)
          viewport->DownUpDolly(d);
        else if (left_option_index == option->m_option_index)
          viewport->LeftRightDolly(-d);
        else if (right_option_index == option->m_option_index)
          viewport->LeftRightDolly(d);
        else if (in_option_index == option->m_option_index)
          viewport->InOutDolly(d);
        else if (out_option_index == option->m_option_index)
          viewport->InOutDolly(-d);
      }

      view->Redraw();
      RhinoApp().SetActiveView(view);
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePan command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
