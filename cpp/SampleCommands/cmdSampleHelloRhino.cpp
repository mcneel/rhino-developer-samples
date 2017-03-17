#include "StdAfx.h"
#include "SampleCommandsPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHelloRhino command
//

#pragma region SampleHelloRhino command

// Do NOT put the definition of class CCommandSampleHelloRhino in a header
// file.  There is only ONE instance of a CCommandSampleHelloRhino class
// and that instance is the static theSampleHelloRhinoCommand that appears
// immediately below the class definition.

class CCommandSampleHelloRhino : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleHelloRhino is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleHelloRhino::CCommandSampleHelloRhino()
  // is called exactly once when static theSampleHelloRhinoCommand is created.
  CCommandSampleHelloRhino() {}

  // CCommandSampleHelloRhino::~CCommandSampleHelloRhino()
  // is called exactly once when static theSampleHelloRhinoCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleHelloRhino() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID()
  {
    // {41499A46-73BC-4AC9-9F29-D7DBAED0FCDE}
    static const GUID SampleHelloRhinoCommand_UUID =
    { 0x41499A46, 0x73BC, 0x4AC9, { 0x9F, 0x29, 0xD7, 0xDB, 0xAE, 0xD0, 0xFC, 0xDE } };
    return SampleHelloRhinoCommand_UUID;
  }

  // Returns the English command name.
  const wchar_t* EnglishCommandName() { return L"SampleHelloRhino"; }

  // Returns the localized command name.
  const wchar_t* LocalCommandName() const { return L"SampleHelloRhino"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleHelloRhino object.  
// Do NOT create any other instance of a CCommandSampleHelloRhino class.
static class CCommandSampleHelloRhino theSampleHelloRhinoCommand;

CRhinoCommand::result CCommandSampleHelloRhino::RunCommand(const CRhinoCommandContext& context)
{
  // CCommandSampleHelloRhino::RunCommand() is called when the user runs the "SampleHelloRhino"
  // command or the "SampleHelloRhino" command is run by a history operation.

  // TODO: Add command code here.

  // Rhino command that display a dialog box interface should also support
  // a command-line, or scriptable interface.

  ON_wString str = L"Hello Rhino!";
  if (context.IsInteractive())
    RhinoMessageBox(str, SampleCommandsPlugIn().PlugInName(), MB_OK);
  else
    RhinoApp().Print(L"%s\n", str);

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
// END SampleHelloRhino command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
