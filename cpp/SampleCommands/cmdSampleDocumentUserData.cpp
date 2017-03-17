#include "StdAfx.h"
#include "SampleCommandsPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDocumentUserData command
//

#pragma region SampleDocumentUserData command


// Do NOT put the definition of class CCommandSampleDocumentUserData in a header
// file.  There is only ONE instance of a CCommandSampleDocumentUserData class
// and that instance is the static theSampleDocumentUserDataCommand that appears
// immediately below the class definition.

class CCommandSampleDocumentUserData : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleDocumentUserData is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleDocumentUserData::CCommandSampleDocumentUserData()
  // is called exactly once when static theSampleDocumentUserDataCommand is created.
  CCommandSampleDocumentUserData() {}

  // CCommandSampleDocumentUserData::~CCommandSampleDocumentUserData()
  // is called exactly once when static theSampleDocumentUserDataCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleDocumentUserData() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID()
  {
    // {5B91A1BA-831F-49D3-89A5-6857EFC998DC}
    static const GUID SampleDocumentUserDataCommand_UUID =
    { 0x5B91A1BA, 0x831F, 0x49D3, { 0x89, 0xA5, 0x68, 0x57, 0xEF, 0xC9, 0x98, 0xDC } };
    return SampleDocumentUserDataCommand_UUID;
  }

  // Returns the English command name.
  const wchar_t* EnglishCommandName() { return L"SampleDocumentUserData"; }

  // Returns the localized command name.
  const wchar_t* LocalCommandName() const { return L"SampleDocumentUserData"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

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
// Do NOT create any other instance of a CCommandSampleDocumentUserData class.
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
    RhinoApp().Print(L"%s\n", plugin.GetStringTableItem(i));
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDocumentUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
