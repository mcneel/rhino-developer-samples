using System;
using System.Collections.Generic;
using Rhino.Geometry;

namespace SampleCsRhino
{
  public class SampleCsRhinoHelpers
  {
    /// <summary>
    /// Converts an object to a Rhino.Geometry.Point3d
    /// </summary>
    public static bool ConvertToPoint3d(object pointObj, ref Point3d point)
    {
      var rc = false;
      if (pointObj is Array point_arr && 3 == point_arr.Length)
      {
        try
        {
          if (2 == point_arr.Length)
          {
            point.X = Convert.ToDouble(point_arr.GetValue(0));
            point.Y = Convert.ToDouble(point_arr.GetValue(1));
            point.Z = 0.0;
          }
          else if (3 == point_arr.Length)
          {
            point.X = Convert.ToDouble(point_arr.GetValue(0));
            point.Y = Convert.ToDouble(point_arr.GetValue(1));
            point.Z = Convert.ToDouble(point_arr.GetValue(2));
          }
          rc = point.IsValid;
        }
        catch
        {
          // ignored
        }
      }
      return rc;
    }

    /// <summary>
    /// Converts an object to a list of Rhino.Geometry.Point3d 
    /// </summary>
    public static bool ConvertToPoint3dList(object pointsObj, ref List<Point3d> points)
    {
      var rc = false;
      var points_count = points.Count;
      if (pointsObj is Array point_arr)
      {
        for (var i = 0; i < point_arr.Length; i++)
        {
          var point = new Point3d();
          if (ConvertToPoint3d(point_arr.GetValue(i), ref point))
            points.Add(point);
        }
        rc = (points.Count - points_count > 0);
      }
      return rc;
    }
  }
}
