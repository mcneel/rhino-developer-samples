/////////////////////////////////////////////////////////////////////////////
// cmdSampleMarker.cpp : command file
//

#include "StdAfx.h"
#include "SampleMarkerPlugIn.h"
#include "SampleMarkerObject.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command


// Do NOT put the definition of class CCommandSampleMarker in a header
// file.  There is only ONE instance of a CCommandSampleMarker class
// and that instance is the static theSampleMarkerCommand that appears
// immediately below the class definition.

class CCommandSampleMarker : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleMarker is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleMarker::CCommandSampleMarker()
  // is called exactly once when static theSampleMarkerCommand is created.
	CCommandSampleMarker() {}

  // CCommandSampleMarker::~CCommandSampleMarker()
  // is called exactly once when static theSampleMarkerCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleMarker() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {06438264-8517-445A-8125-F62F13E3A8B4}
    static const GUID SampleMarkerCommand_UUID =
    { 0x6438264, 0x8517, 0x445A, { 0x81, 0x25, 0xF6, 0x2F, 0x13, 0xE3, 0xA8, 0xB4 } };
    return SampleMarkerCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleMarker"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleMarker"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleMarker object.  
// Do NOT create any other instance of a CCommandSampleMarker class.
static class CCommandSampleMarker theSampleMarkerCommand;

CRhinoCommand::result CCommandSampleMarker::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt( L"Location of marker object" );
  gp.GetPoint();
  if( gp.CommandResult() == CRhinoCommand::success )
  {
    CSampleMarkerObject* marker = new CSampleMarkerObject();
    marker->SetPoint( gp.Point() );
    context.m_doc.AddObject( marker );
    context.m_doc.Redraw();
  }

  return gp.CommandResult();
}

#pragma endregion

//
// END SampleMarker command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
