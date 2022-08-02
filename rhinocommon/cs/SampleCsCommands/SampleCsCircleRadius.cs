using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsCircleRadius : Command
  {
    private double m_radius = 1.0;

    public override string EnglishName => "SampleCsCircleRadius";

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

      while (true)
      {
        var gr = new GetRadiusPoint(plane);
        gr.SetCommandPrompt("Radius");
        gr.AcceptNumber(true, false);
        gr.SetDefaultNumber(m_radius);
        gr.SetBasePoint(plane.Origin, true);
        gr.DrawLineFromPoint(plane.Origin, true);
        var result = gr.Get();
        if (result == GetResult.Point)
        {
          if (!gr.CalculateCircle(gr.Point()))
            continue;

          var circle = gr.Circle;
          m_radius = circle.Radius;
          doc.Objects.AddCircle(circle);
          break;
        }
        if (result == GetResult.Number)
        {
          var radius = gr.Number();
          if (radius <= 0.0)
          {
            RhinoApp.WriteLine("Radius must be greater than zero.");
            continue;
          }

          var circle = new Circle(plane, radius);
          m_radius = circle.Radius;
          doc.Objects.AddCircle(circle);
          break;
        }

        return Result.Cancel;
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }

  internal class GetRadiusPoint : GetPoint
  {
    private Plane BasePlane { get; }
    private bool CanDraw { get; set; }
    private System.Drawing.Color DrawColor { get; }
    
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
