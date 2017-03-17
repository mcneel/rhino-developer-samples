#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOffsetCurve command
//

#pragma region SampleOffsetCurve command

class CCommandSampleOffsetCurve : public CRhinoCommand
{
public:
  CCommandSampleOffsetCurve();
  ~CCommandSampleOffsetCurve() {}
  UUID CommandUUID()
  {
    // {55054530-834-456C-BE03-81D6D6EBF475}
    static const GUID SampleOffsetCurveCommand_UUID =
    { 0x55054530, 0x834, 0x456C, { 0xBE, 0x03, 0x81, 0xD6, 0xD6, 0xEB, 0xF4, 0x75 } };
    return SampleOffsetCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleOffsetCurve"; }
  const wchar_t* LocalCommandName() const { return L"SampleOffsetCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  int m_corner;
  double m_distance;
};

// The one and only CCommandSampleOffsetCurve object
static class CCommandSampleOffsetCurve theSampleOffsetCurveCommand;

CCommandSampleOffsetCurve::CCommandSampleOffsetCurve()
{
  m_corner = 1;
  m_distance = 1.0;
}

CRhinoCommand::result CCommandSampleOffsetCurve::RunCommand(const CRhinoCommandContext& context)
{
  // Corner options
  CRhinoCommandOptionValue list[5] =
  {
    RHCMDOPTVALUE(L"None"),
    RHCMDOPTVALUE(L"Sharp"),
    RHCMDOPTVALUE(L"Round"),
    RHCMDOPTVALUE(L"Smooth"),
    RHCMDOPTVALUE(L"Chamfer")
  };

  // Get curves to offset
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to offset");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableSubObjectSelect(FALSE);
  for (;;)
  {
    go.ClearCommandOptions();
    /*int distance =*/ go.AddCommandOptionNumber(RHCMDOPTNAME(L"Distance"), &m_distance, 0, 0, 0.0);
    int corner = go.AddCommandOptionList(RHCMDOPTNAME(L"Corner"), 5, list, m_corner);

    CRhinoGet::result res = go.GetObjects(1, 0);

    if (res == CRhinoGet::number)
    {
      m_distance = fabs(go.Number());
      continue;
    }
    else if (res == CRhinoGet::option)
    {
      if (corner == go.OptionIndex() && go.Option())
        m_corner = go.Option()->m_list_option_current;
      continue;
    }

    break;
  }

  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();


  // Get offset point
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Side to offset");
  go.AcceptNumber();
  for (;;)
  {
    gp.ClearCommandOptions();
    /*int distance =*/ gp.AddCommandOptionNumber(RHCMDOPTNAME(L"Distance"), &m_distance, 0, 0, 0.0);
    int corner = gp.AddCommandOptionList(RHCMDOPTNAME(L"Corner"), 5, list, m_corner);

    CRhinoGet::result res = gp.GetPoint();

    if (res == CRhinoGet::number)
    {
      m_distance = fabs(go.Number());
      continue;
    }
    else if (res == CRhinoGet::option)
    {
      if (corner == go.OptionIndex() && go.Option())
        m_corner = go.Option()->m_list_option_current;
      continue;
    }

    break;
  }

  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();


  // Validate offset point view
  CRhinoView* view = gp.View();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_3dVector offset_normal = view->ActiveViewport().ConstructionPlane().m_plane.Normal();
  ON_3dPoint offset_point = gp.Point();


  // Initialize offset point on each curve, and find closed curve closest to offset point
  ON_SimpleArray<ON_3dPoint> points;
  ON_SimpleArray<ON_Plane> planes;
  ON_Plane invalid_plane;
  invalid_plane.origin = ON_UNSET_POINT;

  int i, j, closest_closed_crv = -1;
  double closest_dist = ON_DBL_MAX;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Curve* crv = go.Object(i).Curve();
    if (0 == crv)
      return CRhinoCommand::failure;

    points.Append(offset_point);

    ON_Plane plane;
    if (crv->IsClosed() && crv->IsPlanar(&plane))
    {
      planes.Append(plane);

      double t = ON_UNSET_VALUE;
      if (crv->GetClosestPoint(offset_point, &t))
      {
        ON_3dPoint pt = crv->PointAt(t);
        double dist = pt.DistanceTo(offset_point);
        if (dist < closest_dist)
        {
          closest_dist = dist;
          closest_closed_crv = i;
        }
      }
    }
    else
      planes.Append(invalid_plane);
  }


  // Update offset point on other closed curves if the point is inside the closest closed curve
  if (closest_closed_crv > -1)
  {
    const ON_Curve* crv = go.Object(closest_closed_crv).Curve();
    if (0 == crv)
      return CRhinoCommand::failure;

    ON_Plane test_plane = planes[closest_closed_crv];
    ON_3dPoint test_pt = test_plane.ClosestPointTo(offset_point);

    if (1 == RhinoPointInPlanarClosedCurve(test_pt, *crv, test_plane, context.m_doc.AbsoluteTolerance()))
    {
      ON_RandomNumberGenerator rng;
      for (i = 0; i < go.ObjectCount(); i++)
      {
        if (i == closest_closed_crv)
          continue;

        if (planes[i].IsValid())
        {
          ON_BoundingBox bbox;
          crv->GetTightBoundingBox(bbox);

          // Cook up a point inside of crv.
          // Try random points inside curve bounding box until we get one
          for (j = 0; j < 1000; j++)
          {
            ON_3dPoint pt;
            pt.x = rng.RandomDouble(bbox.m_min.x, bbox.m_max.x);
            pt.y = rng.RandomDouble(bbox.m_min.y, bbox.m_max.y);
            pt.z = rng.RandomDouble(bbox.m_min.z, bbox.m_max.z);

            pt = planes[i].ClosestPointTo(pt);
            if (1 == RhinoPointInPlanarClosedCurve(pt, *crv, planes[i], context.m_doc.AbsoluteTolerance()))
            {
              points[i] = pt;
              break;
            }
          }
        }
      }
    }
  }


  // Offset curves and add to document
  double tolerance = context.m_doc.AbsoluteTolerance();
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Curve* crv = go.Object(closest_closed_crv).Curve();
    if (0 == crv)
      return CRhinoCommand::failure;

    ON_SimpleArray<ON_Curve*> curves_out;
    if (RhinoOffsetCurve(*crv, m_distance, points[i], offset_normal, m_corner, tolerance, curves_out))
    {
      for (j = 0; j < curves_out.Count(); j++)
      {
        CRhinoCurveObject* obj = new CRhinoCurveObject();
        obj->SetCurve(curves_out[j]);
        context.m_doc.AddObject(obj);
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleOffsetCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
