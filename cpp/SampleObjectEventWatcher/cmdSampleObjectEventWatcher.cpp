/////////////////////////////////////////////////////////////////////////////
// cmdSampleObjectEventWatcher.cpp : command file
//

#include "StdAfx.h"
#include "SampleObjectEventWatcherPlugIn.h"
#include "CustomObjectEventWatcher.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectEventWatcher command
//

#pragma region SampleObjectEventWatcher command


// Do NOT put the definition of class CCommandSampleObjectEventWatcher in a header
// file.  There is only ONE instance of a CCommandSampleObjectEventWatcher class
// and that instance is the static theSampleObjectEventWatcherCommand that appears
// immediately below the class definition.

class CCommandSampleObjectEventWatcher : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleObjectEventWatcher is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleObjectEventWatcher::CCommandSampleObjectEventWatcher()
  // is called exactly once when static theSampleObjectEventWatcherCommand is created.
	CCommandSampleObjectEventWatcher();

  // CCommandSampleObjectEventWatcher::~CCommandSampleObjectEventWatcher()
  // is called exactly once when static theSampleObjectEventWatcherCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleObjectEventWatcher() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {910C6743-4D07-4C12-9B01-34D23A64A510}
    static const GUID SampleObjectEventWatcherCommand_UUID =
    { 0x910C6743, 0x4D07, 0x4C12, { 0x9B, 0x1, 0x34, 0xD2, 0x3A, 0x64, 0xA5, 0x10 } };
    return SampleObjectEventWatcherCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleObjectEventWatcher"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleObjectEventWatcher"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

private:
  // Declare event watcher as member variable
	CCustomObjectEventWatcher m_watcher;
	bool m_bRegistered;
};

// The one and only CCommandSampleObjectEventWatcher object.  
// Do NOT create any other instance of a CCommandSampleObjectEventWatcher class.
static class CCommandSampleObjectEventWatcher theSampleObjectEventWatcherCommand;

CCommandSampleObjectEventWatcher::CCommandSampleObjectEventWatcher()
{
  m_bRegistered = false;
}

CRhinoCommand::result CCommandSampleObjectEventWatcher::RunCommand( const CRhinoCommandContext& context )
{
	// Register the event watcher
  if( IDYES == RhinoMessageBox( L"Watch object events?", L"Object Watcher", MB_YESNO | MB_ICONQUESTION ) )
  {
    if( !m_bRegistered )
    {
      m_watcher.Register();
      m_bRegistered = true;
    }
	  // Enable the event watcher
    m_watcher.Enable( true );
  }
  else 
  {
	  // Disable the event watcher
    m_watcher.Enable( false );
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleObjectEventWatcher command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
