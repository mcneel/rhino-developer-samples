#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBackgroundBitmap command
//

class CCommandSampleBackgroundBitmap : public CRhinoCommand
{
public:
	CCommandSampleBackgroundBitmap() {}
	~CCommandSampleBackgroundBitmap() {}
	UUID CommandUUID()
	{
		// {9E9A9E47-FC76-4F27-B078-02BEC87F98B0}
		static const GUID SampleBackgroundBitmapCommand_UUID =
		{ 0x9E9A9E47, 0xFC76, 0x4F27, { 0xB0, 0x78, 0x02, 0xBE, 0xC8, 0x7F, 0x98, 0xB0 } };
		return SampleBackgroundBitmapCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleBackgroundBitmap"; }
	const wchar_t* LocalCommandName() { return L"SampleBackgroundBitmap"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleBackgroundBitmap object
static class CCommandSampleBackgroundBitmap theSampleBackgroundBitmapCommand;

CRhinoCommand::result CCommandSampleBackgroundBitmap::RunCommand( const CRhinoCommandContext& context )
{
  // Allow the user to select a bitmap file
  ON_wString bitmap_filename;

  CRhinoGetFileDialog gf;
  gf.SetScriptMode( context.IsInteractive() ? FALSE : TRUE );
  BOOL rc = gf.DisplayFileDialog( 
        CRhinoGetFileDialog::open_bitmap_dialog, 
        bitmap_filename, 
        CWnd::FromHandle( RhinoApp().MainWnd() ) 
        );

  if( !rc )
    return CRhinoCommand::nothing;

  // Verify the file that was selected
  bitmap_filename = gf.FileName();
  bitmap_filename.TrimLeftAndRight();
  if( bitmap_filename.IsEmpty() )
    return CRhinoCommand::nothing;

  // Verify that the bitmap file is valid
  CRhinoDib dib;
  if( !dib.ReadFromFile(bitmap_filename) )
  {
    RhinoApp().Print( L"The specified file cannot be identifed as a supported type.\n" );
    return CRhinoCommand::nothing;
  }

  // Allow the user to pick the bitmap origin
  CRhinoGetPoint gp;
  gp.SetCommandPrompt( L"Bitmap origin" );
  gp.ConstrainToConstructionPlane();
  gp.GetPoint();
  if( gp.CommandResult() != CRhinoCommand::success )
    return gp.CommandResult();

  // Get the view that the point was picked in.
  // This will be the view that the bitmap appears in.
  CRhinoView* view = gp.View();
  if( !view )
  {
    view = RhinoApp().ActiveView();
    if( !view )
      return CRhinoCommand::failure;
  }

  // Allow the user to specify the bitmap with in model units
  CRhinoGetNumber gn;
  gn.SetCommandPrompt( L"Bitmap width" );
  gn.SetLowerLimit( 1.0 );
  gn.GetNumber();
  if( gn.CommandResult() != CRhinoCommand::success )
    return gn.CommandResult();

  // Cook up some scale factors
  double w = gn.Number();
  double dib_width = (double)dib.Width();
  double dib_height = (double)dib.Height();
  double h = w * ( dib_height / dib_width );
    
  // Calculate the 3-D points that bound the bitmap
  ON_3dPoint rect[4];
  rect[0] = gp.Point();
  rect[1] = ON_3dPoint(rect[0].x+w, rect[0].y,   rect[0].z);
  rect[2] = ON_3dPoint(rect[0].x+w, rect[0].y+h, rect[0].z);
  rect[3] = ON_3dPoint(rect[0].x,   rect[0].y+h, rect[0].z);

  // Get Rhino's bitmap table
  CRhinoBitmapTable& bitmap_table = context.m_doc.m_bitmap_table;

  // Search the bitmap table to see if the bitmap has already
  // been used. If so, delete the bitmap so Rhino will not just
  // reuse the one it already has (if we want to re-read the bitmap
  // file
  const CRhinoBitmap* rhino_bitmap = bitmap_table.Bitmap( bitmap_filename );
  if( rhino_bitmap )
  {
    view->ActiveViewport().SetTraceImage( ON_3dmViewTraceImage() );
    bitmap_table.DeleteBitmap( bitmap_filename );
  }
  rhino_bitmap = 0;

  // Create the trace image (background bitmap) object
  ON_3dmViewTraceImage trace_image;
  trace_image.m_bitmap_filename = bitmap_filename;
  trace_image.m_bGrayScale = false;
  trace_image.m_plane = view->ActiveViewport().ConstructionPlane().m_plane;
  trace_image.m_plane.SetOrigin( rect[0] );
  trace_image.m_width = fabs( (rect[0] - rect[1]).Length() );
  trace_image.m_height = fabs( (rect[0] - rect[3]).Length() );
  trace_image.m_bHidden = false;

  // Add the trace image to the viewport
  view->ActiveViewport().SetTraceImage( trace_image );
  view->Redraw();

  return CRhinoCommand::success;
}

//
// END SampleBackgroundBitmap command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
