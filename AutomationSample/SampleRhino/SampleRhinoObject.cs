using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Rhino;

namespace SampleRhino
{
  [System.Runtime.InteropServices.ComVisible(true)]
  public class SampleRhinoObject
  {
    /// <summary>
    /// Return a sample integer
    /// </summary>
    public int GetInteger()
    {
      return 24;
    }

    /// <summary>
    /// Return a sample double
    /// </summary>
    public double GetDouble()
    {
      // Returns a double-precision number
      return System.Math.PI;
    }

    /// <summary>
    /// Return a sample string
    /// </summary>
    public string GetString()
    {
      return "Hello Rhino!";
    }

    /// <summary>
    /// Return a sample 3-D point
    /// </summary>
    public object GetPoint()
    {
      ArrayList pt = new ArrayList(3);
      pt.Add(2.0);
      pt.Add(1.0);
      pt.Add(0.0);
      return pt.ToArray();
    }

    /// <summary>
    /// Returns a sample array of 3-D points
    /// </summary>
    public object GetPoints()
    {
      ArrayList pts = new ArrayList();

      ArrayList p0 = new ArrayList();
      p0.Add(0.0);
      p0.Add(0.0);
      p0.Add(0.0);
      pts.Add(p0.ToArray());

      ArrayList p1 = new ArrayList();
      p1.Add(10.0);
      p1.Add(0.0);
      p1.Add(0.0);
      pts.Add(p1.ToArray());

      ArrayList p2 = new ArrayList();
      p2.Add(10.0);
      p2.Add(10.0);
      p2.Add(0.0);
      pts.Add(p2.ToArray());

      ArrayList p3 = new ArrayList();
      p3.Add(0.0);
      p3.Add(10.0);
      p3.Add(0.0);
      pts.Add(p3.ToArray());

      return pts.ToArray();
    }

    /// <summary>
    /// Add a 3-D point to the document
    /// </summary>
    public object AddPoint(object pointObj)
    {
      Rhino.Geometry.Point3d point = new Rhino.Geometry.Point3d();
      if (SampleRhinoHelpers.ConvertToPoint3d(pointObj, ref point))
      {
        Rhino.RhinoDoc doc = Rhino.RhinoDoc.ActiveDoc;
        if (null != doc)
        {
          System.Guid objectId = doc.Objects.AddPoint(point);
          if (!objectId.Equals(System.Guid.Empty))
          {
            doc.Views.Redraw();
            return objectId.ToString();
          }
        }
      }
      return null;
    }

    /// <summary>
    /// Add an array of 3-D points to the document
    /// </summary>
    public object AddPoints(object pointsObj)
    {
      List<Rhino.Geometry.Point3d> points = new List<Rhino.Geometry.Point3d>();
      if (SampleRhinoHelpers.ConvertToPoint3dList(pointsObj, ref points))
      {
        Rhino.RhinoDoc doc = Rhino.RhinoDoc.ActiveDoc;
        if (null != doc)
        {
          ArrayList objectIds = new ArrayList();
          for (int i = 0; i < points.Count(); i++)
          {
            System.Guid objectId = doc.Objects.AddPoint(points[i]);
            if (!objectId.Equals(System.Guid.Empty))
              objectIds.Add(objectId.ToString());
          }
          if (objectIds.Count > 0)
          {
            doc.Views.Redraw();
            return objectIds.ToArray();
          }
        }
      }
      return null;
    }
  }
}
