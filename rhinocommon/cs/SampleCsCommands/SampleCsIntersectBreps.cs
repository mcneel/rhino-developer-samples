using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;
using System;

namespace SampleCsCommands
{
  public class SampleCsIntersectBreps : Command
  {
    public override string EnglishName => "SampleCsIntersectBreps";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select two surfaces or polysurfaces to intersect");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go.SubObjectSelect = false;
      go.GetMultiple(2, 2);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      Brep brep0 = go.Object(0).Brep();
      Brep brep1 = go.Object(1).Brep();
      if (null == brep0 || null == brep1)
        return Result.Failure;

      bool rc = Intersection.BrepBrep(brep0, brep1, doc.ModelAbsoluteTolerance, out Curve[] curves, out Point3d[] points);
      if (!rc)
      {
        RhinoApp.WriteLine("Unable to intersect two Breps.");
        return Result.Cancel;
      }

      foreach (Curve curve in curves)
      {
        Guid object_id = doc.Objects.AddCurve(curve);
        RhinoObject rhino_object = doc.Objects.Find(object_id);
        rhino_object?.Select(true);
      }

      foreach (Point3d point in points)
      {
        Guid object_id = doc.Objects.AddPoint(point);
        RhinoObject rhino_object = doc.Objects.Find(object_id);
        rhino_object?.Select(true);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}