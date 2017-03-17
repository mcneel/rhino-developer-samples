/////////////////////////////////////////////////////////////////////////////
// cmdSampleAnimator.cpp : command file
//

#include "StdAfx.h"
#include "SampleAnimatorPlugIn.h"
#include "SampleAnimatorDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAnimator command
//

#pragma region SampleAnimator command


// Do NOT put the definition of class CCommandSampleAnimator in a header
// file.  There is only ONE instance of a CCommandSampleAnimator class
// and that instance is the static theSampleAnimatorCommand that appears
// immediately below the class definition.

class CCommandSampleAnimator : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleAnimator is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleAnimator::CCommandSampleAnimator()
  // is called exactly once when static theSampleAnimatorCommand is created.
	CCommandSampleAnimator() { m_max_steps = 100; }

  // CCommandSampleAnimator::~CCommandSampleAnimator()
  // is called exactly once when static theSampleAnimatorCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleAnimator() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {C189E68C-E54F-4B17-B6A6-67568A37DCC3}
    static const GUID SampleAnimatorCommand_UUID =
    { 0xC189E68C, 0xE54F, 0x4B17, { 0xB6, 0xA6, 0x67, 0x56, 0x8A, 0x37, 0xDC, 0xC3 } };
    return SampleAnimatorCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"SampleAnimator"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"SampleAnimator"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

private:
  int m_max_steps;
};

// The one and only CCommandSampleAnimator object.  
// Do NOT create any other instance of a CCommandSampleAnimator class.
static class CCommandSampleAnimator theSampleAnimatorCommand;

CRhinoCommand::result CCommandSampleAnimator::RunCommand( const CRhinoCommandContext& context )
{
  // Select objects to animate
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select objects to animate" );
  go.GetObjects( 1, 0 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  // Select path curve
  CRhinoGetObject gc;
  gc.SetCommandPrompt( L"Select path curve" );
  gc.SetGeometryFilter( CRhinoGetObject::curve_object );
  gc.SetGeometryAttributeFilter( CRhinoGetObject::open_curve );
  gc.EnableDeselectAllBeforePostSelect( false );
  gc.GetObjects( 1, 1 );
  if( gc.CommandResult() != success )
    return gc.CommandResult();

  const ON_Curve* crv = gc.Object(0).Curve();
  if( 0 == crv )
    return failure;

  // Create an array of normalized curve parameters
  ON_SimpleArray<double> t_array( m_max_steps );
  t_array.SetCount( m_max_steps );
  int i = 0;
  for( i = 0; i < m_max_steps; i++ )
  {
    double t = (double)i / ( (double)m_max_steps - 1 );
    t_array[i] = t;
  }

  // Get the real parameters along the curve
  if( !crv->GetNormalizedArcLengthPoints(m_max_steps, t_array.Array(), t_array.Array()) ) 
    return failure;

  // Create our dialog box with animation slider...
  CSampleAnimatorDialog dlg( CWnd::FromHandle(RhinoApp().MainWnd()) );
  dlg.m_max_steps = m_max_steps;
  dlg.m_start = crv->PointAtStart();

  // Get points along curve
  for( i = 0; i < m_max_steps; i++ )
  {
    ON_3dPoint pt = crv->PointAt( t_array[i] );
    dlg.m_points.Append( pt );
  }

  // Hide objects and add them to callback's object array
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    CRhinoObjRef ref = go.Object(i);
    context.m_doc.HideObject( ref );
    dlg.m_conduit.m_objects.Append( ref.Object() );
  }

  // Do the dialog
  INT_PTR rc = dlg.DoModal();

  // If OK was pressed, transform the objects.
  // Otherwise, just unhide them.
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    CRhinoObjRef ref = go.Object(i);
    context.m_doc.ShowObject( ref );
    if( rc == IDOK )
    {
      ON_Xform xform = dlg.m_conduit.m_xform;
      context.m_doc.TransformObject( ref, xform );
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleAnimator command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
