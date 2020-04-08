using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  /// <summary>
  /// Class to hold arc arguments
  /// </summary>
  internal class ArcArguments
  {
    /// <summary>
    /// Pubic constructor
    /// </summary>
    public ArcArguments()
    {
      Point1 = Point3d.Unset;
      Point2 = Point3d.Unset;
      Point3 = Point3d.Unset;
      Normal = Vector3d.Unset;
      Angle = RhinoMath.UnsetValue;
      Quadrant = 0;
      Dir = 1;
    }

    /// <summary>
    /// Center of the arc.
    /// </summary>
    public Point3d Point1 { get; set; }

    /// <summary>
    /// Staring point of arc, which defines the radius.
    /// </summary>
    public Point3d Point2 { get; set; }

    /// <summary>
    /// End point of arc, which defines the arc angle.
    /// </summary>
    public Point3d Point3 { get; set; }

    /// <summary>
    /// A normal defining the orientation in relation to the world.
    /// </summary>
    public Vector3d Normal { get; set; }

    /// <summary>
    /// The calculated arc angle in radians.
    /// </summary>
    public double Angle { get; set; }

    /// <summary>
    /// Quadrant (1,2,3,4 in counterclockwise order).
    /// </summary>
    public int Quadrant { get; set; }

    /// <summary>
    /// The direction of the arc, 1 = counter-clockwise, -1 = clockwise.
    /// </summary>
    public int Dir { get; set; }
  }


  /// <summary>
  /// GetThirdArcPoint class
  /// </summary>
  internal class GetThirdArcPoint : GetPoint
  {
    private ArcArguments m_args;
    private Arc m_arc;
    private bool m_have_arc = false;

    /// <summary>
    /// Private constructor
    /// </summary>
    private GetThirdArcPoint()
    {
    }

    /// <summary>
    /// Public consttructor
    /// </summary>
    public GetThirdArcPoint(ArcArguments args)
    {
      m_args = args;
    }

    /// <summary>
    /// OnMouseMove override
    /// </summary>
    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      m_args.Point3 = e.Point;
      m_have_arc = CalculateArc(m_args, out Arc arc);
      if (m_have_arc)
        m_arc = arc;
      base.OnMouseMove(e);
    }

    /// <summary>
    /// OnDynamicDraw override
    /// </summary>
    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (m_have_arc)
      {
        e.Display.DrawArc(m_arc, DynamicDrawColor);
        e.Display.DrawPoint(m_arc.Center);
      }

      var relative_length = 1.5;
      var v1 = m_args.Point2 - m_args.Point1;
      var v2 = m_args.Point3 - m_args.Point1;
      var v3 = e.CurrentPoint - m_args.Point1;
      var length1 = v1.Length * relative_length;
      var length3 = v3.Length;
      v2.Unitize();
      v1 *= relative_length;
      if (length1 > length3)
        v2 *= length1;
      else
        v2 *= length3;

      e.Display.DrawLine(m_args.Point1, m_args.Point1 + v1, DynamicDrawColor);
      e.Display.DrawLine(m_args.Point1, m_args.Point1 + v2, DynamicDrawColor);

      e.Display.DrawPoint(m_args.Point1);
      e.Display.DrawPoint(m_args.Point2);

      base.OnDynamicDraw(e);
    }

    /// <summary>
    /// Arc calculator
    /// </summary>
    public static bool CalculateArc(ArcArguments args, out Arc arc)
    {
      arc = new Arc();

      var x = args.Point2 - args.Point1;
      var y = args.Point3 - args.Point1;
      if (x.IsTiny() || y.IsTiny())
        return false;

      var radius = x.Length;
      var quadrant = args.Quadrant;
      var dir = args.Dir;

      var new_quadrant = WhichQuadrant(args.Point1, args.Point2, args.Point3, args.Normal);
      if (quadrant == 0)
        dir = (new_quadrant < 3) ? 1 : -1; // unspecified previous quadrant
      else if ((quadrant == 1 && new_quadrant == 4 && dir == 1) || (quadrant == 4 && new_quadrant == 1 && dir == -1))
        dir = -dir; // switched directions
      quadrant = new_quadrant;

      x.Unitize();
      y.Unitize();

      var dot = x * y;
      dot = RhinoMath.Clamp(dot, -1.0, 1.0);
      var angle = Math.Acos(dot);

      if (dir > 0)
        y = Vector3d.CrossProduct(args.Normal, x);
      else
        y = Vector3d.CrossProduct(-args.Normal, x);

      var plane = new Plane(args.Point1, x, y);
      if (!plane.IsValid)
        return false;

      if ((dir == 1 && new_quadrant > 2) || (dir == -1 && new_quadrant < 3))
        angle = 2.0 * Math.PI - angle;

      arc = new Arc(plane, args.Point1, radius, angle);
      var rc = arc.IsValid;
      if (rc)
      {
        args.Quadrant = quadrant;
        args.Dir = dir;
      }

      return rc;
    }

    /// <summary>
    /// Determines the point location in relation to two axes.
    /// The quadrants are numbered in counter-clockwise order starting from 1.
    /// </summary>
    private static int WhichQuadrant(Point3d point1, Point3d point2, Point3d point3, Vector3d normal)
    {
      var x = point2 - point1;
      var r = point3 - point1;
      var y = Vector3d.CrossProduct(normal, x);
      var dotX = x * r;
      var dotY = y * r;
      if (dotX >= 0.0)
        return (dotY >= 0.0) ? 1 : 4;
      else
        return (dotY >= 0.0) ? 2 : 3;
    }
  }

  /// <summary>
  /// SampleCsArc command
  /// </summary>
  public class SampleCsArc : Command
  {
    public override string EnglishName => "SampleCsArc";

    /// <summary>
    /// RunCommand override
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var args = new ArcArguments();

      // Center of arc
      var gp = new GetPoint();
      gp.SetCommandPrompt("Center of arc");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      args.Point1 = gp.Point();

      // Start of arc
      gp.SetCommandPrompt("Start of arc");
      gp.ConstrainToConstructionPlane(true);
      gp.SetBasePoint(args.Point1, true);
      gp.DrawLineFromPoint(args.Point1, true);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      args.Point2 = gp.Point();

      var arc_plane = gp.View().ActiveViewport.ConstructionPlane();
      arc_plane.Origin = args.Point1;
      args.Normal = arc_plane.ZAxis;

      // End of arc
      var gp3 = new GetThirdArcPoint(args);
      gp3.SetCommandPrompt("End of arc");
      gp3.Constrain(arc_plane, false);
      gp3.ConstrainDistanceFromBasePoint(args.Point1.DistanceTo(args.Point2));
      gp3.SetBasePoint(args.Point1, false);
      gp3.Get();
      if (gp3.CommandResult() != Result.Success)
        return gp3.CommandResult();

      args.Point3 = gp3.Point();

      if (GetThirdArcPoint.CalculateArc(args, out Arc arc))
      {
        doc.Objects.AddArc(arc);
        doc.Views.Redraw();
        return Result.Success;
      }

      return Result.Failure;
    }
  }
}