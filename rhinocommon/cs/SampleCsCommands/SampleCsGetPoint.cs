using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input.Custom;
using System.Globalization;

namespace SampleCsCommands
{
  public class SampleCsGetPoint : Command
  {
    public override string EnglishName => "SampleCsGetPoint";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetPoint gp = new GetPoint();
      gp.SetCommandPrompt("Pick a point");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      Point3d point = gp.Point();

      string format = string.Format("F{0}", doc.ModelDistanceDisplayPrecision);
      CultureInfo provider = CultureInfo.InvariantCulture;

      string x = point.X.ToString(format, provider);
      string y = point.Y.ToString(format, provider);
      string z = point.Z.ToString(format, provider);
      RhinoApp.WriteLine("World coordinates: {0},{1},{2}", x, y, z);

      Rhino.Display.RhinoView view = gp.View();
      if (null != view)
      {
        Plane plane = view.ActiveViewport.ConstructionPlane();
        Transform xform = Transform.ChangeBasis(Plane.WorldXY, plane);

        point.Transform(xform);

        x = point.X.ToString(format, provider);
        y = point.Y.ToString(format, provider);
        z = point.Z.ToString(format, provider);
        RhinoApp.WriteLine("CPlane coordinates: {0},{1},{2}", x, y, z);
      }

      return Result.Success;
    }
  }
}
