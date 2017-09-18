/////////////////////////////////////////////////////////////////////////////
// cmdSamplePropertiesPages.cpp : command file
//

#include "StdAfx.h"
#include "SamplePropertiesPagesPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePropertiesPages command
//

#pragma region SamplePropertiesPages command


// Do NOT put the definition of class CCommandSamplePropertiesPages in a header
// file.  There is only ONE instance of a CCommandSamplePropertiesPages class
// and that instance is the static theSamplePropertiesPagesCommand that appears
// immediately below the class definition.

class CCommandSamplePropertiesPages : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSamplePropertiesPages is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSamplePropertiesPages::CCommandSamplePropertiesPages()
  // is called exactly once when static theSamplePropertiesPagesCommand is created.
	CCommandSamplePropertiesPages() {}

  // CCommandSamplePropertiesPages::~CCommandSamplePropertiesPages()
  // is called exactly once when static theSamplePropertiesPagesCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSamplePropertiesPages() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {378678F4-058C-40D3-93DB-EB8C0F519CA5}
    static const GUID SamplePropertiesPagesCommand_UUID =
    { 0x378678F4, 0x58C, 0x40D3, { 0x93, 0xDB, 0xEB, 0x8C, 0xF, 0x51, 0x9C, 0xA5 } };
    return SamplePropertiesPagesCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SamplePropertiesPages"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SamplePropertiesPages"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSamplePropertiesPages object.  
// Do NOT create any other instance of a CCommandSamplePropertiesPages class.
static class CCommandSamplePropertiesPages theSamplePropertiesPagesCommand;

CRhinoCommand::result CCommandSamplePropertiesPages::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSamplePropertiesPages::RunCommand() is called when the user runs the "SamplePropertiesPages"
  // command or the "SamplePropertiesPages" command is run by a history operation.

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
// END SamplePropertiesPages command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
