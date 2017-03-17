#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCircle command
//

class CGetCircleRadiusPoint : public CRhinoGetPoint
{
public:
  CGetCircleRadiusPoint();

  void SetCenterPoint(const ON_3dPoint center_point);

  bool CalculateCircle(CRhinoViewport& vp, const ON_3dPoint& pt);

  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* pt2d);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

  const ON_Circle& Circle() const;

private:
  ON_Circle m_circle;
  ON_3dPoint m_center_point;
  bool m_draw_circle;
};

CGetCircleRadiusPoint::CGetCircleRadiusPoint()
{
  m_draw_circle = false;
}

void CGetCircleRadiusPoint::SetCenterPoint(const ON_3dPoint center_point)
{
  m_center_point = center_point;
}

bool CGetCircleRadiusPoint::CalculateCircle(CRhinoViewport& vp, const ON_3dPoint& pt)
{
  double radius = m_center_point.DistanceTo(pt);
  if (radius < ON_SQRT_EPSILON)
    return false;

  ON_Plane plane = vp.ConstructionPlane().m_plane;
  plane.SetOrigin(m_center_point);

  m_circle.Create(plane, radius);

  return m_circle.IsValid() ? true : false;
}

void CGetCircleRadiusPoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* pt2d)
{
  m_draw_circle = CalculateCircle(vp, pt);

  CRhinoGetPoint::OnMouseMove(vp, flags, pt, pt2d);
}

void CGetCircleRadiusPoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  if (m_draw_circle)
    dp.DrawCircle(m_circle, RhinoApp().AppSettings().ActiveLayerColor());

  CRhinoGetPoint::DynamicDraw(dp, pt);
}

const ON_Circle& CGetCircleRadiusPoint::Circle() const
{
  return m_circle;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CCommandSampleCircle : public CRhinoCommand
{
public:
  CCommandSampleCircle() {}
  ~CCommandSampleCircle() {}
  UUID CommandUUID()
  {
    // {4018B527-BC6E-49EA-B729-4043628802CD}
    static const GUID SampleCircleCommand_UUID =
    { 0x4018B527, 0xBC6E, 0x49EA, { 0xB7, 0x29, 0x40, 0x43, 0x62, 0x88, 0x02, 0xCD } };
    return SampleCircleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCircle"; }
  const wchar_t* LocalCommandName() { return L"SampleCircle"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCircle object
static class CCommandSampleCircle theSampleCircleCommand;

CRhinoCommand::result CCommandSampleCircle::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Center point");
  gp.ConstrainToConstructionPlane(FALSE);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint center_point = gp.Point();

  CGetCircleRadiusPoint gc;
  gc.SetCommandPrompt(L"Radius");
  gc.ConstrainToConstructionPlane(FALSE);
  gc.SetBasePoint(center_point);
  gc.DrawLineFromPoint(center_point, TRUE);
  gc.SetCenterPoint(center_point);
  gc.GetPoint();
  if (gc.CommandResult() != CRhinoCommand::success)
    return gc.CommandResult();

  if (gc.CalculateCircle(gc.View()->Viewport(), gc.Point()))
  {
    ON_Circle circle = gc.Circle();
    context.m_doc.AddCurveObject(circle);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleCircle command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
