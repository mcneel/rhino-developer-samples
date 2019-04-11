#include "StdAfx.h"
#include "SampleCommandsPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDocumentUserData command
//

#pragma region SampleDocumentUserData command

class CCommandSampleDocumentUserData : public CRhinoCommand
{
public:
  CCommandSampleDocumentUserData() = default;
  ~CCommandSampleDocumentUserData() = default;
  UUID CommandUUID() override
  {
    // {5B91A1BA-831F-49D3-89A5-6857EFC998DC}
    static const GUID SampleDocumentUserDataCommand_UUID =
    { 0x5B91A1BA, 0x831F, 0x49D3, { 0x89, 0xA5, 0x68, 0x57, 0xEF, 0xC9, 0x98, 0xDC } };
    return SampleDocumentUserDataCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDocumentUserData"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;

  enum CommandOptions
  {
    add_option = 1,
    delete_option,
    list_option,
    option_count
  };

private:
  CRhinoCommand::result CommandOption(const CRhinoCommandOption* option);
  CRhinoCommand::result AddOption();
  CRhinoCommand::result DeleteOption();
  CRhinoCommand::result ListOption();
};

// The one and only CCommandSampleDocumentUserData object.  
static class CCommandSampleDocumentUserData theSampleDocumentUserDataCommand;

CRhinoCommand::result CCommandSampleDocumentUserData::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Choose an option");
  go.AddCommandOption(RHCMDOPTNAME(L"Add"));
  go.AddCommandOption(RHCMDOPTNAME(L"Delete"));
  go.AddCommandOption(RHCMDOPTNAME(L"List"));
  go.AcceptNothing();

  for (;; )
  {
    CRhinoGet::result res = go.GetOption();
    switch (res)
    {
    case CRhinoGet::option:
      if (CommandOption(go.Option()) == CRhinoCommand::cancel)
        return CRhinoCommand::cancel;
      break;
    case CRhinoGet::nothing:
      return CRhinoCommand::success;
    default:
      return CRhinoCommand::cancel;
    }
  }

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleDocumentUserData::CommandOption(const CRhinoCommandOption* option)
{
  if (0 == option)
    return CRhinoCommand::cancel;

  switch (option->m_option_index)
  {
  case add_option:
    return AddOption();
  case delete_option:
    return DeleteOption();
  case list_option:
    return ListOption();
  }

  return CRhinoCommand::cancel;
}

CRhinoCommand::result CCommandSampleDocumentUserData::AddOption()
{
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"String to add");
  gs.AcceptNothing();
  switch (gs.GetString())
  {
  case CRhinoGet::string:
    break;
  case CRhinoGet::nothing:
    return CRhinoCommand::nothing;
  default:
    return CRhinoCommand::cancel;
  }

  ON_wString str(gs.String());
  str.TrimLeft();
  str.TrimRight();
  if (str.IsEmpty())
    return CRhinoCommand::nothing;

  CSampleCommandsPlugIn& plugin = SampleCommandsPlugIn();
  int index = plugin.AddStringTableItem(str);
  if (index < 0)
    RhinoApp().Print(L"Unable to add string.\n");

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleDocumentUserData::DeleteOption()
{
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"String to remove");
  gs.AcceptNothing();
  switch (gs.GetString())
  {
  case CRhinoGet::string:
    break;
  case CRhinoGet::nothing:
    return CRhinoCommand::nothing;
  default:
    return CRhinoCommand::cancel;
  }

  ON_wString str(gs.String());
  str.TrimLeft();
  str.TrimRight();
  if (str.IsEmpty())
    return CRhinoCommand::nothing;

  CSampleCommandsPlugIn& plugin = SampleCommandsPlugIn();
  bool rc = plugin.DeleteStringTableItem(str);
  if (!rc)
    RhinoApp().Print(L"Unable to delete string.\n");

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleDocumentUserData::ListOption()
{
  CSampleCommandsPlugIn& plugin = SampleCommandsPlugIn();
  for (int i = 0; i < plugin.StringTableCount(); i++)
    RhinoApp().Print(L"%ls\n", static_cast<const wchar_t*>(plugin.GetStringTableItem(i)));
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDocumentUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
