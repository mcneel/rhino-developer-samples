#include "stdafx.h"
#include "SampleUserInterfacePlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMenuToggle command
//

#pragma region SampleMenuToggle command

class CCommandSampleMenuToggle : public CRhinoCommand
{
public:
  CCommandSampleMenuToggle() {}
  ~CCommandSampleMenuToggle() {}
  UUID CommandUUID()
  {
    // {F065C992-5B0E-484A-BA61-098E5A414974}
    static const GUID SampleMenuToggleCommand_UUID =
    { 0xF065C992, 0x5B0E, 0x484A, { 0xBA, 0x61, 0x09, 0x8E, 0x5A, 0x41, 0x49, 0x74 } };
    return SampleMenuToggleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMenuToggle"; }
  const wchar_t* LocalCommandName() const { return L"SampleMenuToggle"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMenuToggle object
static class CCommandSampleMenuToggle theSampleMenuToggleCommand;

CRhinoCommand::result CCommandSampleMenuToggle::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  BOOL bVisible = SampleUserInterfacePlugIn().IsSampleMenuVisible();

  ON_wString str;
  str.Format(L"%s is %s. New value", LocalCommandName(), bVisible ? L"visible" : L"hidden");

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
    SampleUserInterfacePlugIn().HideSampleMenu();
  else if (s_option == option_index && !bVisible)
    SampleUserInterfacePlugIn().ShowSampleMenu();
  else if (t_option == option_index)
  {
    if (bVisible)
      SampleUserInterfacePlugIn().HideSampleMenu();
    else
      SampleUserInterfacePlugIn().ShowSampleMenu();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMenuToggle command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
