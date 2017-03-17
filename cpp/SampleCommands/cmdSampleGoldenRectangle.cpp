#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGoldenRectangle command
//

static double GoldenRatio()
{
  // From http://en.wikipedia.org/wiki/Golden_ratio
  return (1.0 + sqrt(5.0)) / 2.0;
}

class CGetGoldenRectangle : public CRhinoGetPoint
{
public:
  CGetGoldenRectangle();
  ~CGetGoldenRectangle() {}

  void SetSwap(bool bSwap);

  void OnMouseMove(CRhinoViewport& vp, UINT nFlags, const ON_3dPoint& pt, const CPoint* p);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

  void CalcCorners(CRhinoViewport& vp, const ON_3dPoint& point);
  ON_Polyline Rectangle() const;

private:
  ON_3dPoint m_corners[4];
  bool m_bSwap;
};

CGetGoldenRectangle::CGetGoldenRectangle()
{
  m_bSwap = false;
}

void CGetGoldenRectangle::SetSwap(bool bSwap)
{
  m_bSwap = bSwap;
}

ON_Polyline CGetGoldenRectangle::Rectangle() const
{
  ON_Polyline pline;
  pline.Append(4, m_corners);
  pline.Append(m_corners[0]);
  return pline;
}

void CGetGoldenRectangle::CalcCorners(CRhinoViewport& vp, const ON_3dPoint& pt)
{
  ON_Plane plane(vp.ConstructionPlane().m_plane);

  ON_3dPoint base = BasePoint();
  plane.SetOrigin(base);

  ON_2dPoint d2, b2;

  plane.ClosestPointTo(base, &b2.x, &b2.y);
  plane.ClosestPointTo(pt, &d2.x, &d2.y);

  if (fabs(b2.x - d2.x) < ON_SQRT_EPSILON && fabs(b2.y - d2.y) < ON_SQRT_EPSILON)
    return;

  double ratio = (m_bSwap) ? 1 / GoldenRatio() : GoldenRatio();

  ON_Interval x_interval(b2.x, d2.x);
  ON_Interval y_interval(b2.y, d2.x * ratio);
  if ((d2.x > 0 && d2.y < 0) || (d2.x < 0 && d2.y > 0))
    y_interval.Set(b2.y, -d2.x * ratio);

  if (!x_interval.IsIncreasing())
    x_interval.Swap();
  if (!y_interval.IsIncreasing())
    y_interval.Swap();

  m_corners[0] = plane.PointAt(x_interval[0], y_interval[0]);
  m_corners[1] = plane.PointAt(x_interval[1], y_interval[0]);
  m_corners[2] = plane.PointAt(x_interval[1], y_interval[1]);
  m_corners[3] = plane.PointAt(x_interval[0], y_interval[1]);
}

void CGetGoldenRectangle::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  dp.DrawLine(m_corners[0], m_corners[1]);
  dp.DrawLine(m_corners[1], m_corners[2]);
  dp.DrawLine(m_corners[2], m_corners[3]);
  dp.DrawLine(m_corners[3], m_corners[0]);
  CRhinoGetPoint::DynamicDraw(dp, pt);
}

void CGetGoldenRectangle::OnMouseMove(CRhinoViewport& vp, UINT nFlags, const ON_3dPoint& pt, const CPoint* p)
{
  CalcCorners(vp, pt);
  CRhinoGetPoint::OnMouseMove(vp, nFlags, pt, p);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleGoldenRectangle : public CRhinoCommand
{
public:
  CCommandSampleGoldenRectangle() { m_bSwap = false; }
  ~CCommandSampleGoldenRectangle() {}
  UUID CommandUUID()
  {
    // {8332424B-2371-4B85-9C35-586ED543DAEA}
    static const GUID SampleGoldenRectangleCommand_UUID =
    { 0x8332424B, 0x2371, 0x4B85, { 0x9C, 0x35, 0x58, 0x6E, 0xD5, 0x43, 0xDA, 0xEA } };
    return SampleGoldenRectangleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGoldenRectangle"; }
  const wchar_t* LocalCommandName() { return L"SampleGoldenRectangle"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool m_bSwap;
};

// The one and only CCommandSampleGoldenRectangle object
static class CCommandSampleGoldenRectangle theSampleGoldenRectangleCommand;

CRhinoCommand::result CCommandSampleGoldenRectangle::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"First corner of rectangle");
  gp.ConstrainToConstructionPlane();
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint origin(gp.Point());

  CGetGoldenRectangle gr;
  gr.SetCommandPrompt(L"Other corner of rectangle");
  gr.AddCommandOptionToggle(RHCMDOPTNAME(L"Swap"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), m_bSwap, &m_bSwap);
  gr.SetBasePoint(origin);
  gr.ConstrainToConstructionPlane();
  for (;;)
  {
    CRhinoGet::result res = gr.GetPoint();

    if (res == CRhinoGet::option)
    {
      gr.SetSwap(m_bSwap);
      continue;
    }

    if (res != CRhinoGet::point)
      return CRhinoCommand::cancel;

    break;
  }

  CRhinoView* view = gr.View();
  if (0 == view)
    return CRhinoCommand::failure;

  gr.CalcCorners(view->ActiveViewport(), gr.Point());
  ON_Polyline rect = gr.Rectangle();
  if (rect.IsValid())
  {
    context.m_doc.AddCurveObject(rect);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleGoldenRectangle command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
