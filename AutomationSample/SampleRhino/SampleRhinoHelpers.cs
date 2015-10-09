using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SampleRhino
{
  class SampleRhinoHelpers
  {
    /// <summary>
    /// Converts an object to a Rhino.Geometry.Point3d
    /// </summary>
    static public bool ConvertToPoint3d(object pointObj, ref Rhino.Geometry.Point3d point)
    {
      bool rc = false;
      Array pointArr = pointObj as Array;
      if (null != pointArr && 3 == pointArr.Length)
      {
        try
        {
          if (2 == pointArr.Length)
          {
            point.X = Convert.ToDouble(pointArr.GetValue(0));
            point.Y = Convert.ToDouble(pointArr.GetValue(1));
            point.Z = 0.0;
          }
          else if (3 == pointArr.Length)
          {
            point.X = Convert.ToDouble(pointArr.GetValue(0));
            point.Y = Convert.ToDouble(pointArr.GetValue(1));
            point.Z = Convert.ToDouble(pointArr.GetValue(2));
          }
          rc = point.IsValid;
        }
        catch
        {
          // Suppress System.InvalidCastException
        }
      }
      return rc;
    }

    /// <summary>
    /// Converts an object to a list of Rhino.Geometry.Point3d 
    /// </summary>
    static public bool ConvertToPoint3dList(object pointsObj, ref List<Rhino.Geometry.Point3d> points)
    {
      bool rc = false;
      int pointsCount = points.Count;
      Array pointsArr = pointsObj as Array;
      if (null != pointsArr)
      {
        for (int i = 0; i < pointsArr.Length; i++)
        {
          Rhino.Geometry.Point3d point = new Rhino.Geometry.Point3d();
          if (SampleRhinoHelpers.ConvertToPoint3d(pointsArr.GetValue(i), ref point))
            points.Add(point);
        }
        rc = (points.Count - pointsCount > 0);
      }
      return rc;
    }
  }
}
