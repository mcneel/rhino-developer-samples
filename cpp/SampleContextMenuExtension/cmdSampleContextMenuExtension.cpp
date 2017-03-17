/////////////////////////////////////////////////////////////////////////////
// cmdSampleContextMenuExtension.cpp : command file
//

#include "StdAfx.h"
#include "SampleContextMenuExtensionPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleContextMenuExtension command
//

#pragma region SampleContextMenuExtension command


// Do NOT put the definition of class CCommandSampleContextMenuExtension in a header
// file.  There is only ONE instance of a CCommandSampleContextMenuExtension class
// and that instance is the static theSampleContextMenuExtensionCommand that appears
// immediately below the class definition.

class CCommandSampleContextMenuExtension : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleContextMenuExtension is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleContextMenuExtension::CCommandSampleContextMenuExtension()
  // is called exactly once when static theSampleContextMenuExtensionCommand is created.
	CCommandSampleContextMenuExtension() {}

  // CCommandSampleContextMenuExtension::~CCommandSampleContextMenuExtension()
  // is called exactly once when static theSampleContextMenuExtensionCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleContextMenuExtension() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {5B19192B-1001-46AD-AF18-B47DB8B80EA1}
    static const GUID SampleContextMenuExtensionCommand_UUID =
    { 0x5B19192B, 0x1001, 0x46AD, { 0xAF, 0x18, 0xB4, 0x7D, 0xB8, 0xB8, 0xE, 0xA1 } };
    return SampleContextMenuExtensionCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleContextMenuExtension"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleContextMenuExtension"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleContextMenuExtension object.  
// Do NOT create any other instance of a CCommandSampleContextMenuExtension class.
static class CCommandSampleContextMenuExtension theSampleContextMenuExtensionCommand;

CRhinoCommand::result CCommandSampleContextMenuExtension::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSampleContextMenuExtension::RunCommand() is called when the user runs the "SampleContextMenuExtension"
  // command or the "SampleContextMenuExtension" command is run by a history operation.

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
// END SampleContextMenuExtension command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
