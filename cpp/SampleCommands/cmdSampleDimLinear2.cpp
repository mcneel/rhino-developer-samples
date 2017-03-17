#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimLinear2 command
//

#pragma region SampleDimLinear2 command

class CCommandSampleDimLinear2 : public CRhinoCommand
{
public:
  CCommandSampleDimLinear2() {}
  ~CCommandSampleDimLinear2() {}
  UUID CommandUUID()
  {
    // {20B62071-3B41-4250-B295-8D4226530A4D}
    static const GUID SampleDimLinear2Command_UUID =
    { 0x20B62071, 0x3B41, 0x4250, { 0xB2, 0x95, 0x8D, 0x42, 0x26, 0x53, 0x0A, 0x4D } };
    return SampleDimLinear2Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimLinear2"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimLinear2"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDimLinear2 object
static class CCommandSampleDimLinear2 theSampleDimLinear2Command;

CRhinoCommand::result CCommandSampleDimLinear2::RunCommand( const CRhinoCommandContext& context )
{
  ON_3dPoint origin( 1, 1, 0 );
  ON_3dPoint offset( 11, 1, 0 );
  ON_3dPoint pt( (offset.x-origin.x)/2, 3, 0 );
 
  CRhinoLinearDimension* dim_obj = new CRhinoLinearDimension();
 
  ON_Plane plane( ON_xy_plane );
  plane.SetOrigin( origin );
  dim_obj->SetPlane( plane );

  double u, v;
  plane.ClosestPointTo( origin, &u, &v );
  dim_obj->SetPoint( 0, ON_2dPoint(u,v) );
 
  plane.ClosestPointTo( offset, &u, &v );
  dim_obj->SetPoint( 2, ON_2dPoint(u,v) );
 
  plane.ClosestPointTo( pt, &u, &v );

  ON_2dPoint p2d( u, v );
  dim_obj->UpdateDimPoints( p2d );
  dim_obj->UpdateText();
 
  if( context.m_doc.AddObject(dim_obj) )
    context.m_doc.Redraw();
  else
    delete dim_obj;

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDimLinear2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
