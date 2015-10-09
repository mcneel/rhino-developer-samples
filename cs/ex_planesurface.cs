using Rhino;
using Rhino.Geometry;
using Rhino.Commands;

namespace examples_cs
{
  public class PlaneSurfaceCommand : Command
  {
    public override string EnglishName { get { return "csPlaneSurface"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Point3d[] corners;
      var rc = Rhino.Input.RhinoGet.GetRectangle(out corners);
      if (rc != Result.Success)
        return rc;

      var plane = new Plane(corners[0], corners[1], corners[2]);

      var plane_surface = new PlaneSurface(plane, 
        new Interval(0, corners[0].DistanceTo(corners[1])), 
        new Interval(0, corners[1].DistanceTo(corners[2])));

      doc.Objects.Add(plane_surface);
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}