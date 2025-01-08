using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using System;
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
      Result rc = RhinoGet.GetMultipleObjects("Select objects to contour", false, filter, out ObjRef[] objrefs);
      if (rc != Result.Success)
        return rc;

      if (objrefs == null || objrefs.Length < 1)
        return Result.Failure;

      rc = RhinoGet.GetPoint("Contour plane base point", false, out Point3d base_point);
      if (rc != Result.Success)
        return rc;

      Rhino.Input.Custom.GetPoint gp = new Rhino.Input.Custom.GetPoint();
      gp.SetCommandPrompt("Direction perpendicular to contour planes");
      gp.DrawLineFromPoint(base_point, false);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      Point3d end_point = gp.Point();

      double interval = 1.0;
      rc = RhinoGet.GetNumber("Distance between contours", true, ref interval, 0.001, 10000);
      if (rc != Result.Success)
        return rc;

      BoundingBox bounds = BoundingBox.Unset;
      List<GeometryBase> geometries = new List<GeometryBase>();
      foreach (ObjRef objref in objrefs)
      {
        GeometryBase geometry = objref.Geometry();
        if (geometry is Extrusion)
          geometry = objref.Brep();
        geometries.Add(geometry);
        BoundingBox bbox = geometry.GetBoundingBox(false);
        bounds.Union(bbox);
      }

      Vector3d normal = end_point - base_point;
      normal.Unitize();
      Plane curplane = new Plane(base_point, normal);
      if (!curplane.DistanceTo(bounds, out double min_t, out double max_t))
        return Result.Failure;

      min_t -= interval;
      max_t += interval;
      min_t = Math.Floor(min_t / interval);
      max_t = Math.Ceiling(max_t / interval);
      double tolerance = doc.ModelAbsoluteTolerance;

      List<Task<Curve[]>> tasks = new List<Task<Curve[]>>();
      Rhino.Input.Custom.GetCancel gc = new Rhino.Input.Custom.GetCancel();
      for (double t = min_t; t <= max_t; t += 1.0)
      {
        double offset = t * interval;
        Point3d point = base_point + normal * offset;
        Plane plane = new Plane(point, normal);
        foreach (GeometryBase geom in geometries)
        {
          GeometryBase geom1 = geom;
          Task<Curve[]> task = Task.Run(() => Section(plane, geom1, tolerance), gc.Token);
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
        Curve[] curves = t.Result;
        if (curves != null && curves.Length > 0)
        {
          foreach (Curve curve in curves)
            e.Doc.Objects.AddCurve(curve);
          e.Redraw = true;
        }
      }
    }

    private static Curve[] Section(Plane plane, GeometryBase geometry, double tolerance)
    {
      Curve[] rc = new Curve[0];
      if (geometry is Brep brep)
      {
        Rhino.Geometry.Intersect.Intersection.BrepPlane(brep, plane, tolerance, out rc, out Point3d[] pts);
        return rc;
      }

      if (geometry is Mesh mesh)
      {
        Polyline[] polylines = Rhino.Geometry.Intersect.Intersection.MeshPlane(mesh, plane);
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