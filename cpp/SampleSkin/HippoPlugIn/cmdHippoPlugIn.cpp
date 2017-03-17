/////////////////////////////////////////////////////////////////////////////
// cmdHippoPlugIn.cpp : command file
//

#include "StdAfx.h"
#include "HippoPlugInPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN HippoPlugIn command
//

#pragma region HippoPlugIn command


// Do NOT put the definition of class CCommandHippoPlugIn in a header
// file.  There is only ONE instance of a CCommandHippoPlugIn class
// and that instance is the static theHippoPlugInCommand that appears
// immediately below the class definition.

class CCommandHippoPlugIn : public CRhinoCommand
{
public:
  // The one and only instance of CCommandHippoPlugIn is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandHippoPlugIn::CCommandHippoPlugIn()
  // is called exactly once when static theHippoPlugInCommand is created.
	CCommandHippoPlugIn() {}

  // CCommandHippoPlugIn::~CCommandHippoPlugIn()
  // is called exactly once when static theHippoPlugInCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandHippoPlugIn() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {8BE05F57-E9E4-459F-AF9E-DBB16EACE1CB}
    static const GUID HippoPlugInCommand_UUID =
    { 0x8BE05F57, 0xE9E4, 0x459F, { 0xAF, 0x9E, 0xDB, 0xB1, 0x6E, 0xAC, 0xE1, 0xCB } };
    return HippoPlugInCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"HippoPlugIn"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"HippoPlugIn"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandHippoPlugIn object.  
// Do NOT create any other instance of a CCommandHippoPlugIn class.
static class CCommandHippoPlugIn theHippoPlugInCommand;

CRhinoCommand::result CCommandHippoPlugIn::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandHippoPlugIn::RunCommand() is called when the user runs the "HippoPlugIn"
  // command or the "HippoPlugIn" command is run by a history operation.

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
// END HippoPlugIn command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
