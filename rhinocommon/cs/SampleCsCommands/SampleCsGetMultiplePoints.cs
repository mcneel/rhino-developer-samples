using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  #region SampleCsGetMultiplePointsConduit

  /// <summary>
  /// SampleCsGetMultiplePointsConduit
  /// </summary>
  public class SampleCsGetMultiplePointsConduit : Rhino.Display.DisplayConduit
  {
    private List<Rhino.Geometry.Point3d> _points;

    public SampleCsGetMultiplePointsConduit()
    {
      _points = new List<Rhino.Geometry.Point3d>();
    }

    public void AddPoint(Rhino.Geometry.Point3d point)
    {
      _points.Add(point);
    }

    public void RemoveLastPoint()
    {
      int pointCount = _points.Count;
      if (pointCount > 0)
        _points.RemoveAt(pointCount - 1);
    }

    public List<Rhino.Geometry.Point3d> Points
    {
      get { return _points; }
    }

    public int PointCount
    {
      get { return _points.Count; }
    }

    protected override void CalculateBoundingBox(Rhino.Display.CalculateBoundingBoxEventArgs e)
    {
      var bbox = e.BoundingBox;
      foreach (Point3d pt in _points)
        bbox.Union(pt);
      e.IncludeBoundingBox(bbox);
    }

    protected override void DrawOverlay(Rhino.Display.DrawEventArgs e)
    {
      System.Drawing.Color color = Rhino.ApplicationSettings.AppearanceSettings.DefaultObjectColor;
      for (int i = 0; i < _points.Count; i++)
        e.Display.DrawPoint(_points[i], color);
    }
  }

  #endregion

  #region SampleCsGetMultiplePoints

  /// <summary>
  /// SampleCsGetMultiplePoints command
  /// </summary>
  [System.Runtime.InteropServices.Guid("6c4b530a-78ec-44cf-be47-59fa92b08ad0")]
  public class SampleCsGetMultiplePoints : Command
  {
    public SampleCsGetMultiplePoints()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsGetMultiplePoints"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      SampleCsGetMultiplePointsConduit conduit = new SampleCsGetMultiplePointsConduit();
      conduit.Enabled = true;

      Rhino.Commands.Result rc = Result.Nothing;

      Rhino.Input.Custom.GetPoint gp = new Rhino.Input.Custom.GetPoint();
      while (true)
      {
        if (0 == conduit.PointCount)
        {
          gp.SetCommandPrompt("Location of point object.");
          gp.AcceptNothing(false);
          gp.ClearCommandOptions();
        }
        else
        {
          gp.SetCommandPrompt("Location of point object. Press Enter when done");
          gp.AcceptNothing(true);
          gp.AddOption("Undo");
        }

        Rhino.Input.GetResult res = gp.Get();

        if (res == Rhino.Input.GetResult.Point)
        {
          conduit.AddPoint(gp.Point());
          doc.Views.Redraw();
        }
        else if (res == Rhino.Input.GetResult.Option)
        {
          conduit.RemoveLastPoint();
          doc.Views.Redraw();
        }
        else if (res == Rhino.Input.GetResult.Nothing)
        {
          rc = Rhino.Commands.Result.Success;
          break;
        }
        else
        {
          rc = Rhino.Commands.Result.Cancel;
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
