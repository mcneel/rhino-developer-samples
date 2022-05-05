#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSnapToPoint command
//

class CSnapToPickPoint : public CRhinoGetPoint
{
public:
  CSnapToPickPoint(ON_3dPoint pick_point);
  ~CSnapToPickPoint() = default;

  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const ON_2iPoint*) override;
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point) override;

  ON_3dPoint m_pick_point;
  bool m_have_snap_point;
  double m_max_dist;
};

CSnapToPickPoint::CSnapToPickPoint(ON_3dPoint pick_point)
  : m_pick_point(pick_point)
{
  m_have_snap_point = false;
  int osnap_pickbox_radius = RhinoApp().AppSettings().OsnapPickboxRadius();
  m_max_dist = osnap_pickbox_radius > 15
    ? osnap_pickbox_radius
    : 15;
}

void CSnapToPickPoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const ON_2iPoint*)
{
  ON_Xform w2s;
  vp.VP().GetXform(ON::world_cs, ON::screen_cs, w2s);
  const ON_3dPoint mouse_pt = w2s * point;
  ON_2dPoint test_pt = w2s * m_pick_point;
  double d = (test_pt - mouse_pt).MaximumCoordinate();
  m_have_snap_point = d <= m_max_dist;
}

void CSnapToPickPoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point)
{
  dp.DrawPoint(m_pick_point);
  if (m_have_snap_point)
    dp.DrawPoint(m_pick_point);
  else
    CRhinoGetPoint::DynamicDraw(dp, point);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleSnapToPoint : public CRhinoCommand
{
public:
  CCommandSampleSnapToPoint() = default;
  UUID CommandUUID() override
  {
    // {EEA15C8-7D13-4EF2-99BC-3AB0FDBECD56}
    static const GUID SampleSnapToPointCommand_UUID =
    { 0xEEA15C8, 0x7D13, 0x4EF2, { 0x99, 0xBC, 0x3A, 0xB0, 0xFD, 0xBE, 0xCD, 0x56 } };
    return SampleSnapToPointCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSnapToPoint"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSnapToPoint object
static class CCommandSampleSnapToPoint theSampleSnapToPointCommand;

CRhinoCommand::result CCommandSampleSnapToPoint::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point location");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint pick_point = gp.Point();

  CSnapToPickPoint gs(pick_point);
  gs.SetCommandPrompt(L"Snap to point location");
  gs.GetPoint();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  return CRhinoCommand::success;
}

//
// END SampleSnapToPoint command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
