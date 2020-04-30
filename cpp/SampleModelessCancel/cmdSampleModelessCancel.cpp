// cmdSampleModelessCancel.cpp : command file
//

#include "StdAfx.h"
#include "SampleModelessCancelPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleModelessCancel command
//

#pragma region SampleModelessCancel command

// Do NOT put the definition of class CCommandSampleModelessCancel in a header
// file. There is only ONE instance of a CCommandSampleModelessCancel class
// and that instance is the static theSampleModelessCancelCommand that appears
// immediately below the class definition.

class CCommandSampleModelessCancel : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleModelessCancel is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandSampleModelessCancel::CCommandSampleModelessCancel()
  // is called exactly once when static theSampleModelessCancelCommand is created.
  CCommandSampleModelessCancel() = default;

  // CCommandSampleModelessCancel::~CCommandSampleModelessCancel()
  // is called exactly once when static theSampleModelessCancelCommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleModelessCancel() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {587F38B5-3889-4BCD-B4C5-35A8E6A82241}
    static const GUID SampleModelessCancelCommand_UUID =
    { 0x587F38B5, 0x3889, 0x4BCD, { 0xB4, 0xC5, 0x35, 0xA8, 0xE6, 0xA8, 0x22, 0x41 } };
    return SampleModelessCancelCommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"SampleModelessCancel"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleModelessCancel object
// Do NOT create any other instance of a CCommandSampleModelessCancel class.
static class CCommandSampleModelessCancel theSampleModelessCancelCommand;

CRhinoCommand::result CCommandSampleModelessCancel::RunCommand(const CRhinoCommandContext& context)
{
  // CCommandSampleModelessCancel::RunCommand() is called when the user
  // runs the "SampleModelessCancel".

  // TODO: Add command code here.

  // Rhino command that display a dialog box interface should also support
  // a command-line, or scriptable interface.

  ON_wString str;
  str.Format(L"The \"%s\" command is under construction.\n", EnglishCommandName());
  const wchar_t* pszStr = static_cast<const wchar_t*>(str);
  if (context.IsInteractive())
    RhinoMessageBox(pszStr, SampleModelessCancelPlugIn().PlugInName(), MB_OK);
  else
    RhinoApp().Print(pszStr);

  // TODO: Return one of the following values:
  //   CRhinoCommand::success:  The command worked.
  //   CRhinoCommand::failure:  The command failed because of invalid input, inability
  //                            to compute the desired result, or some other reason
  //                            computation reason.
  //   CRhinoCommand::cancel:   The user interactively canceled the command 
  //                            (by pressing ESCAPE, clicking a CANCEL button, etc.)
  //                            in a Get operation, dialog, time consuming computation, etc.

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleModelessCancel command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
