using System;
using Rhino;
using Rhino.Commands;
using Rhino.Input;
using Rhino.Geometry;
using Rhino.DocObjects;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace SampleCsCommands
{
  public class SampleCsContour : Command
  {
    public override string EnglishName => "SampleCsContour";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType filter = ObjectType.Brep | ObjectType.Extrusion | ObjectType.Mesh;
      var rc = RhinoGet.GetMultipleObjects("Select objects to contour", false, filter, out var objrefs);
      if (rc != Result.Success)
        return rc;

      if (objrefs == null || objrefs.Length < 1)
        return Result.Failure;

      rc = RhinoGet.GetPoint("Contour plane base point", false, out var base_point);
      if (rc != Result.Success)
        return rc;

      var gp = new Rhino.Input.Custom.GetPoint();
      gp.SetCommandPrompt("Direction perpendicular to contour planes");
      gp.DrawLineFromPoint(base_point, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var end_point = gp.Point();

      var interval = 1.0;
      rc = RhinoGet.GetNumber("Distance between contours", true, ref interval, 0.001, 10000);
      if (rc != Result.Success)
        return rc;

      var bounds = BoundingBox.Unset;
      var geometries = new List<GeometryBase>();
      foreach (var objref in objrefs)
      {
        var geometry = objref.Geometry();
        if (geometry is Extrusion)
          geometry = objref.Brep();
        geometries.Add(geometry);
        var bbox = geometry.GetBoundingBox(false);
        bounds.Union(bbox);
      }

      var normal = end_point - base_point;
      normal.Unitize();
      var curplane = new Plane(base_point, normal);
      if (!curplane.DistanceTo(bounds, out var min_t, out var max_t))
        return Result.Failure;

      min_t -= interval;
      max_t += interval;
      min_t = Math.Floor(min_t / interval);
      max_t = Math.Ceiling(max_t / interval);
      var tolerance = doc.ModelAbsoluteTolerance;

      var tasks = new List<Task<Curve[]>>();
      var gc = new Rhino.Input.Custom.GetCancel();
      for (var t = min_t; t <= max_t; t += 1.0)
      {
        var offset = t * interval;
        var point = base_point + normal * offset;
        var plane = new Plane(point, normal);
        foreach (var geom in geometries)
        {
          var geom1 = geom;
          var task = Task.Run(() => Section(plane, geom1, tolerance), gc.Token);
          tasks.Add(task);
        }
      }
      gc.TaskCompleted += OnTaskCompleted;
      rc = gc.WaitAll(tasks, doc);
      return rc;
    }

    private void OnTaskCompleted(object sender, Rhino.Input.Custom.TaskCompleteEventArgs e)
    {
      if (e.Task is Task<Curve[]> t && t.Status == TaskStatus.RanToCompletion)
      {
        var curves = t.Result;
        if (curves != null && curves.Length > 0)
        {
          foreach (var curve in curves)
            e.Doc.Objects.AddCurve(curve);
          e.Redraw = true;
        }
      }
    }

    private static Curve[] Section(Plane plane, GeometryBase geometry, double tolerance)
    {
      var rc = new Curve[0];
      if (geometry is Brep brep)
      {
        Rhino.Geometry.Intersect.Intersection.BrepPlane(brep, plane, tolerance, out rc, out var pts);
        return rc;
      }

      if (geometry is Mesh mesh)
      {
        var polylines = Rhino.Geometry.Intersect.Intersection.MeshPlane(mesh, plane);
        if (polylines != null)
        {
          rc = new Curve[polylines.Length];
          for (int i = 0; i < polylines.Length; i++)
          {
            rc[i] = new PolylineCurve(polylines[i]);
          }
        }
      }
      return rc;
    }
  }
}