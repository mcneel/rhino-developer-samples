#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLeader command
//

class CCommandSampleLeader : public CRhinoCommand
{
public:
	CCommandSampleLeader() {}
	~CCommandSampleLeader() {}
	UUID CommandUUID()
	{
		// {7D5E43E9-D737-49CB-BD44-39E1C6957B54}
		static const GUID SampleLeaderCommand_UUID =
		{ 0x7D5E43E9, 0xD737, 0x49CB, { 0xBD, 0x44, 0x39, 0xE1, 0xC6, 0x95, 0x7B, 0x54 } };
		return SampleLeaderCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleLeader"; }
	const wchar_t* LocalCommandName() { return L"SampleLeader"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleLeader object
static class CCommandSampleLeader theSampleLeaderCommand;

CRhinoCommand::result CCommandSampleLeader::RunCommand( const CRhinoCommandContext& context )
{
  // Some set of points that define the leader
  ON_3dPointArray points;
  points.Append( ON_3dPoint(1.0, 1.0, 0.0) );
  points.Append( ON_3dPoint(5.0, 1.0, 0.0) );
  points.Append( ON_3dPoint(5.0, 5.0, 0.0) );
  points.Append( ON_3dPoint(9.0, 5.0, 0.0) );

  // The plane in which the leader resides
  ON_Plane plane = ON_xy_plane;

  // Create the leader
  ON_Leader2 leader;
  leader.SetPlane( plane );

  // Add the points to the leader
  int i;
  for( i = 0; i < points.Count(); i++ )
  {
    ON_2dPoint p2;
    if( leader.m_plane.ClosestPointTo(points[i], &p2.x, &p2.y) )
    {
      if( leader.m_points.Count() < 1 || p2.DistanceTo(*leader.m_points.Last()) > ON_SQRT_EPSILON )
        leader.m_points.Append( p2 );
    }
  }

  // Create the leader object
  CRhinoAnnotationLeader* leader_object = new CRhinoAnnotationLeader();
  // Add our leader to the object
  leader_object->SetAnnotation( leader );

  if( context.m_doc.AddObject(leader_object) )
    context.m_doc.Redraw();
  else
    delete leader_object; // error

  return CRhinoCommand::success;
}

//
// END SampleLeader command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
