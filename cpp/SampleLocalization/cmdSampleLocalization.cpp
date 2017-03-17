// cmdSampleLocalization.cpp : command file
//

#include "StdAfx.h"
#include "SampleLocalizationPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLocalization command
//

#pragma region SampleLocalization command

// Do NOT put the definition of class CCommandSampleLocalization in a header
// file.  There is only ONE instance of a CCommandSampleLocalization class
// and that instance is the static theSampleLocalizationCommand that appears
// immediately below the class definition.

class CCommandSampleLocalization : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleLocalization is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleLocalization::CCommandSampleLocalization()
  // is called exactly once when static theSampleLocalizationCommand is created.
	CCommandSampleLocalization() {}

  // CCommandSampleLocalization::~CCommandSampleLocalization()
  // is called exactly once when static theSampleLocalizationCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleLocalization() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {9163EC4E-CABA-43FC-9554-B569173B0348}
    static const GUID SampleLocalizationCommand_UUID =
    { 0x9163EC4E, 0xCABA, 0x43FC, { 0x95, 0x54, 0xB5, 0x69, 0x17, 0x3B, 0x3, 0x48 } };
    return SampleLocalizationCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleLocalization"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleLocalization"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleLocalization object
// Do NOT create any other instance of a CCommandSampleLocalization class.
static class CCommandSampleLocalization theSampleLocalizationCommand;

CRhinoCommand::result CCommandSampleLocalization::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSampleLocalization::RunCommand() is called when the user
  // runs the "SampleLocalization".

  // TODO: Add command code here.

  // Rhino command that display a dialog box interface should also support
  // a command-line, or scriptable interface.

  ON_wString str;
  str.Format( L"The \"%s\" command is under construction.\n", EnglishCommandName() );
  if( context.IsInteractive() )
    RhinoMessageBox( str, SampleLocalizationPlugIn().PlugInName(), MB_OK );
  else
	  RhinoApp().Print( str );

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
// END SampleLocalization command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
