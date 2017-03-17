#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMirror command
//

class CMirror1Xform : public CRhinoGetXform
{
public:
  BOOL CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform);
};

BOOL CMirror1Xform::CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform)
{
  ON_Plane plane = vp.ConstructionPlane().m_plane;
  ON_3dVector dir = ON_CrossProduct(pt - m_basepoint, plane.Normal());
  bool rc = dir.Unitize();
  if (rc)
  {
    xform.Mirror(m_basepoint, dir);
    rc = xform.IsValid();
  }
  return (rc) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CMirror2Xform : public CRhinoGetXform
{
public:
  CMirror2Xform(const ON_Line& line);
  BOOL CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

private:
  bool CalcCorners(CRhinoViewport& vp, const ON_3dPoint& point, ON_3dPoint corners[4]);

private:
  ON_Line m_line;
  ON_Plane m_plane;
  ON_3dPoint m_corners[4];
};

CMirror2Xform::CMirror2Xform(const ON_Line& line)
{
  m_line = line;
}

BOOL CMirror2Xform::CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& point, ON_Xform& xform)
{
  bool rc = CalcCorners(vp, point, m_corners);
  if (rc)
  {
    xform.Mirror(m_plane.origin, m_plane.Normal());
    rc = xform.IsValid();
  }
  return (rc) ? TRUE : FALSE;
}

void CMirror2Xform::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point)
{
  dp.DrawLine(m_corners[0], m_corners[1]);
  dp.DrawLine(m_corners[1], m_corners[2]);
  dp.DrawLine(m_corners[2], m_corners[3]);
  dp.DrawLine(m_corners[3], m_corners[0]);
  dp.DrawPoint(m_line.from);
  dp.DrawPoint(m_line.to);
  CRhinoGetXform::DynamicDraw(dp, point);
}

bool CMirror2Xform::CalcCorners(CRhinoViewport& vp, const ON_3dPoint& point, ON_3dPoint corners[4])
{
  UNREFERENCED_PARAMETER(vp);

  m_plane.CreateFromPoints(m_line.from, m_line.to, point);
  if (!m_plane.IsValid())
    return false;

  ON_2dPoint P, X, Y;
  m_plane.ClosestPointTo(point, &P.x, &P.y);
  m_plane.ClosestPointTo(m_line.to, &X.x, &X.y);
  Y.x = -X.y;
  Y.y = X.x;

  double S = ((P * Y) / (Y * Y)) * Y.y;

  // Is rectangle empty?
  if (fabs(S) < ON_SQRT_EPSILON)
    return false;

  ON_Interval x_interval(0.0, X.x);
  ON_Interval y_interval(0.0, S);

  // Make the lower coordinates at the origin and the higher ones positive
  if (!x_interval.IsIncreasing())
    x_interval.Swap();
  if (!y_interval.IsIncreasing())
    y_interval.Swap();

  // Make 3d points from the plane coordinates
  corners[0] = m_plane.PointAt(x_interval[0], y_interval[0]);
  corners[1] = m_plane.PointAt(x_interval[1], y_interval[0]);
  corners[2] = m_plane.PointAt(x_interval[1], y_interval[1]);
  corners[3] = m_plane.PointAt(x_interval[0], y_interval[1]);

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMirror : public CRhinoTransformCommand
{
public:
  CCommandSampleMirror() {}
  ~CCommandSampleMirror() {}
  UUID CommandUUID()
  {
    // {DFE87FFC-C771-4EE0-8746-0D17347BDA94}
    static const GUID SampleMirrorCommand_UUID =
    { 0xDFE87FFC, 0xC771, 0x4EE0, { 0x87, 0x46, 0x0D, 0x17, 0x34, 0x7B, 0xDA, 0x94 } };
    return SampleMirrorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMirror"; }
  const wchar_t* LocalCommandName() const { return L"SampleMirror"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMirror object
static class CCommandSampleMirror theSampleMirrorCommand;

CRhinoCommand::result CCommandSampleMirror::RunCommand(const CRhinoCommandContext& context)
{
  // Select objects to mirror
  CRhinoXformObjectList list;
  CRhinoCommand::result rc = SelectObjects(L"Select objects to mirror", list);
  if (rc != CRhinoCommand::success)
    return rc;

  // First point of mirror plane
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"First point of mirror plane");
  gp.GetPoint();
  rc = gp.CommandResult();
  if (rc != CRhinoCommand::success)
    return rc;

  ON_3dPoint point1 = gp.Point();

  // Second point of mirror plane
  CMirror1Xform gm1;
  gm1.SetCommandPrompt(L"Second point of mirror plane");
  gm1.AppendObjects(list);
  gm1.SetBasePoint(point1);
  gm1.DrawLineFromPoint(point1, TRUE);
  gm1.GetPoint();
  rc = gm1.CommandResult();
  if (rc != CRhinoCommand::success)
    return rc;

  ON_3dPoint point2 = gm1.Point();

  ON_Line line(point1, point2);
  if (line.Length() <= ON_SQRT_EPSILON)
    return CRhinoCommand::cancel;

  // Third point of mirror plane
  CMirror2Xform gm2(line);
  gm2.SetCommandPrompt(L"Third point of mirror plane");
  gm2.AppendObjects(list);
  gm2.SetBasePoint(point1);
  gm2.GetXform();
  rc = gm2.CommandResult();
  if (rc != CRhinoCommand::success)
    return rc;

  // Do mirror transformation
  CRhinoView* view = gm2.View();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_Xform xform;
  if (!gm2.CalculateTransform(view->ActiveViewport(), gm2.Point(), xform))
    return CRhinoCommand::failure;

  TransformObjects(context.m_doc, list, xform, TRUE, TRUE);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleMirror command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
