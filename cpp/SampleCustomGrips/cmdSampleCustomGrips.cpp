/////////////////////////////////////////////////////////////////////////////
// cmdSampleCustomGrips.cpp : command file
//

#include "StdAfx.h"
#include "SampleCustomGripsPlugIn.h"
#include "RhinoRectangleObject.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomGrips command
//

#pragma region SampleCustomGrips command


// Do NOT put the definition of class CCommandSampleCustomGrips in a header
// file.  There is only ONE instance of a CCommandSampleCustomGrips class
// and that instance is the static theSampleCustomGripsCommand that appears
// immediately below the class definition.

class CCommandSampleCustomGrips : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleCustomGrips is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleCustomGrips::CCommandSampleCustomGrips()
  // is called exactly once when static theSampleCustomGripsCommand is created.
	CCommandSampleCustomGrips() {}

  // CCommandSampleCustomGrips::~CCommandSampleCustomGrips()
  // is called exactly once when static theSampleCustomGripsCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleCustomGrips() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {C28CAF32-E5C1-4628-944D-34C47DE267BC}
    static const GUID SampleCustomGripsCommand_UUID =
    { 0xC28CAF32, 0xE5C1, 0x4628, { 0x94, 0x4D, 0x34, 0xC4, 0x7D, 0xE2, 0x67, 0xBC } };
    return SampleCustomGripsCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleCustomGrips"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleCustomGrips"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleCustomGrips object.  
// Do NOT create any other instance of a CCommandSampleCustomGrips class.
static class CCommandSampleCustomGrips theSampleCustomGripsCommand;

CRhinoCommand::result CCommandSampleCustomGrips::RunCommand( const CRhinoCommandContext& context )
{
  ON_3dPoint rect[5];
  
  CArgsRhinoGetPlane args;
  args.SetCornerMode( CArgsRhinoGetPlane::mode_corners );
  args.SetAllowRounded( FALSE );
  args.SetAllowDeformable( FALSE );
 
  CRhinoCommand::result rc = RhinoGetRectangle( args, rect );
  
  if( rc == CRhinoCommand::success)
  {
    ON_Polyline pline;
    rect[4] = rect[0];
    pline.Create( 3, FALSE, 5, 3, (double*)&rect );

    ON_PolylineCurve* pline_curve = new ON_PolylineCurve( pline );
    
    CRhinoRectangleObject* rect_object = new CRhinoRectangleObject();
    rect_object->SetCurve( pline_curve );

    if( context.m_doc.AddObject(rect_object) )
      context.m_doc.Redraw();
    else
      delete rect_object;
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomGrips command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
