#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRotate command
//

class CGetAngleFirstPoint : public CRhinoGetPoint
{
public:
  CGetAngleFirstPoint(const ON_Plane& plane);

  // CRhinoGetPoint overrides
  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* view_wnd_point);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point);

protected:
  ON_Plane m_base_plane;
  ON_Color m__color;
  ON_Circle m__circle;
  bool m__draw;
};

CGetAngleFirstPoint::CGetAngleFirstPoint(const ON_Plane& plane)
  : m_base_plane(plane)
  , m__draw(false)
{
  m__color = RhinoApp().AppSettings().TrackingColor();
  Constrain(plane);
  SetBasePoint(plane.origin);
  DrawLineFromPoint(plane.origin, true);
}

void CGetAngleFirstPoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* view_wnd_point)
{
  ON_3dPoint base_point = BasePoint();
  m__draw = m__circle.Create(m_base_plane, base_point, point.DistanceTo(base_point));
  CRhinoGetPoint::OnMouseMove(vp, flags, point, view_wnd_point);
}

void CGetAngleFirstPoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point)
{
  if (m__draw)
    dp.DrawCircle(m__circle, m__color);
  CRhinoGetPoint::DynamicDraw(dp, point);
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CGetAngleSecondPoint : public CRhinoGetPoint
{
public:
  CGetAngleSecondPoint(const ON_Plane& plane, const ON_3dPoint& refpt);

  // CRhinoGetPoint overrides
  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* view_wnd_point);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point);

  double Angle() const;

private:
  bool CalculateArc(int& quadrant, int& dir, const ON_3dVector& N, const ON_3dPoint& P1, const ON_3dPoint& P2, const ON_3dPoint& P3, ON_Arc& arc);
  int WhichQuadrant(const ON_3dVector& n, const ON_3dPoint& p1, const ON_3dPoint& p2, const ON_3dPoint& p3);

private:
  ON_Plane m_base_plane;
  ON_3dPoint m_ref_point;
  int m_quadrant;
  int m_dir;
  ON_Arc m_arc;
};

CGetAngleSecondPoint::CGetAngleSecondPoint(const ON_Plane& plane, const ON_3dPoint& refpt)
  : m_base_plane(plane)
  , m_ref_point(refpt)
  , m_quadrant(0)
  , m_dir(1)
{
  Constrain(plane);
  SetBasePoint(plane.origin);
  DrawLineFromPoint(plane.origin, true);
}

void CGetAngleSecondPoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* view_wnd_point)
{
  int quadrant = m_quadrant;
  int dir = m_dir;
  if (CalculateArc(quadrant, dir, m_base_plane.Normal(), BasePoint(), m_ref_point, point, m_arc))
  {
    m_quadrant = quadrant;
    m_dir = dir;
  }

  RhinoApp().SetStatusBarPointPane(point);
  RhinoApp().SetStatusBarDistancePane(m_arc.AngleDegrees() * m_dir);

  CRhinoGetPoint::OnMouseMove(vp, flags, point, view_wnd_point);
}

void CGetAngleSecondPoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point)
{
  if (m_arc.IsValid())
  {
    dp.DrawArc(m_arc);

    ON_3dPoint start = m_arc.StartPoint();
    ON_3dVector v(start - m_arc.Center());
    v *= 1.5;
    dp.DrawLine(m_arc.Center(), m_arc.Center() + v);

    v = m_arc.EndPoint() - m_arc.Center();
    v *= 1.5;
    ON_3dVector v1(point - m_arc.Center());
    if (v1.LengthSquared() > v.LengthSquared())
      v = v1;
    dp.DrawLine(m_arc.Center(), m_arc.Center() + v);

    dp.DrawPoint(m_arc.Center());
    dp.DrawPoint(start);
  }

  CRhinoGetPoint::DynamicDraw(dp, point);
}

