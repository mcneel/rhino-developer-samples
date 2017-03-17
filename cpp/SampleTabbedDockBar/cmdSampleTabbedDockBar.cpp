/////////////////////////////////////////////////////////////////////////////
// cmdSampleTabbedDockBar.cpp : command file
//

#include "StdAfx.h"
#include "SampleTabbedDockBarPlugIn.h"
#include "SampleTabbedDockBarDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTabbedDockBar command
//

#pragma region SampleTabbedDockBar command


// Do NOT put the definition of class CCommandSampleTabbedDockBar in a header
// file.  There is only ONE instance of a CCommandSampleTabbedDockBar class
// and that instance is the static theSampleTabbedDockBarCommand that appears
// immediately below the class definition.

class CCommandSampleTabbedDockBar : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleTabbedDockBar is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleTabbedDockBar::CCommandSampleTabbedDockBar()
  // is called exactly once when static theSampleTabbedDockBarCommand is created.
	CCommandSampleTabbedDockBar() {}

  // CCommandSampleTabbedDockBar::~CCommandSampleTabbedDockBar()
  // is called exactly once when static theSampleTabbedDockBarCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleTabbedDockBar() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {241B6E5D-ADEA-4FF0-90F6-2CF14B49BDF4}
    static const GUID SampleTabbedDockBarCommand_UUID =
    { 0x241B6E5D, 0xADEA, 0x4FF0, { 0x90, 0xF6, 0x2C, 0xF1, 0x4B, 0x49, 0xBD, 0xF4 } };
    return SampleTabbedDockBarCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleTabbedDockBar"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleTabbedDockBar"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleTabbedDockBar object.  
// Do NOT create any other instance of a CCommandSampleTabbedDockBar class.
static class CCommandSampleTabbedDockBar theSampleTabbedDockBarCommand;

CRhinoCommand::result CCommandSampleTabbedDockBar::RunCommand( const CRhinoCommandContext& context )
{
  ON_UUID tabId = CSampleTabbedDockBarDialog::ID();

  if( context.IsInteractive() )
  {
    CRhinoTabbedDockBarDialog::OpenDockbarTab( tabId );
  }
  else
  {
    bool bVisible = CRhinoTabbedDockBarDialog::IsTabVisible( tabId );
 
    ON_wString str;
    str.Format( L"%s is %s. New value", LocalCommandName(), bVisible ? L"visible" : L"hidden" );
 
    CRhinoGetOption go;
    go.SetCommandPrompt( str );
    int h_option = go.AddCommandOption( RHCMDOPTNAME(L"Hide") );
    int s_option = go.AddCommandOption( RHCMDOPTNAME(L"Show") );
    int t_option = go.AddCommandOption( RHCMDOPTNAME(L"Toggle") );
    go.GetOption();
    if( go.CommandResult() != CRhinoCommand::success )
      return go.CommandResult();
 
    const CRhinoCommandOption* option = go.Option();
    if( 0 == option )
      return CRhinoCommand::failure;
 
    int option_index = option->m_option_index;

    if( h_option == option_index && bVisible )
      CRhinoTabbedDockBarDialog::ShowDockbarTab( tabId, false );
    else if( s_option == option_index && !bVisible  )
      CRhinoTabbedDockBarDialog::ShowDockbarTab( tabId, true );
    else if( t_option == option_index )
      CRhinoTabbedDockBarDialog::ShowDockbarTab( tabId, !bVisible );
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleTabbedDockBar command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
