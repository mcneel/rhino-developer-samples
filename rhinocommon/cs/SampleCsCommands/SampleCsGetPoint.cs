using System;
using System.Globalization;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsGetPoint : Command
  {
    public SampleCsGetPoint()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsGetPoint"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gp = new GetPoint();
      gp.SetCommandPrompt("Pick a point");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var point = gp.Point();

      var format = string.Format("F{0}", doc.DistanceDisplayPrecision);
      var provider = CultureInfo.InvariantCulture;

      var x = point.X.ToString(format, provider);
      var y = point.Y.ToString(format, provider);
      var z = point.Z.ToString(format, provider);
      RhinoApp.WriteLine("World coordinates: {0},{1},{2}", x, y, z);

      var view = gp.View();
      if (null != view)
      {
        var plane = view.ActiveViewport.ConstructionPlane();
        var xform = Transform.ChangeBasis(Plane.WorldXY, plane);
        
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
