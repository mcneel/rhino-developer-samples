using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace SampleCsRhino
{
  [System.Runtime.InteropServices.ComVisible(true)]
  public class SampleCsRhinoObject
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
      var pt = new ArrayList(3) {2.0, 1.0, 0.0};
      return pt.ToArray();
    }

    /// <summary>
    /// Returns a sample array of 3-D points
    /// </summary>
    public object GetPoints()
    {
      var pts = new ArrayList();

      var p0 = new ArrayList {0.0, 0.0, 0.0};
      pts.Add(p0.ToArray());

      var p1 = new ArrayList {10.0, 0.0, 0.0};
      pts.Add(p1.ToArray());

      var p2 = new ArrayList {10.0, 10.0, 0.0};
      pts.Add(p2.ToArray());

      var p3 = new ArrayList {0.0, 10.0, 0.0};
      pts.Add(p3.ToArray());

      return pts.ToArray();
    }

    /// <summary>
    /// Add a 3-D point to the document
    /// </summary>
    public object AddPoint(object pointObj)
    {
      var point = new Rhino.Geometry.Point3d();
      if (SampleCsRhinoHelpers.ConvertToPoint3d(pointObj, ref point))
      {
        var doc = Rhino.RhinoDoc.ActiveDoc;
        if (null != doc)
        {
          var object_id = doc.Objects.AddPoint(point);
          if (!object_id.Equals(System.Guid.Empty))
          {
            doc.Views.Redraw();
            return object_id.ToString();
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
      var points = new List<Rhino.Geometry.Point3d>();
      if (SampleCsRhinoHelpers.ConvertToPoint3dList(pointsObj, ref points))
      {
        var doc = Rhino.RhinoDoc.ActiveDoc;
        if (null != doc)
        {
          var object_ids = new ArrayList();
          for (var i = 0; i < points.Count(); i++)
          {
            var object_id = doc.Objects.AddPoint(points[i]);
            if (!object_id.Equals(System.Guid.Empty))
              object_ids.Add(object_id.ToString());
          }
          if (object_ids.Count > 0)
          {
            doc.Views.Redraw();
            return object_ids.ToArray();
          }
        }
      }
      return null;
    }

    /// <summary>
    /// Runs a Rhino command script
    /// </summary>
    public bool RunScript(string script, bool echo)
    {
      script = script.TrimStart('!');
      var rc = Rhino.RhinoApp.RunScript(script, echo);
      return rc;
    }
  }
}
