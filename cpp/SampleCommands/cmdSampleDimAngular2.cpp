#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimAngular2 command
//

#pragma region SampleDimAngular2 command

class CCommandSampleDimAngular2 : public CRhinoCommand
{
public:
  CCommandSampleDimAngular2() {}
  ~CCommandSampleDimAngular2() {}
  UUID CommandUUID()
  {
    // {2F11D133-3E10-4A9B-8AEC-2CE268E7885C}
    static const GUID SampleDimAngular2Command_UUID =
    { 0x2F11D133, 0x3E10, 0x4A9B, { 0x8A, 0xEC, 0x2C, 0xE2, 0x68, 0xE7, 0x88, 0x5C } };
    return SampleDimAngular2Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimAngular2"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimAngular2"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDimAngular2 object
static class CCommandSampleDimAngular2 theSampleDimAngular2Command;

CRhinoCommand::result CCommandSampleDimAngular2::RunCommand( const CRhinoCommandContext& context )
{
  ON_3dPoint origin(0, 0, 0);
  ON_3dPoint p3d0(10, 0, 0);
  ON_3dPoint p3d1(0, 10, 0);
  ON_3dPoint p3d2(5, 5, 0 );

  // Dimension plane
  ON_Plane plane = ON_Plane::World_xy;
  plane.SetOrigin(origin);

  // Points on plane
  ON_2dPoint p2d0, p2d1, p2d2;
  plane.ClosestPointTo(p3d0, &p2d0.x, &p2d0.y);
  plane.ClosestPointTo(p3d1, &p2d1.x, &p2d1.y);
  plane.ClosestPointTo(p3d2, &p2d2.x, &p2d2.y);

  ON_3dPoint point0 = plane.PointAt(p2d0.x, p2d0.y);
  ON_3dPoint point1 = plane.PointAt(p2d1.x, p2d1.y);
  ON_3dPoint point2 = plane.PointAt(p2d2.x, p2d2.y);

  CRhinoAngularDimension* dim = new CRhinoAngularDimension();
  dim->CreateFromPoints(plane.Origin(), point0, point1, point2);

  context.m_doc.AddObject(dim);
  context.m_doc.Redraw();
  
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDimAngular2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
