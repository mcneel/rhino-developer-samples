using Rhino;
using Rhino.DocObjects;
using Rhino.Input.Custom;
using Rhino.Commands;
using System.Collections.Generic;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;

namespace examples_cs
{
  public class ProjectPointsToBrepsCommand : Command
  {
    public override string EnglishName { get { return "csProjectPtointsToBreps"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gs = new GetObject();
      gs.SetCommandPrompt("select surface");
      gs.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      gs.DisablePreSelect();
      gs.SubObjectSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();
      var brep = gs.Object(0).Brep();
      if (brep == null)
        return Result.Failure;

      var points = Intersection.ProjectPointsToBreps(
                   new List<Brep> {brep}, // brep on which to project
                   new List<Point3d> {new Point3d(0, 0, 0), new Point3d(3,0,3), new Point3d(-2,0,-2)}, // some random points to project
                   new Vector3d(0, 1, 0), // project on Y axis
                   doc.ModelAbsoluteTolerance);

      if (points != null && points.Length > 0)
      {
        foreach (var point in points)
        {
          doc.Objects.AddPoint(point);
        }
      }
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}