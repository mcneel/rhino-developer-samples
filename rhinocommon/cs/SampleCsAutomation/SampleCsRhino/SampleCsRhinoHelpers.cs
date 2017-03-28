using System;
using System.Collections.Generic;

namespace SampleCsRhino
{
  public class SampleCsRhinoHelpers
  {
    /// <summary>
    /// Converts an object to a Rhino.Geometry.Point3d
    /// </summary>
    public static bool ConvertToPoint3d(object pointObj, ref Rhino.Geometry.Point3d point)
    {
      var rc = false;
      var point_arr = pointObj as Array;
      if (null != point_arr && 3 == point_arr.Length)
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
          // Suppress System.InvalidCastException
        }
      }
      return rc;
    }

    /// <summary>
    /// Converts an object to a list of Rhino.Geometry.Point3d 
    /// </summary>
    public static bool ConvertToPoint3dList(object pointsObj, ref List<Rhino.Geometry.Point3d> points)
    {
      var rc = false;
      var points_count = points.Count;
      var point_arr = pointsObj as Array;
      if (null != point_arr)
      {
        for (int i = 0; i < point_arr.Length; i++)
        {
          Rhino.Geometry.Point3d point = new Rhino.Geometry.Point3d();
          if (ConvertToPoint3d(point_arr.GetValue(i), ref point))
            points.Add(point);
        }
        rc = (points.Count - points_count > 0);
      }
      return rc;
    }
  }
}
