using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsIntersectCurveLine : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsIntersectCurveLine"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select curve to intersect");
      go.GeometryFilter = ObjectType.Curve;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curve = go.Object(0).Curve();
      if (null == curve)
        return Result.Failure;

      var gp = new GetPoint();
      gp.SetCommandPrompt("First point of infinite intersecting line");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var from = gp.Point();

      gp.SetCommandPrompt("Second point of infinite intersecting line");
      gp.SetBasePoint(from, true);
      gp.DrawLineFromPoint(from, true);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var line = new Rhino.Geometry.Line(from, gp.Point());
      if (!line.IsValid || line.Length < Rhino.RhinoMath.SqrtEpsilon)
        return Result.Nothing;

      var ccx = IntersectCurveLine(curve, line, doc.ModelAbsoluteTolerance, doc.ModelAbsoluteTolerance);
      if (null != ccx)
      {
        foreach (var x in ccx)
        {
          if (x.IsPoint)
            doc.Objects.AddPoint(x.PointA);
        }
        doc.Views.Redraw();
      }

      return Result.Success;
    }

    /// <summary>
    /// Intersects a curve and an infinite line.
    /// </summary>
    /// <param name="curve">Curve to intersect.</param>
    /// <param name="line">Infinite line to intesect.</param>
    /// <param name="tolerance">If the distance from a point on curve to line
    /// is &lt;= tolerance, then the point will be part of an intersection
    /// event. If the tolerance &lt;= 0.0, then 0.001 is used.</param>
    /// <param name="overlapTolerance">If t1 and t2 are parameters of curve's
    /// intersection events and the distance from curve(t) to line is &lt;= 
    /// overlapTolerance for every t1 &lt;= t &lt;= t2, then the event will 
    /// be returened as an overlap event. If overlapTolerance &lt;= 0.0, 
    /// then tolerance * 2.0 is used.</param>
    /// <returns>A collection of intersection events.</returns>
    public static Rhino.Geometry.Intersect.CurveIntersections IntersectCurveLine(
      Rhino.Geometry.Curve curve, 
      Rhino.Geometry.Line line, 
      double tolerance, 
      double overlapTolerance
      )
    {
      if (!curve.IsValid || !line.IsValid || line.Length < Rhino.RhinoMath.SqrtEpsilon)
        return null;

      // Extend the line through the curve's bounding box
      var bbox = curve.GetBoundingBox(false);
      if (!bbox.IsValid)
        return null;

      var dir = line.Direction;
      dir.Unitize();

      var points = bbox.GetCorners();
      var plane = new Rhino.Geometry.Plane(line.From, dir);

      double max_dist;
      var min_dist = max_dist = plane.DistanceTo(points[0]);
      for (var i = 1; i < points.Length; i++)
      {
        var dist = plane.DistanceTo(points[i]);
        if (dist < min_dist)
          min_dist = dist;
        if (dist > max_dist)
          max_dist = dist;
      }

      // +- 1.0 makes the line a little bigger than the bounding box
      line.From = line.From + dir * (min_dist - 1.0);
      line.To = line.From + dir * (max_dist + 1.0);

      // Calculate curve-curve intersection
      var line_curve = new Rhino.Geometry.LineCurve(line);
      return Rhino.Geometry.Intersect.Intersection.CurveCurve(curve, line_curve, tolerance, overlapTolerance);
    }
  }
}
