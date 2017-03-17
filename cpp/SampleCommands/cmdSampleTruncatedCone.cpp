#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTruncatedCone command
//

class CCommandSampleTruncatedCone : public CRhinoCommand
{
public:
  CCommandSampleTruncatedCone() {}
  ~CCommandSampleTruncatedCone() {}
  UUID CommandUUID()
  {
    // {CAC2AB9B-3F44-412D-9D2F-26AA4482B769}
    static const GUID SampleTruncatedConeCommand_UUID =
    { 0xCAC2AB9B, 0x3F44, 0x412D, { 0x9D, 0x2F, 0x26, 0xAA, 0x44, 0x82, 0xB7, 0x69 } };
    return SampleTruncatedConeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleTruncatedCone"; }
  const wchar_t* LocalCommandName() { return L"SampleTruncatedCone"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleTruncatedCone object
static class CCommandSampleTruncatedCone theSampleTruncatedConeCommand;

CRhinoCommand::result CCommandSampleTruncatedCone::RunCommand(const CRhinoCommandContext& context)
{
  ON_Plane bottom_plane = ON_Plane::World_xy;
  ON_Circle bottom_circle(bottom_plane, 5.0);

  ON_Plane top_plane = bottom_plane;
  top_plane.SetOrigin(ON_3dPoint(0.0, 0.0, 10.0));
  ON_Circle top_circle(top_plane, 3.0);

  ON_RevSurface* rev_surface = ON_RevSurface::New();

  ON_LineCurve* line_curve = new ON_LineCurve(bottom_circle.PointAt(0), top_circle.PointAt(0));
  rev_surface->m_curve = line_curve;

  ON_Line center_line(bottom_circle.Center(), top_circle.Center());
  rev_surface->m_axis = center_line;

  rev_surface->m_angle[0] = rev_surface->m_t[0] = 0.0;
  rev_surface->m_angle[1] = rev_surface->m_t[1] = 2.0 * ON_PI;

  ON_Brep truncated_cone;
  ON_BrepRevSurface(rev_surface, FALSE, FALSE, &truncated_cone);

  context.m_doc.AddBrepObject(truncated_cone);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleTruncatedCone command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
