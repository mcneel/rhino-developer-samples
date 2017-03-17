#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddLayout command
//

class CCommandSampleAddLayout : public CRhinoCommand
{
public:
	CCommandSampleAddLayout() {}
	~CCommandSampleAddLayout() {}
	UUID CommandUUID()
	{
		// {4787C41-F962-4767-BC47-D43B93CF1A1E}
		static const GUID SampleAddLayoutCommand_UUID =
		{ 0x4787C41, 0xF962, 0x4767, { 0xBC, 0x47, 0xD4, 0x3B, 0x93, 0xCF, 0x1A, 0x1E } };
		return SampleAddLayoutCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleAddLayout"; }
	const wchar_t* LocalCommandName() { return L"SampleAddLayout"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleAddLayout object
static class CCommandSampleAddLayout theSampleAddLayoutCommand;

CRhinoCommand::result CCommandSampleAddLayout::RunCommand( const CRhinoCommandContext& context )
{
  // Add a new page view (layout)
  CRhinoPageView* page_view = CRhinoPageView::CreatePageView();
  if( 0 == page_view )
    return CRhinoCommand::failure;

  double scale = ON::UnitScale( ON::inches, ON::millimeters );

  ON_3dmPageSettings page_settings;
  page_settings.m_width_mm = 11.0 * scale;
  page_settings.m_height_mm = 8.5 * scale;

  // Set the page view's "view"
  ON_3dmView view = page_view->MainViewport().View();
  view.m_name = L"MyPage";
  view.m_page_settings = page_settings;
  page_view->MainViewport().SetView( view );

  // Zoom the page view
  RhinoDollyExtents( page_view );
  page_view->MainViewport().ClearUndoInformation();

  double margin = 10.0;;
  double bottom = 10.0;
  double top = page_settings.m_height_mm - margin;
  double left = margin;
  double right = page_settings.m_width_mm - margin;

  ON_2dPoint corner1( left, top );
  ON_2dPoint corner2( right, bottom );

  ON_UnitSystem units_mm( ON::millimeters );

  // Add a detail view as a top view
  CRhinoDetailViewObject* detail_view = page_view->AddDetailView( corner1, corner2, &units_mm );
  if( detail_view )
  {
    detail_view->Viewport().m_v.m_bLockedProjection = false;
    detail_view->Viewport().SetToTopView( L"Top" );
    detail_view->Viewport().m_v.m_vp.UnlockFrustumSymmetry();
    detail_view->DollyExtents();
  }

  // Set the page view as the active view
  RhinoApp().SetActiveView( page_view );

	return CRhinoCommand::success;
}

//
// END SampleAddLayout command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
