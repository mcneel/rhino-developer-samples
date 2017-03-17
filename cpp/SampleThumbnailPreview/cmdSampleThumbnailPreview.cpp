/////////////////////////////////////////////////////////////////////////////
// cmdSampleThumbnailPreview.cpp : command file
//

#include "StdAfx.h"
#include "SampleThumbnailPreviewPlugIn.h"
#include "SampleThumbnailPreviewDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleThumbnailPreview command
//

#pragma region SampleThumbnailPreview command


// Do NOT put the definition of class CCommandSampleThumbnailPreview in a header
// file.  There is only ONE instance of a CCommandSampleThumbnailPreview class
// and that instance is the static theSampleThumbnailPreviewCommand that appears
// immediately below the class definition.

class CCommandSampleThumbnailPreview : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleThumbnailPreview is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleThumbnailPreview::CCommandSampleThumbnailPreview()
  // is called exactly once when static theSampleThumbnailPreviewCommand is created.
	CCommandSampleThumbnailPreview() {}

  // CCommandSampleThumbnailPreview::~CCommandSampleThumbnailPreview()
  // is called exactly once when static theSampleThumbnailPreviewCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleThumbnailPreview() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {52C573FE-16F4-4643-80A6-5CB9EDFB8189}
    static const GUID SampleThumbnailPreviewCommand_UUID =
    { 0x52C573FE, 0x16F4, 0x4643, { 0x80, 0xA6, 0x5C, 0xB9, 0xED, 0xFB, 0x81, 0x89 } };
    return SampleThumbnailPreviewCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleThumbnailPreview"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleThumbnailPreview"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleThumbnailPreview object.  
// Do NOT create any other instance of a CCommandSampleThumbnailPreview class.
static class CCommandSampleThumbnailPreview theSampleThumbnailPreviewCommand;

CRhinoCommand::result CCommandSampleThumbnailPreview::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandSampleThumbnailPreview::RunCommand() is called when the user runs the "SampleThumbnailPreview"
  // command or the "SampleThumbnailPreview" command is run by a history operation.

  // TODO: Add command code here.

  CSampleThumbnailPreviewDialog dialog( CWnd::FromHandle(RhinoApp().MainWnd()) );
  dialog.DoModal();

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
// END SampleThumbnailPreview command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
