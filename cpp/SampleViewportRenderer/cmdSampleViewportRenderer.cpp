/////////////////////////////////////////////////////////////////////////////
// cmdSampleViewportRenderer.cpp : command file
//

#include "StdAfx.h"
#include "SampleViewportRendererPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleViewportRenderer command
//

#pragma region SampleViewportRenderer command


// Do NOT put the definition of class CCommandSampleViewportRenderer in a header
// file.  There is only ONE instance of a CCommandSampleViewportRenderer class
// and that instance is the static theSampleViewportRendererCommand that appears
// immediately below the class definition.

class CCommandSampleViewportRenderer : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleViewportRenderer is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleViewportRenderer::CCommandSampleViewportRenderer()
  // is called exactly once when static theSampleViewportRendererCommand is created.
	CCommandSampleViewportRenderer() {}

  // CCommandSampleViewportRenderer::~CCommandSampleViewportRenderer()
  // is called exactly once when static theSampleViewportRendererCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleViewportRenderer() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {1F7F7A63-F303-4369-A3EB-B8D39AD9FAE1}
    static const GUID SampleViewportRendererCommand_UUID =
    { 0x1F7F7A63, 0xF303, 0x4369, { 0xA3, 0xEB, 0xB8, 0xD3, 0x9A, 0xD9, 0xFA, 0xE1 } };
    return SampleViewportRendererCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleViewportRenderer"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleViewportRenderer"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleViewportRenderer object.  
// Do NOT create any other instance of a CCommandSampleViewportRenderer class.
static class CCommandSampleViewportRenderer theSampleViewportRendererCommand;

CRhinoCommand::result CCommandSampleViewportRenderer::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSampleViewportRenderer::RunCommand() is called when the user runs the "SampleViewportRenderer"
  // command or the "SampleViewportRenderer" command is run by a history operation.

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
// END SampleViewportRenderer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
