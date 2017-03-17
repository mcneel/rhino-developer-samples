#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRotateView command
//

#pragma region SampleRotateView command

class CCommandSampleRotateView : public CRhinoCommand
{
public:
  CCommandSampleRotateView() { m_direction = 2; m_angle = 1.0; }
  ~CCommandSampleRotateView() {}
  UUID CommandUUID()
  {
    // {272BB8E3-E88F-49D3-A603-7EC78CAFAC43}
    static const GUID SampleRotateViewCommand_UUID =
    { 0x272BB8E3, 0xE88F, 0x49D3, { 0xA6, 0x03, 0x7E, 0xC7, 0x8C, 0xAF, 0xAC, 0x43 } };
    return SampleRotateViewCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleRotateView"; }
  const wchar_t* LocalCommandName() const { return L"SampleRotateView"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  int m_direction;
  double m_angle;
};

// The one and only CCommandSampleRotateView object
static class CCommandSampleRotateView theSampleRotateViewCommand;

CRhinoCommand::result CCommandSampleRotateView::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoView* rhino_view = RhinoApp().ActiveView();
  if (0 == rhino_view)
    return CRhinoCommand::failure;

  const CRhinoAppViewSettings& settings = RhinoApp().AppSettings().ViewSettings();
  double fixed_angle_radians = 2.0 * ON_PI / settings.m_rotate_circle_increment;
  if (settings.m_rotate_reverse_keyboard)
    fixed_angle_radians = -fixed_angle_radians;

  CRhinoCommandOptionValue list[4] =
  {
    RHCMDOPTVALUE(L"Down"),
    RHCMDOPTVALUE(L"Left"),
    RHCMDOPTVALUE(L"Right"),
    RHCMDOPTVALUE(L"Up"),
  };

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Rotation angle");
  gn.AcceptNothing();
  for (;;)
  {
    gn.ClearCommandOptions();
    int d_opt = gn.AddCommandOptionList(RHCMDOPTNAME(L"Direction"), _countof(list), list, m_direction);
    gn.AddCommandOption(RHCMDOPTNAME(L"Step"));

    CRhinoGet::result res = gn.GetNumber();
    if (res == CRhinoGet::number)
    {
      m_angle = gn.Number();

      double angle_radians = fabs(m_angle) * (ON_PI / 180.0);
      if (settings.m_rotate_reverse_keyboard)
        angle_radians = -angle_radians;

      if (m_direction == 0)
        rhino_view->ActiveViewport().DownUpRotate(-angle_radians);
      else if (m_direction == 1)
        rhino_view->ActiveViewport().LeftRightRotate(-angle_radians);
      else if (m_direction == 2)
        rhino_view->ActiveViewport().LeftRightRotate(angle_radians);
      else //(m_direction == 3)
        rhino_view->ActiveViewport().DownUpRotate(angle_radians);
      rhino_view->Redraw();
      continue;
    }
    else if (res == CRhinoGet::option)
    {
      if (d_opt == gn.OptionIndex() && gn.Option())
        m_direction = gn.Option()->m_list_option_current;
      else
      {
        if (m_direction == 0)
          rhino_view->ActiveViewport().DownUpRotate(-fixed_angle_radians);
        else if (m_direction == 1)
          rhino_view->ActiveViewport().LeftRightRotate(-fixed_angle_radians);
        else if (m_direction == 2)
          rhino_view->ActiveViewport().LeftRightRotate(fixed_angle_radians);
        else //(m_direction == 3)
          rhino_view->ActiveViewport().DownUpRotate(fixed_angle_radians);
        rhino_view->Redraw();
      }
      continue;
    }

    break;
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRotateView command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
