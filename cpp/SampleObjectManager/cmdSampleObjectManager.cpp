/////////////////////////////////////////////////////////////////////////////
// cmdSampleObjectManager.cpp : command file
//

#include "StdAfx.h"
#include "SampleObjectManagerPlugIn.h"
#include "SampleObjectManagerDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectManager command
//

#pragma region SampleObjectManager command


// Do NOT put the definition of class CCommandSampleObjectManager in a header
// file.  There is only ONE instance of a CCommandSampleObjectManager class
// and that instance is the static theSampleObjectManagerCommand that appears
// immediately below the class definition.

class CCommandSampleObjectManager : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleObjectManager is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleObjectManager::CCommandSampleObjectManager()
  // is called exactly once when static theSampleObjectManagerCommand is created.
	CCommandSampleObjectManager() {}

  // CCommandSampleObjectManager::~CCommandSampleObjectManager()
  // is called exactly once when static theSampleObjectManagerCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleObjectManager() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {415ABCAE-E332-466A-B139-2963E81166C1}
    static const GUID SampleObjectManagerCommand_UUID =
    { 0x415ABCAE, 0xE332, 0x466A, { 0xB1, 0x39, 0x29, 0x63, 0xE8, 0x11, 0x66, 0xC1 } };
    return SampleObjectManagerCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleObjectManager"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleObjectManager"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleObjectManager object.  
// Do NOT create any other instance of a CCommandSampleObjectManager class.
static class CCommandSampleObjectManager theSampleObjectManagerCommand;

CRhinoCommand::result CCommandSampleObjectManager::RunCommand( const CRhinoCommandContext& context )
{
  ON_UUID tabId = CSampleObjectManagerDialog::ID();

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
// END SampleObjectManager command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
