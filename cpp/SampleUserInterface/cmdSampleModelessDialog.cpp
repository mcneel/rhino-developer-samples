#include "StdAfx.h"
#include "SampleUserInterfacePlugIn.h"
#include "SampleModelessDialog.h"
#include "Resource.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleModelessDialog command
//

class CCommandSampleModelessDialog : public CRhinoCommand
{
public:
  CCommandSampleModelessDialog() {}
  ~CCommandSampleModelessDialog() {}
  UUID CommandUUID()
  {
    // {5110DE79-8458-4AFE-9EE2-895A77EDF05C}
    static const GUID SampleModelessDialogCommand_UUID =
    { 0x5110DE79, 0x8458, 0x4AFE, { 0x9E, 0xE2, 0x89, 0x5A, 0x77, 0xED, 0xF0, 0x5C } };
    return SampleModelessDialogCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleModelessDialog"; }
  const wchar_t* LocalCommandName() { return L"SampleModelessDialog"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleModelessDialog object
static class CCommandSampleModelessDialog theSampleModelessDialogCommand;

CRhinoCommand::result CCommandSampleModelessDialog::RunCommand(const CRhinoCommandContext& context)
{
  CSampleUserInterfacePlugIn& plugin = SampleUserInterfacePlugIn();

  bool bVisible = plugin.IsDlgVisible();

  if (context.IsInteractive())
  {
    if (false == bVisible)
      plugin.DisplayDlg();
  }
  else
  {
    ON_wString prompt;
    prompt.Format(L"%s window is %s. New value", EnglishCommandName(), bVisible ? L"visible" : L"hidden");

    CRhinoGetOption go;
    go.SetCommandPrompt(prompt);
    int show_opt = go.AddCommandOption(RHCMDOPTNAME(L"Show"));
    int hide_opt = go.AddCommandOption(RHCMDOPTNAME(L"Hide"));
    int toggle_opt = go.AddCommandOption(RHCMDOPTNAME(L"Toggle"));
    go.GetOption();
    if (go.CommandResult() != CRhinoCommand::success)
      return go.CommandResult();

    const CRhinoCommandOption* option = go.Option();
    if (!option)
      return CRhinoCommand::failure;

    if (option->m_option_index == show_opt)
    {
      if (false == bVisible)
        plugin.DisplayDlg();
    }
    else if (option->m_option_index == hide_opt)
    {
      if (true == bVisible)
        plugin.DestroyDlg();
    }
    else if (option->m_option_index == toggle_opt)
    {
      if (true == bVisible)
        plugin.DestroyDlg();
      else
        plugin.DisplayDlg();
    }
    else
      return CRhinoCommand::failure;
  }

  return CRhinoCommand::success;
}

//
// END SampleModelessDialog command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static CRhinoCommand::result GetDialogPoint(const CRhinoCommandContext&, ON_3dPoint& pt)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point");
  gp.GetPoint();
  if (gp.CommandResult() == CRhinoCommand::success)
    pt = gp.Point();
  return gp.CommandResult();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN GetPoint1 command
//

class CCommandGetPoint1 : public CRhinoTestCommand
{
public:
  CCommandGetPoint1() {}
  ~CCommandGetPoint1() {}
  UUID CommandUUID()
  {
    // {A3E576E2-7A1F-42F9-8EFE-D0F92B17BAD8}
    static const GUID GetPoint1Command_UUID =
    { 0xA3E576E2, 0x7A1F, 0x42F9, { 0x8E, 0xFE, 0xD0, 0xF9, 0x2B, 0x17, 0xBA, 0xD8 } };
    return GetPoint1Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"GetPoint1"; }
  const wchar_t* LocalCommandName() { return L"GetPoint1"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandGetPoint1 object
static class CCommandGetPoint1 theGetPoint1Command;

CRhinoCommand::result CCommandGetPoint1::RunCommand(const CRhinoCommandContext& context)
{
  CSampleUserInterfacePlugIn& plugin = SampleUserInterfacePlugIn();
  if (!plugin.IsDlgVisible())
    return CRhinoCommand::nothing;

  ON_3dPoint pt;
  CRhinoCommand::result rc = GetDialogPoint(context, pt);
  if (rc == CRhinoCommand::success)
    plugin.SetDlgPointValue(1, pt);

  return rc;
}

//
// END GetPoint1 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN GetPoint2 command
//

class CCommandGetPoint2 : public CRhinoTestCommand
{
public:
  CCommandGetPoint2() {}
  ~CCommandGetPoint2() {}
  UUID CommandUUID()
  {
    // {EB82C70C-1A56-41F5-AD76-A2BCEA6770E8}
    static const GUID GetPoint2Command_UUID =
    { 0xEB82C70C, 0x1A56, 0x41F5, { 0xAD, 0x76, 0xA2, 0xBC, 0xEA, 0x67, 0x70, 0xE8 } };
    return GetPoint2Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"GetPoint2"; }
  const wchar_t* LocalCommandName() { return L"GetPoint2"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandGetPoint2 object
static class CCommandGetPoint2 theGetPoint2Command;

CRhinoCommand::result CCommandGetPoint2::RunCommand(const CRhinoCommandContext& context)
{
  CSampleUserInterfacePlugIn& plugin = SampleUserInterfacePlugIn();
  if (!plugin.IsDlgVisible())
    return CRhinoCommand::nothing;

  ON_3dPoint pt;
  CRhinoCommand::result rc = GetDialogPoint(context, pt);
  if (rc == CRhinoCommand::success)
    plugin.SetDlgPointValue(2, pt);

  return rc;
}

//
// END GetPoint2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN GetPoint3 command
//

class CCommandGetPoint3 : public CRhinoTestCommand
{
public:
  CCommandGetPoint3() {}
  ~CCommandGetPoint3() {}
  UUID CommandUUID()
  {
    // {A76695F4-764F-4A0F-81A9-4D126DCD67FF}
    static const GUID GetPoint3Command_UUID =
    { 0xA76695F4, 0x764F, 0x4A0F, { 0x81, 0xA9, 0x4D, 0x12, 0x6D, 0xCD, 0x67, 0xFF } };
    return GetPoint3Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"GetPoint3"; }
  const wchar_t* LocalCommandName() { return L"GetPoint3"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandGetPoint3 object
static class CCommandGetPoint3 theGetPoint3Command;

CRhinoCommand::result CCommandGetPoint3::RunCommand(const CRhinoCommandContext& context)
{
  CSampleUserInterfacePlugIn& plugin = SampleUserInterfacePlugIn();
  if (!plugin.IsDlgVisible())
    return CRhinoCommand::nothing;

  ON_3dPoint pt;
  CRhinoCommand::result rc = GetDialogPoint(context, pt);
  if (rc == CRhinoCommand::success)
    plugin.SetDlgPointValue(3, pt);

  return rc;
}

//
// END GetPoint3 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
