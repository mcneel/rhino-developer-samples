using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("5c16e399-76ea-4361-824f-aa049e0211fc")]
  public class SampleCsCurveGetter : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsCurveGetter"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select curves");
      go.GeometryFilter = ObjectType.Curve;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      for (var i = 0; i < go.ObjectCount; i++)
      {
        var curve = go.Object(i).Curve();
        if (null == curve)
          return Result.Failure;

        if (curve is LineCurve)
          RhinoApp.WriteLine("Curve {0} is a line.", i);
        else if (curve is ArcCurve)
        {
          RhinoApp.WriteLine(curve.IsClosed ? "Curve {0} is a circle." : "Curve {0} is an arc.", i);
        }
        else if (curve is PolylineCurve)
          RhinoApp.WriteLine("Curve {0} is a polyline.", i);
        else if (curve is PolyCurve)
          RhinoApp.WriteLine("Curve {0} is a polycurve.", i);
        else if (curve is NurbsCurve)
        {
          if (curve.IsEllipse())
          {
            if (curve.IsClosed)
              RhinoApp.WriteLine("Curve {0} is an ellipse.", i);
            else
              RhinoApp.WriteLine("Curve {0} is an elliptical arc.", i);
          }
          else
            RhinoApp.WriteLine("Curve {0} is a NURBS curve.", i);
        }
        else
          RhinoApp.WriteLine("Curve {0} is an unknown type.", i);
      }

      return Result.Success;
    }
  }
}
