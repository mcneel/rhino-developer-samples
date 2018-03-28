using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsCircleRadius : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsCircleRadius"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gp = new GetPoint();
      gp.SetCommandPrompt("Center of circle");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      var view = gp.View();
      if (null == view)
        return Result.Failure;

      var plane = view.ActiveViewport.ConstructionPlane();
      plane.Origin = gp.Point();

      var gr = new GetRadiusPoint(plane);
      gr.SetCommandPrompt("Radius");
      gr.Get();
      if (gr.CommandResult() != Result.Success)
        return gr.CommandResult();

      if (gr.CalculateCircle(gr.Point()))
      {
        doc.Objects.AddCircle(gr.Circle);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }

  internal class GetRadiusPoint : GetPoint
  {
    private Plane BasePlane { get; set; }
    private bool CanDraw { get; set; }
    private System.Drawing.Color DrawColor { get; set; }
    
    public Circle Circle { get; private set; }

    public GetRadiusPoint(Plane basePlane)
    {
      BasePlane = basePlane;
      CanDraw = false;
      DrawColor = Rhino.ApplicationSettings.AppearanceSettings.DefaultLayerColor;

      Circle = new Circle(Plane.WorldXY, 1.0);

      SetBasePoint(BasePlane.Origin, true);
      DrawLineFromPoint(BasePlane.Origin, true);
      Constrain(BasePlane, false);
    }

    public bool CalculateCircle(Point3d testPoint)
    {
      var rc = false;
      var point = BasePlane.ClosestPoint(testPoint);
      var dir = point - BasePlane.Origin;
      if (!dir.IsTiny())
      {
        Circle = new Circle(BasePlane, dir.Length);
        rc = Circle.IsValid;
      }
      return rc;
    }

    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      CanDraw = CalculateCircle(e.Point);
      base.OnMouseMove(e);
    }

    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (CanDraw)
        e.Display.DrawCircle(Circle, DrawColor);
      base.OnDynamicDraw(e);
    }
  }
}
