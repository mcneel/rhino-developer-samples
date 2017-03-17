#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleClassifyCurve command
//

class CCommandSampleClassifyCurve : public CRhinoCommand
{
public:
  CCommandSampleClassifyCurve() {}
  ~CCommandSampleClassifyCurve() {}
  UUID CommandUUID()
  {
    // {543D9BCB-33B5-46D2-B0D3-F493978EFC5B}
    static const GUID SampleClassifyCurveCommand_UUID =
    { 0x543D9BCB, 0x33B5, 0x46D2, { 0xB0, 0xD3, 0xF4, 0x93, 0x97, 0x8E, 0xFC, 0x5B } };
    return SampleClassifyCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleClassifyCurve"; }
  const wchar_t* LocalCommandName() { return L"SampleClassifyCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool IsLine(const ON_Curve* crv);
  bool IsArc(const ON_Curve* crv, double tol, bool& bCircle);
  bool IsEllipse(const ON_Curve* crv, double tol, bool& bEllipticalArc);
  bool IsPolyline(const ON_Curve* crv, double tol, double angtol, int& point_count, bool& bClosed, bool& bDistance, bool& bAngle, bool& bIntersect);
  bool IsNurbsCurve(const ON_Curve* crv);
  double AngleBetweenVectors(ON_3dVector v0, ON_3dVector v1);
};

// The one and only CCommandSampleClassifyCurve object
static class CCommandSampleClassifyCurve theSampleClassifyCurveCommand;

CRhinoCommand::result CCommandSampleClassifyCurve::RunCommand(const CRhinoCommandContext& context)
{
  // Select a curve to clasify
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to classify");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Get the geometry
  const CRhinoObjRef objref = go.Object(0);
  const ON_Curve* crv = objref.Curve();
  if (0 == crv)
    return CRhinoCommand::failure;

  // Obtain the document tolerances
  double tol = context.m_doc.AbsoluteTolerance();
  double angtol = context.m_doc.AngleToleranceRadians();

  // Is the curve a line?
  if (IsLine(crv))
  {
    RhinoApp().Print(L"Curve is a line.\n");
    return CRhinoCommand::success;
  }

  // Is the curve an arc or circle?
  {
    bool bCircle = false;
    if (IsArc(crv, tol, bCircle))
    {
      if (bCircle)
        RhinoApp().Print(L"Curve is a circle.\n");
      else
        RhinoApp().Print(L"Curve is an arc.\n");
      return CRhinoCommand::success;
    }
  }

  // Is the curve an ellipse or an elliptical arc?
  {
    bool bEllipticalArc = false;
    if (IsEllipse(crv, tol, bEllipticalArc))
    {
      if (bEllipticalArc)
        RhinoApp().Print(L"Curve is an elliptical arc.\n");
      else
        RhinoApp().Print(L"Curve is an ellipse.\n");
      return CRhinoCommand::success;
    }
  }

  // Is the curve some kind of polyline?
  int point_count = 0;     // number of vertices
  bool bClosed = false;    // curve is closed
  bool bDistance = false;  // segments are identical
  bool bAngle = false;     // angles are identical
  bool bIntersect = false; // curve self-intersects
  if (IsPolyline(crv, tol, angtol, point_count, bClosed, bDistance, bAngle, bIntersect))
  {
    // If the curve is closed and does not self-intersect, 
    // then it must be a polygon.
    if (bClosed && !bIntersect)
    {
      // If segment distances and angles are identical, 
      // then it must be a regular polygon.
      if (bDistance && bAngle && !bIntersect)
      {
        if (point_count == 3)
          RhinoApp().Print(L"Curve is an equilateral triangle.\n");
        else if (point_count == 4)
          RhinoApp().Print(L"Curve is a square.\n");
        else if (point_count == 5)
          RhinoApp().Print(L"Curve is a regular pentagon.\n");
        else if (point_count == 6)
          RhinoApp().Print(L"Curve is a regular hexagon.\n");
        else if (point_count == 7)
          RhinoApp().Print(L"Curve is a regular heptagon.\n");
        else if (point_count == 8)
          RhinoApp().Print(L"Curve is a regular octagon.\n");
        else if (point_count == 9)
          RhinoApp().Print(L"Curve is a regular nonagon.\n");
        else if (point_count == 10)
          RhinoApp().Print(L"Curve is a regular decagon.\n");
        else if (point_count == 11)
          RhinoApp().Print(L"Curve is an regular hendecagon.\n");
        else if (point_count == 12)
          RhinoApp().Print(L"Curve is an regular dodecagon.\n");
        else if (point_count == 13)
          RhinoApp().Print(L"Curve is an regular tridecagon.\n");
        else if (point_count == 14)
          RhinoApp().Print(L"Curve is an regular tetradecagon.\n");
        else if (point_count == 15)
          RhinoApp().Print(L"Curve is an regular pentadecagon.\n");
        else if (point_count == 16)
          RhinoApp().Print(L"Curve is an regular hexadecagon.\n");
        else if (point_count == 17)
          RhinoApp().Print(L"Curve is an regular heptadecagon.\n");
        else if (point_count == 18)
          RhinoApp().Print(L"Curve is an regular octadecagon.\n");
        else if (point_count == 19)
          RhinoApp().Print(L"Curve is an regular nonadecagon.\n");
        else if (point_count == 20)
          RhinoApp().Print(L"Curve is an regular icosagon.\n");
        else
          RhinoApp().Print(L"Curve is a regular %d-sided polygon.\n", point_count);
      }
      // If segment distances or angles are not identical, 
      // then it must be an irregular polygon.
      else if (!bIntersect)
      {
        if (point_count == 3)
        {
          // I'll leave searching for isosceles, scalene, acute, obtuse
          // and right triangles for somebody else...
          RhinoApp().Print(L"Curve is a triangle.\n");
        }
        else if (point_count == 4)
        {
          // I'll leave searching for parallelogram and trapezoids
          // for somebody else...
          if (bAngle)
            RhinoApp().Print(L"Curve is a rectangle.\n");
          else if (bDistance)
            RhinoApp().Print(L"Curve is a rhombus.\n");
          else
            RhinoApp().Print(L"Curve is a quadrilateral.\n");
        }
        else if (point_count == 5)
          RhinoApp().Print(L"Curve is an irregular pentagon.\n");
        else if (point_count == 6)
          RhinoApp().Print(L"Curve is an irregular hexagon.\n");
        else if (point_count == 7)
          RhinoApp().Print(L"Curve is an irregular heptagon.\n");
        else if (point_count == 8)
          RhinoApp().Print(L"Curve is an irregular octagon.\n");
        else if (point_count == 9)
          RhinoApp().Print(L"Curve is an irregular nonagon.\n");
        else if (point_count == 10)
          RhinoApp().Print(L"Curve is an irregular decagon.\n");
        else if (point_count == 11)
          RhinoApp().Print(L"Curve is an irregular hendecagon.\n");
        else if (point_count == 12)
          RhinoApp().Print(L"Curve is an irregular dodecagon.\n");
        else if (point_count == 13)
          RhinoApp().Print(L"Curve is an irregular tridecagon.\n");
        else if (point_count == 14)
          RhinoApp().Print(L"Curve is an irregular tetradecagon.\n");
        else if (point_count == 15)
          RhinoApp().Print(L"Curve is an irregular pentadecagon.\n");
        else if (point_count == 16)
          RhinoApp().Print(L"Curve is an irregular hexadecagon.\n");
        else if (point_count == 17)
          RhinoApp().Print(L"Curve is an irregular heptadecagon.\n");
        else if (point_count == 18)
          RhinoApp().Print(L"Curve is an irregular octadecagon.\n");
        else if (point_count == 19)
          RhinoApp().Print(L"Curve is an irregular nonadecagon.\n");
        else if (point_count == 20)
          RhinoApp().Print(L"Curve is an irregular icosagon.\n");
        else
          RhinoApp().Print(L"Curve is a irregular %d-sided polygon.\n", point_count);
      }
    }
    else
      RhinoApp().Print(L"Curve is a polyline.\n");
    return CRhinoCommand::success;
  }

  // Is the curve a NURBS curve?
  if (IsNurbsCurve(crv))
  {
    RhinoApp().Print(L"Curve is a NURBS curve.\n");
    return CRhinoCommand::success;
  }

  // Give up...
  RhinoApp().Print(L"Curve is unclassified.\n");


  return CRhinoCommand::success;
}

bool CCommandSampleClassifyCurve::IsLine(const ON_Curve* crv)
{
  if (0 == crv)
    return false;

  // Is the curve a line curve?
  const ON_LineCurve* line_crv = ON_LineCurve::Cast(crv);
  if (line_crv)
    return true;

  // Is the curve a polyline that looks like a line?
  const ON_PolylineCurve* pline_crv = ON_PolylineCurve::Cast(crv);
  if (pline_crv && pline_crv->m_pline.Count() == 2)
    return true;

  // Is the curve a polycurve that looks like a line?
  const ON_PolyCurve* poly_crv = ON_PolyCurve::Cast(crv);
  if (poly_crv && poly_crv->IsPolyline() == 2)
    return true;

  // Is the curve a NURBS curve that looks like a line?
  const ON_NurbsCurve* nurb_crv = ON_NurbsCurve::Cast(crv);
  if (nurb_crv && nurb_crv->IsPolyline() == 2)
    return true;

  return false;
}

bool CCommandSampleClassifyCurve::IsArc(const ON_Curve* crv, double tol, bool& bCircle)
{
  if (0 == crv)
    return false;

  // Is the curve an arc curve?
  const ON_ArcCurve* arc_crv = ON_ArcCurve::Cast(crv);
  if (arc_crv)
  {
    bCircle = arc_crv->IsCircle();
    return true;
  }

  // Is the curve a polycurve that looks like an arc?
  const ON_PolyCurve* poly_crv = ON_PolyCurve::Cast(crv);
  if (poly_crv)
  {
    ON_Arc arc;
    if (poly_crv->IsArc(0, &arc, tol))
    {
      bCircle = arc.IsCircle();
      return true;
    }
  }

  // Is the curve an ellipse that looks like an arc?
  ON_Ellipse ellipse;
  if (crv->IsEllipse(0, &ellipse, tol))
  {
    if (ellipse.IsCircle())
    {
      bCircle = crv->IsClosed() ? true : false;
      return true;
    }
  }

  // Is the curve a NURBS curve that looks like an arc?
  const ON_NurbsCurve* nurb_crv = ON_NurbsCurve::Cast(crv);
  if (nurb_crv)
  {
    ON_Arc arc;
    if (nurb_crv->IsArc(0, &arc, tol))
    {
      bCircle = arc.IsCircle();
      return true;
    }
  }

  return false;
}

bool CCommandSampleClassifyCurve::IsEllipse(const ON_Curve* crv, double tol, bool& bEllipticalArc)
{
  if (0 == crv)
    return false;

  // If curve is a polycure, get the first curve segment
  const ON_PolyCurve* poly_crv = ON_PolyCurve::Cast(crv);
  if (poly_crv)
  {
    if (poly_crv->Count() == 1)
      crv = poly_crv->SegmentCurve(0);
    else
      return false;
  }

  ON_Ellipse ellipse;
  if (crv->IsEllipse(0, &ellipse, tol))
  {
    if (!ellipse.IsCircle())
    {
      bEllipticalArc = !crv->IsClosed() ? true : false;
      return true;
    }
  }

  return false;
}

bool CCommandSampleClassifyCurve::IsPolyline(const ON_Curve* crv, double tol, double angtol, int& point_count,
  bool& bClosed, bool& bDistance, bool& bAngle, bool& bIntersect)
{
  if (0 == crv)
    return false;

  ON_3dPointArray pline_points;
  int i = 0;

  const ON_PolylineCurve* pline_crv = ON_PolylineCurve::Cast(crv);
  const ON_PolyCurve* poly_crv = ON_PolyCurve::Cast(crv);
  const ON_NurbsCurve* nurb_crv = ON_NurbsCurve::Cast(crv);

  // Is the curve a polyline?
  if (pline_crv)
  {
    if (pline_crv->m_pline.Count() <= 2)
      return false;

    pline_points = pline_crv->m_pline;
  }
  // Is the curve a polycurve that looks like a polyline?
  else if (poly_crv)
  {
    if (poly_crv->IsPolyline(&pline_points) <= 2)
      return true;
  }
  // Is the curve a NURBS curve that looks like a line?
  else if (nurb_crv)
  {
    if (nurb_crv->IsPolyline(&pline_points) <= 2)
      return false;
  }
  else
    return false;

  // Is the curve closed?
  bClosed = (crv->IsClosed() ? true : false);

  // Get the vertex count
  point_count = (bClosed ? pline_points.Count() - 1 : pline_points.Count());

  // Test for self-intersection
  ON_SimpleArray<ON_X_EVENT> x;
  bIntersect = (crv->IntersectSelf(x) > 0 ? true : false);

  // If the curve is closed, no reason to continue...
  if (!bClosed)
    return true;

  // Test if the distance between each point is identical
  double distance = 0.0;
  for (i = 1; i < pline_points.Count(); i++)
  {
    ON_3dPoint p0 = pline_points[i - 1];
    ON_3dPoint p1 = pline_points[i];
    ON_3dVector v = p0 - p1;
    double d = v.Length();
    if (i == 1)
    {
      distance = d;
      continue;
    }
    else if (fabs(distance - d) < tol)
      continue;
    else
    {
      distance = ON_UNSET_VALUE;
      break;
    }
  }

  // Set return value
  bDistance = (distance == ON_UNSET_VALUE ? false : true);

  // Test if the angle between each point is identical
  double angle = 0.0;
  for (i = 1; i < pline_points.Count() - 1; i++)
  {
    ON_3dPoint p0 = pline_points[i - 1];
    ON_3dPoint p1 = pline_points[i];
    ON_3dPoint p2 = pline_points[i + 1];

    ON_3dVector v0 = p1 - p0;
    v0.Unitize();

    ON_3dVector v1 = p1 - p2;
    v1.Unitize();

    double a = AngleBetweenVectors(v0, v1);
    if (i == 1)
    {
      angle = a;
      continue;
    }
    else if (fabs(angle - a) < angtol)
      continue;
    else
    {
      angle = ON_UNSET_VALUE;
      break;
    }
  }

  // Set return value
  bAngle = (angle == ON_UNSET_VALUE ? false : true);

  return true;
}

bool CCommandSampleClassifyCurve::IsNurbsCurve(const ON_Curve* crv)
{
  if (0 == crv)
    return false;

  const ON_NurbsCurve* nurb_crv = ON_NurbsCurve::Cast(crv);
  if (nurb_crv)
    return true;

  return false;
}

double CCommandSampleClassifyCurve::AngleBetweenVectors(ON_3dVector v0, ON_3dVector v1)
{
  v0.Unitize();
  v1.Unitize();
  double dot = RHINO_CLAMP(ON_DotProduct(v0, v1), -1.0, 1.0);
  return acos(dot);
}

//
// END SampleClassifyCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