bool CGetAngleSecondPoint::CalculateArc(int& quadrant, int& dir, const ON_3dVector& N, const ON_3dPoint& P1, const ON_3dPoint& P2, const ON_3dPoint& P3, ON_Arc& arc)
{
  ON_3dVector X(P2 - P1);
  ON_3dVector Y(P3 - P1);
  if (X.IsTiny() || Y.IsTiny())
    return FALSE;

  double radius = X.Length();

  int newquadrant = WhichQuadrant(N, P1, P2, P3);
  if (quadrant == 0)
  {
    dir = (newquadrant < 3) ? 1 : -1; // unspecified previous quadrant
  }
  else if ((quadrant == 1 && newquadrant == 4 && dir == 1) || (quadrant == 4 && newquadrant == 1 && dir == -1))
  {
    dir = -dir; // switched directions
  }

  quadrant = newquadrant;
  X.Unitize();
  Y.Unitize();
  double dot = ON_DotProduct(X, Y);
  dot = RHINO_CLAMP(dot, -1.0, 1.0);
  double angle = acos(dot);

  if (dir > 0)
    Y = ON_CrossProduct(N, X);
  else
    Y = ON_CrossProduct(-N, X);

  ON_Plane plane(P1, X, Y);
  if (!plane.IsValid())
    return FALSE;

  if ((dir == 1 && newquadrant > 2) || (dir == -1 && newquadrant < 3))
    angle = 2.0*ON_PI - angle;

  return arc.Create(plane, P1, radius, angle);
}

int CGetAngleSecondPoint::WhichQuadrant(const ON_3dVector& N, const ON_3dPoint& P1, const ON_3dPoint& P2, const ON_3dPoint& P3)
{
  ON_3dVector X(P2 - P1);
  ON_3dVector R(P3 - P1);
  ON_3dVector Y = ON_CrossProduct(N, X);
  double dotX = ON_DotProduct(X, R);
  double dotY = ON_DotProduct(Y, R);
  if (dotX >= 0.0)
    return (dotY >= 0.0) ? 1 : 4;
  return (dotY >= 0.0) ? 2 : 3;
}


double CGetAngleSecondPoint::Angle() const
{
  return m_arc.AngleRadians() * m_dir;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleRotate : public CRhinoCommand
{
public:
  CCommandSampleRotate() {}
  ~CCommandSampleRotate() {}
  UUID CommandUUID()
  {
    // {61BC5D27-18B7-4046-B646-DA8CE3A27271}
    static const GUID SampleRotateCommand_UUID =
    { 0x61BC5D27, 0x18B7, 0x4046,{ 0xB6, 0x46, 0xDA, 0x8C, 0xE3, 0xA2, 0x72, 0x71 } };
    return SampleRotateCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleRotate"; }
  const wchar_t* LocalCommandName() const { return L"SampleRotate"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleRotate object
static class CCommandSampleRotate theSampleRotateCommand;

CRhinoCommand::result CCommandSampleRotate::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Base point");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  CRhinoView* view = gp.View();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_Plane plane = view->ActiveViewport().ConstructionPlane().m_plane;
  plane.SetOrigin(gp.Point());


  CGetAngleFirstPoint ga1(plane);
  ga1.SetCommandPrompt(L"Angle or first reference point");
  ga1.GetPoint();
  if (ga1.CommandResult() != CRhinoCommand::success)
    return ga1.CommandResult();

  ON_3dPoint point = plane.ClosestPointTo(ga1.Point());
  ON_3dVector xaxis = point - plane.origin;
  if (!xaxis.Unitize())
    return CRhinoCommand::failure;

  plane.xaxis = xaxis;
  plane.yaxis = ON_CrossProduct(plane.zaxis, plane.xaxis);
  plane.yaxis.Unitize();
  plane.UpdateEquation();


  CGetAngleSecondPoint ga2(plane, point);
  ga2.SetCommandPrompt(L"Second reference point");
  ga2.GetPoint();
  if (ga2.CommandResult() != CRhinoCommand::success)
    return ga2.CommandResult();

  double angle = ga2.Angle();
  ON_wString angle_str;
  RhinoFormatNumber(angle * 180.0 / ON_PI, angle_str);
  RhinoApp().Print(L"%s\n", angle_str.Array());

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRotate command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
