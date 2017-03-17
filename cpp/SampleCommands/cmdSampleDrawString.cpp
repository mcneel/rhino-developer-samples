#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawString command
//

class CDrawStringGetPoint : public CRhinoGetPoint
{
public:
  CDrawStringGetPoint() {}
  void DynamicDraw( HDC hdc, CRhinoViewport& vp, const ON_3dPoint& pt );
};

void CDrawStringGetPoint::DynamicDraw( HDC hdc, CRhinoViewport& vp, const ON_3dPoint& pt )
{
  // Format active point as a string
  ON_wString str;
  RhinoFormatPoint( pt, str );

  // Build world-to-screen coordinate transformation
  ON_Xform w2s;
  vp.VP().GetXform( ON::world_cs, ON::screen_cs, w2s );

  // Transform point from world to screen coordinates
  ON_3dPoint screenpoint = w2s * pt;

  // Offset point so text does not overlap cursor
  screenpoint.x += 5.0;
  screenpoint.y += -5.0;

  // Draw string using the system font
  vp.DrawString( str, str.Length(), screenpoint, false, 0, 12, L"System" );

  // Allow base class to draw
  CRhinoGetPoint::DynamicDraw( hdc, vp, pt );
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleDrawString : public CRhinoCommand
{
public:
	CCommandSampleDrawString() {}
	~CCommandSampleDrawString() {}
	UUID CommandUUID()
	{
		// {72E436CE-A0E0-4D46-855B-1E767B5B045E}
		static const GUID SampleDrawStringCommand_UUID =
		{ 0x72E436CE, 0xA0E0, 0x4D46, { 0x85, 0x5B, 0x1E, 0x76, 0x7B, 0x5B, 0x04, 0x5E } };
		return SampleDrawStringCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleDrawString"; }
	const wchar_t* LocalCommandName() { return L"SampleDrawString"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDrawString object
static class CCommandSampleDrawString theSampleDrawStringCommand;

CRhinoCommand::result CCommandSampleDrawString::RunCommand( const CRhinoCommandContext& context )
{
	CDrawStringGetPoint gp;
  gp.SetCommandPrompt( L"Pick test point" );
  gp.GetPoint();
  if( gp.CommandResult() != CRhinoCommand::success )
    return gp.CommandResult();

	return CRhinoCommand::success;
}

//
// END SampleDrawString command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
