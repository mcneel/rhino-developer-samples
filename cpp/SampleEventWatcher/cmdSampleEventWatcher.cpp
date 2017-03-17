#include "StdAfx.h"
#include "SampleEventWatcherPlugIn.h"
#include "SampleRhinoEventWatcher.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleEventWatcher command
//

#pragma region SampleEventWatcher command

class CCommandSampleEventWatcher : public CRhinoCommand
{
public:
  CCommandSampleEventWatcher() { m_watcher.Register(); }
  ~CCommandSampleEventWatcher() {}
  UUID CommandUUID()
  {
    // {38BAFDB7-9128-45CD-AF12-5CA15B50C399}
    static const GUID SampleEventWatcherCommand_UUID =
    { 0x38BAFDB7, 0x9128, 0x45CD, { 0xAF, 0x12, 0x5C, 0xA1, 0x5B, 0x50, 0xC3, 0x99 } };
    return SampleEventWatcherCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleEventWatcher"; }
  const wchar_t* LocalCommandName() const { return L"SampleEventWatcher"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CSampleRhinoEventWatcher m_watcher;
};

// The one and only CCommandSampleEventWatcher object.  
// Do NOT create any other instance of a CCommandSampleEventWatcher class.
static class CCommandSampleEventWatcher theSampleEventWatcherCommand;

CRhinoCommand::result CCommandSampleEventWatcher::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  bool bEnabled = m_watcher.IsEnabled();

  ON_wString prompt;
  if (bEnabled)
    prompt = L"Event watcher is enabled. New value";
  else
    prompt = L"Event watcher is disabled. New value";

  CRhinoGetOption go;
  go.SetCommandPrompt(prompt);
  int d_option = go.AddCommandOption(RHCMDOPTNAME(L"Disable"));
  int e_option = go.AddCommandOption(RHCMDOPTNAME(L"Enable"));
  int t_option = go.AddCommandOption(RHCMDOPTNAME(L"Toggle"));
  go.GetOption();
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoCommandOption* opt = go.Option();
  if (0 == opt)
    return CRhinoCommand::failure;

  int option_index = opt->m_option_index;
  if (d_option == option_index)
  {
    if (bEnabled)
      m_watcher.Enable(FALSE);
  }
  else if (e_option == option_index)
  {
    if (!bEnabled)
      m_watcher.Enable(TRUE);
  }
  else if (t_option == option_index)
  {
    m_watcher.Enable(bEnabled ? false : true);
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleEventWatcher command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
