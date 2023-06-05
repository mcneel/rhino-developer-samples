using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsCurveDeviation : Command
  {
    public override string EnglishName => "SampleCsCurveDeviation";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject { GeometryFilter = ObjectType.Curve };
      go.SetCommandPrompt("Select curves to test");
      go.GetMultiple(2, 2);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curveA = go.Object(0).Curve();
      var curveB = go.Object(1).Curve();
      if (null == curveA || null == curveB)
        return Result.Failure;

      var rc = Curve.GetDistancesBetweenCurves(
        curveA,
        curveB,
        doc.ModelAbsoluteTolerance,
        out var maxDistance,
        out var maxDistanceParameterA,
        out var maxDistanceParameterB,
        out var minDistance,
        out _,
        out _
        );

      if (rc)
      {
        var line = new Line
        {
          From = curveA.PointAt(maxDistanceParameterA),
          To = curveB.PointAt(maxDistanceParameterB)
        };

        doc.Objects.AddLine(line);
        doc.Objects.AddPoint(line.From);
        doc.Objects.AddPoint(line.To);
        RhinoApp.WriteLine("Minimum deviation = {0}", minDistance);
        RhinoApp.WriteLine("Maximum deviation = {0}", maxDistance);
      }

      return Result.Success;
    }
  }
}
