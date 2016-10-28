using Rhino;
using Rhino.Geometry;
using Rhino.Commands;

namespace examples_cs
{
  public class SurfaceFromCornersCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csSurfaceFromCorners"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var surface = NurbsSurface.CreateFromCorners(
        new Point3d(5, 0, 0),
        new Point3d(5, 5, 5),
        new Point3d(0, 5, 0),
        new Point3d(0, 0, 0));

      doc.Objects.AddSurface(surface);
      doc.Views.Redraw();

      return Rhino.Commands.Result.Success;
    }
  }
}