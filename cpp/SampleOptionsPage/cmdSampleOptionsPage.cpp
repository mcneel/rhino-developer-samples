/////////////////////////////////////////////////////////////////////////////
// cmdSampleOptionsPage.cpp : command file
//

#include "StdAfx.h"
#include "SampleOptionsPagePlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOptionsPage command
//

#pragma region SampleOptionsPage command


// Do NOT put the definition of class CCommandSampleOptionsPage in a header
// file.  There is only ONE instance of a CCommandSampleOptionsPage class
// and that instance is the static theSampleOptionsPageCommand that appears
// immediately below the class definition.

class CCommandSampleOptionsPage : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleOptionsPage is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleOptionsPage::CCommandSampleOptionsPage()
  // is called exactly once when static theSampleOptionsPageCommand is created.
	CCommandSampleOptionsPage() {}

  // CCommandSampleOptionsPage::~CCommandSampleOptionsPage()
  // is called exactly once when static theSampleOptionsPageCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleOptionsPage() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {0E2E463F-98DA-4B02-8D18-960DB8457400}
    static const GUID SampleOptionsPageCommand_UUID =
    { 0xE2E463F, 0x98DA, 0x4B02, { 0x8D, 0x18, 0x96, 0xD, 0xB8, 0x45, 0x74, 0x0 } };
    return SampleOptionsPageCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleOptionsPage"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleOptionsPage"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleOptionsPage object.  
// Do NOT create any other instance of a CCommandSampleOptionsPage class.
static class CCommandSampleOptionsPage theSampleOptionsPageCommand;

CRhinoCommand::result CCommandSampleOptionsPage::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSampleOptionsPage::RunCommand() is called when the user runs the "SampleOptionsPage"
  // command or the "SampleOptionsPage" command is run by a history operation.

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
// END SampleOptionsPage command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
