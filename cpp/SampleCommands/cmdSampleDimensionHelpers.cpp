#include "stdafx.h"

/*
Description:
  Creates an annotation leader.
Parameters:
  plane  - [in] The plane on which the leader lies. 
                Often this is the active construction plane.
  points – [in] An array of 2 or more ordered points that define the leader.
  text   – [in] The leader's text (optional).
Returns:
  A pointer to a CRhinoAnnotationLeader object if successful.
  NULL if not successful.
Remarks:
  Memory for the CRhinoAnnotationLeader object is allocated and becomes
  the responsibility of the caller.
*/
CRhinoAnnotationLeader* RhinoCreateLeader(
  const ON_Plane& plane, 
  const ON_3dPointArray& points, 
  const wchar_t* text
  )
{
  CRhinoAnnotationLeader* obj = 0;

  if (plane.IsValid() && points.Count() >= 2)
  {
    ON_Leader2 leader;
    leader.SetPlane(plane);

    for(int i = 0; i < points.Count(); i++)
    {
      ON_2dPoint p2;
      if (leader.m_plane.ClosestPointTo(points[i], &p2.x, &p2.y))
      {
        if (leader.m_points.Count() < 1 || p2.DistanceTo(*leader.m_points.Last()) > ON_SQRT_EPSILON)
          leader.m_points.Append(p2);
      }
    }

    if (leader.m_points.Count() > 1)
    {
      obj = new CRhinoAnnotationLeader();
      if (obj)
      {
        obj->SetAnnotation(leader);
        if (text && text[0])
          obj->SetTextString(text);
      }
    }
  }

  return obj;
}


/*
Description:
  Creates a linear dimension based on the current dimension style.
Parameters:
  plane  - [in] The plane on which the dimension lies. 
                Often this is the active construction plane.
  origin – [in] The origin point.
  offset – [in] The offset point.
  point  – [in] The location point of the dimension line. 
Returns:
  A pointer to a CRhinoLinearDimension object if successful.
  NULL if not successful.
Remarks:
  Memory for the CRhinoLinearDimension object is allocated and becomes
  the responsibility of the caller.
*/
CRhinoLinearDimension* RhinoCreateLinearDimension(
  const ON_Plane& plane, 
  const ON_3dPoint& origin, 
  const ON_3dPoint& offset, 
  const ON_3dPoint& point
  )
{
  CRhinoLinearDimension* obj = 0;

  if (plane.IsValid() && origin.IsValid() && offset.IsValid() && point.IsValid())
  {
    obj = new CRhinoLinearDimension();
    if (obj)
    {
      ON_Plane dim_plane(plane);
      dim_plane.SetOrigin(origin);
      obj->SetPlane(dim_plane);

      // origin
      obj->SetPoint(0, ON_2dPoint(0,0));
 
      // offset
      ON_2dPoint p2;
      dim_plane.ClosestPointTo(offset, &p2.x, &p2.y);
      obj->SetPoint(2, p2);
 
      // dimension line point
      dim_plane.ClosestPointTo(point, &p2.x, &p2.y);
      obj->UpdateDimPoints(p2);
      obj->UpdateText();
    }
  }

  return obj;
}


/*
Description:
  Creates an aligned linear dimension based on the current dimension style.
Parameters:
  plane  - [in] The plane on which the dimension lies. 
                Often this is the active construction plane.
  origin – [in] The origin point.
  offset – [in] The offset point.
  point  – [in] The location point of the dimension line. 
Returns:
  A pointer to a CRhinoLinearDimension object if successful.
  NULL if not successful.
Remarks:
  Memory for the CRhinoLinearDimension object is allocated and becomes
  the responsibility of the caller.
*/
CRhinoLinearDimension* RhinoCreateAlignedDimension(
  const ON_Plane& plane, 
  const ON_3dPoint& origin, 
  const ON_3dPoint& offset, 
  const ON_3dPoint& point
  )
{
  CRhinoLinearDimension* obj = 0;

  if (plane.IsValid() && origin.IsValid() && offset.IsValid() && point.IsValid())
  {
    obj = new CRhinoLinearDimension();
    if (obj)
    {
      obj->SetAligned(TRUE);

      // Calculate rotation angle
      ON_Plane dim_plane(plane);
      dim_plane.SetOrigin( origin );

      double s[2], t[2];
      dim_plane.ClosestPointTo(origin, &s[0], &t[0]);
      dim_plane.ClosestPointTo(offset, &s[1], &t[1]);
      
      ON_2dVector angle(s[1]-s[0], t[1]-t[0]);
      angle.Unitize();

      dim_plane.Rotate(angle.y, angle.x, dim_plane.Normal());
      obj->SetPlane( dim_plane );
 
      // origin
      obj->SetPoint(0, ON_2dPoint(0,0));

      // offset
      ON_2dPoint p2;
      dim_plane.ClosestPointTo(offset, &p2.x, &p2.y);
      obj->SetPoint(2, p2);
 
      // dimension line point
      dim_plane.ClosestPointTo(point, &p2.x, &p2.y);
      obj->UpdateDimPoints(p2);
      obj->UpdateText();
    }
  }

  return obj;
}


/*
Description:
  Creates a radial dimension based on the current dimension style.
Parameters:
  plane   - [in] The plane on which the dimension lies. 
                 Often this is the active construction plane.
  center  – [in] The center point.
  arrow   – [in] The arrowhead point. The distance between the center point and
                 the arrowhead point defines the radius or diameter.
  knee    – [in] The knee point, which is the intermediate point between the
                 arrowhead point and the position of the text. 
  bRadius – [in] True creates a radius dimension, 
                 False creates a diameter dimension.
Returns:
  A pointer to a CRhinoRadialDimension object if successful.
  NULL if not successful.
Remarks:
  Memory for the CRhinoRadialDimension object is allocated and becomes
  the responsibility of the caller.
*/
CRhinoRadialDimension* RhinoCreateRadialDimension(
  const ON_Plane& plane, 
  const ON_3dPoint& center, 
  const ON_3dPoint& arrow, 
  const ON_3dPoint& knee,
  bool bRadius
  )
{
  CRhinoRadialDimension* obj = 0;

  if (plane.IsValid() && center.IsValid() && arrow.IsValid() && knee.IsValid())
  {
    obj = new CRhinoRadialDimension();
    if (obj)
    {
      if (bRadius)
      {
        obj->m_radial_dimension.m_type = ON::dtDimRadius;
        obj->SetUserText(obj->m_radial_dimension.DefaultRadiusText());
      }
      else
      {
        obj->m_radial_dimension.m_type = ON::dtDimDiameter;
        obj->SetUserText(obj->m_radial_dimension.DefaultDiameterText());
      }

      ON_Plane dim_plane(plane);
      dim_plane.SetOrigin(center);
 
      ON_2dPoint arrow_pt;
      dim_plane.ClosestPointTo(arrow, &arrow_pt.x, &arrow_pt.y);
 
      ON_2dPoint knee_pt;
      dim_plane.ClosestPointTo(knee, &knee_pt.x, &knee_pt.y);

      obj->SetPlane( plane );
      obj->UpdateDimPoints(arrow_pt, knee_pt);
      obj->UpdateText();
    }
  }

  return obj;
}
