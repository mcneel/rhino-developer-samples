using System.Collections.Generic;
using Rhino;
using Rhino.ApplicationSettings;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  #region SampleCsGetMultiplePointsConduit

  /// <summary>
  /// SampleCsGetMultiplePointsConduit conduit
  /// </summary>
  public class SampleCsGetMultiplePointsConduit : DisplayConduit
  {
    private readonly List<Point3d> m_points;
    private readonly System.Drawing.Color m_color;

    public SampleCsGetMultiplePointsConduit()
    {
      m_points = new List<Point3d>();
      m_color = AppearanceSettings.DefaultObjectColor;
    }

    public void AddPoint(Point3d point)
    {
      m_points.Add(point);
    }

    public void RemoveLastPoint()
    {
      var point_count = m_points.Count;
      if (point_count > 0)
        m_points.RemoveAt(point_count - 1);
    }

    public List<Point3d> Points
    {
      get { return m_points; }
    }

    public int PointCount
    {
      get { return m_points.Count; }
    }

    protected override void CalculateBoundingBox(CalculateBoundingBoxEventArgs e)
    {
      var bbox = new BoundingBox(m_points);
      e.IncludeBoundingBox(bbox);
    }

    protected override void DrawOverlay(DrawEventArgs e)
    {
      e.Display.DrawPoints(m_points, PointStyle.ControlPoint, 3, m_color);
    }
  }

  #endregion


  #region SampleCsGetMultiplePoints

  /// <summary>
  /// SampleCsGetMultiplePoints command
  /// </summary>
  public class SampleCsGetMultiplePoints : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsGetMultiplePoints"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var conduit = new SampleCsGetMultiplePointsConduit { Enabled = true };

      Result rc;

      var gp = new GetPoint();
      while (true)
      {
        if (0 == conduit.PointCount)
        {
          gp.SetCommandPrompt("Location of point object.");
          gp.AcceptNothing(false);
          gp.AcceptUndo(false);
        }
        else
        {
          gp.SetCommandPrompt("Location of point object. Press Enter when done");
          gp.AcceptNothing(true);
          gp.AcceptUndo(true);
        }

        var res = gp.Get();

        if (res == GetResult.Point)
        {
          conduit.AddPoint(gp.Point());
          doc.Views.Redraw();
        }
        else if (res == GetResult.Undo)
        {
          conduit.RemoveLastPoint();
          doc.Views.Redraw();
        }
        else if (res == GetResult.Nothing)
        {
          rc = Result.Success;
          break;
        }
        else
        {
          rc = Result.Cancel;
          break;
        }
      }

      if (rc == Result.Success && conduit.PointCount > 0)
        doc.Objects.AddPoints(conduit.Points);

      conduit.Enabled = false;
      doc.Views.Redraw();

      return rc;
    }
  }

  #endregion
}
