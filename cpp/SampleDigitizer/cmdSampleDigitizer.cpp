/////////////////////////////////////////////////////////////////////////////
// cmdSampleDigitizer.cpp : command file
//

#include "StdAfx.h"
#include "SampleDigitizerPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDigitizer command
//

#pragma region SampleDigitizer command


// Do NOT put the definition of class CCommandSampleDigitizer in a header
// file.  There is only ONE instance of a CCommandSampleDigitizer class
// and that instance is the static theSampleDigitizerCommand that appears
// immediately below the class definition.

class CCommandSampleDigitizer : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleDigitizer is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleDigitizer::CCommandSampleDigitizer()
  // is called exactly once when static theSampleDigitizerCommand is created.
	CCommandSampleDigitizer() {}

  // CCommandSampleDigitizer::~CCommandSampleDigitizer()
  // is called exactly once when static theSampleDigitizerCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleDigitizer() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {0179E856-547B-4BDC-94BD-7637DF87B126}
    static const GUID SampleDigitizerCommand_UUID =
    { 0x179E856, 0x547B, 0x4BDC, { 0x94, 0xBD, 0x76, 0x37, 0xDF, 0x87, 0xB1, 0x26 } };
    return SampleDigitizerCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleDigitizer"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleDigitizer"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDigitizer object.  
// Do NOT create any other instance of a CCommandSampleDigitizer class.
static class CCommandSampleDigitizer theSampleDigitizerCommand;

CRhinoCommand::result CCommandSampleDigitizer::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSampleDigitizer::RunCommand() is called when the user runs the "SampleDigitizer"
  // command or the "SampleDigitizer" command is run by a history operation.

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
// END SampleDigitizer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
