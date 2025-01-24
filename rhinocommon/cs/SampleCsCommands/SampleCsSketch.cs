using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System.Collections.Generic;

namespace SampleCsCommands
{
  /// <summary>
  /// GetSketchPoints class
  /// </summary>
  internal class GetSketchPoints : GetPoint
  {
    private System.Drawing.Point m_previous_point = new System.Drawing.Point(-1, -1);

    public List<Point3d> Points { get; } = new List<Point3d>(2048);

    public GetResult GetPoints()
    {
      GetResult rc = Get(true);
      if (rc == GetResult.Point)
        Points.Add(Point());
      return rc;
    }

    protected override void OnMouseDown(GetPointMouseEventArgs e)
    {
      if (Points.Count == 0)
      {
        m_previous_point = e.WindowPoint;
        Points.Add(e.Point);
        SetBasePoint(e.Point, true);
      }
    }

    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      if (Points.Count > 0)
      {
        int xdiff = m_previous_point.X - e.WindowPoint.X;
        int ydiff = m_previous_point.Y - e.WindowPoint.Y;
        if ((xdiff * xdiff + ydiff * ydiff) > 100)
        {
          m_previous_point = e.WindowPoint;
          Points.Add(e.Point);
        }
      }
      else
      {
        base.OnMouseMove(e);
      }
    }
    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (Points.Count > 0)
      {
        for (int i = 1; i < Points.Count; i++)
          e.Display.DrawLine(Points[i - 1], Points[i], DynamicDrawColor);
      }
      else
      {
        base.OnDynamicDraw(e);
      }
    }
  }

  /// <summary>
  /// SampleCsSketch command
  /// </summary>
  public class SampleCsSketch : Command
  {
    public override string EnglishName => "SampleCsSketch";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      bool old_snap_on = Rhino.ApplicationSettings.ModelAidSettings.GridSnap;
      if (old_snap_on)
        Rhino.ApplicationSettings.ModelAidSettings.GridSnap = false;


      GetSketchPoints gp = new GetSketchPoints();
      gp.SetCommandPrompt("Click and drag to sketch. Press Enter when done");
      gp.PermitOrthoSnap(false);
      gp.AcceptNothing(true);
      GetResult res = gp.GetPoints();
      if (res == GetResult.Point)
      {
        List<Point3d> points = gp.Points;
        if (points.Count > 2)
        {
          Point3d p0 = points[0];
          Point3d p1 = points[points.Count - 1];
          if (p0.DistanceTo(p1) < doc.ModelAbsoluteTolerance)
            points.Add(p0);

          PolylineCurve pline_curve = new PolylineCurve(points);
          doc.Objects.AddCurve(pline_curve);
          doc.Views.Redraw();
        }
      }

      if (old_snap_on)
        Rhino.ApplicationSettings.ModelAidSettings.GridSnap = true;

      return Result.Success;
    }
  }
}