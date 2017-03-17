/////////////////////////////////////////////////////////////////////////////
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
		// {1B0ABAE6-0BB0-4FE9-B6D1-90FA60AE5A1A}
    static const GUID SampleLocalizationCommand_UUID =
    { 0x1B0ABAE6, 0xBB0, 0x4FE9, { 0xB6, 0xD1, 0x90, 0xFA, 0x60, 0xAE, 0x5A, 0x1A } };
    return SampleLocalizationCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleLocalization"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleLocalization"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleLocalization object.  
// Do NOT create any other instance of a CCommandSampleLocalization class.
static class CCommandSampleLocalization theSampleLocalizationCommand;

CRhinoCommand::result CCommandSampleLocalization::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSampleLocalization::RunCommand() is called when the user runs the "SampleLocalization"
  // command or the "SampleLocalization" command is run by a history operation.

  // TODO: Add command code here.

  // Rhino command that display a dialog box interface should also support
  // a command-line, or scriptable interface.

  ON_wString wStr;
  wStr.Format( L"The \"%s\" command is under construction.\n", EnglishCommandName() );
  if( context.IsInteractive() )
    RhinoMessageBox( wStr, PlugIn()->PlugInName(), MB_OK );
  else
	  RhinoApp().Print( wStr );

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
