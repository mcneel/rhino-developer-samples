// cmdSampleDockBarDialog.cpp : command file
//

#include "StdAfx.h"
#include "SampleDockBarDialogPlugIn.h"
#include "SampleTabbedDockBarDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDockBarDialog command
//

#pragma region SampleDockBarDialog command

// Do NOT put the definition of class CCommandSampleDockBarDialog in a header
// file. There is only ONE instance of a CCommandSampleDockBarDialog class
// and that instance is the static theSampleDockBarDialogCommand that appears
// immediately below the class definition.

class CCommandSampleDockBarDialog : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleDockBarDialog is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandSampleDockBarDialog::CCommandSampleDockBarDialog()
  // is called exactly once when static theSampleDockBarDialogCommand is created.
  CCommandSampleDockBarDialog() = default;

  // CCommandSampleDockBarDialog::~CCommandSampleDockBarDialog()
  // is called exactly once when static theSampleDockBarDialogCommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleDockBarDialog() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {1CB91610-1762-4C93-8582-1D50BFC43152}
    static const GUID SampleDockBarDialogCommand_UUID =
    { 0x1CB91610, 0x1762, 0x4C93, { 0x85, 0x82, 0x1D, 0x50, 0xBF, 0xC4, 0x31, 0x52 } };
    return SampleDockBarDialogCommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"SampleDockBarDialog"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDockBarDialog object
// Do NOT create any other instance of a CCommandSampleDockBarDialog class.
static class CCommandSampleDockBarDialog theSampleDockBarDialogCommand;

CRhinoCommand::result CCommandSampleDockBarDialog::RunCommand(const CRhinoCommandContext& context)
{
  ON_UUID tabId = CSampleTabbedDockBarDialog::Id();

  if (context.IsInteractive())
  {
    CRhinoTabbedDockBarDialog::OpenDockbarTab(context.m_doc, tabId);
  }
  else
  {
    bool bVisible = CRhinoTabbedDockBarDialog::IsTabVisible(context.m_doc, tabId);

    ON_wString str;
    str.Format(L"%ls is %ls. New value", LocalCommandName(), bVisible ? L"visible" : L"hidden");

    CRhinoGetOption go;
    go.SetCommandPrompt(str);
    int h_option = go.AddCommandOption(RHCMDOPTNAME(L"Hide"));
    int s_option = go.AddCommandOption(RHCMDOPTNAME(L"Show"));
    int t_option = go.AddCommandOption(RHCMDOPTNAME(L"Toggle"));
    go.GetOption();
    if (go.CommandResult() != CRhinoCommand::success)
      return go.CommandResult();

    const CRhinoCommandOption* option = go.Option();
    if (0 == option)
      return CRhinoCommand::failure;

    int option_index = option->m_option_index;

    if (h_option == option_index && bVisible)
      CRhinoTabbedDockBarDialog::ShowDockbarTab(context.m_doc, tabId, false, false, nullptr);
    else if (s_option == option_index && !bVisible)
      CRhinoTabbedDockBarDialog::ShowDockbarTab(context.m_doc, tabId, true, true, nullptr);
    else if (t_option == option_index)
      CRhinoTabbedDockBarDialog::ShowDockbarTab(context.m_doc, tabId, !bVisible, !bVisible, nullptr);
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDockBarDialog command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
