using Rhino;
using System.Collections.Generic;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace examples_cs
{
  public class PickPointsOnConduitCommand : Rhino.Commands.Command
  {
    private readonly List<ConduitPoint> m_conduit_points = new List<ConduitPoint>();

    public override string EnglishName { get { return "csPickPoints"; } }

    protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      var conduit = new PointsConduit(m_conduit_points);
      conduit.Enabled = true;

      var gp = new Rhino.Input.Custom.GetPoint();
      while (true)
      {
        gp.SetCommandPrompt("click location to create point. (<ESC> exit)");
        gp.AcceptNothing(true);
        gp.Get();
        if (gp.CommandResult() != Rhino.Commands.Result.Success)
          break;
        m_conduit_points.Add(new ConduitPoint(gp.Point()));
        doc.Views.Redraw();
      }

      var gcp = new GetConduitPoint(m_conduit_points);
      while (true)
      {
        gcp.SetCommandPrompt("select conduit point. (<ESC> to exit)");
        gcp.AcceptNothing(true);
        gcp.Get(true);
        doc.Views.Redraw();
        if (gcp.CommandResult() != Rhino.Commands.Result.Success)
          break;
      }

      return Rhino.Commands.Result.Success;
    }
  }

  public class ConduitPoint
  {
    public ConduitPoint(Point3d point)
    {
      Color = System.Drawing.Color.White;
      Point = point;
    }
    public System.Drawing.Color Color { get; set; }
    public Point3d Point { get; set; }
  }

  public class GetConduitPoint : GetPoint
  {
    private readonly List<ConduitPoint> m_conduit_points;
 
    public GetConduitPoint(List<ConduitPoint> conduitPoints )
    {
      m_conduit_points = conduitPoints;
    }

    protected override void OnMouseDown(GetPointMouseEventArgs e)
    {
      base.OnMouseDown(e);
      var picker = new PickContext();
      picker.View = e.Viewport.ParentView;

      picker.PickStyle = PickStyle.PointPick;

      var xform = e.Viewport.GetPickTransform(e.WindowPoint);
      picker.SetPickTransform(xform);

      foreach (var cp in m_conduit_points)
      {
        double depth;
        double distance;
        if (picker.PickFrustumTest(cp.Point, out depth, out distance))
          cp.Color = System.Drawing.Color.Red;
        else
          cp.Color = System.Drawing.Color.White;
      }
    }
  }

  class PointsConduit : Rhino.Display.DisplayConduit
  {
    private readonly List<ConduitPoint> m_conduit_points;
 
    public PointsConduit(List<ConduitPoint> conduitPoints )
    {
      m_conduit_points = conduitPoints;
    }

    protected override void DrawForeground(Rhino.Display.DrawEventArgs e)
    {
      if (m_conduit_points != null)
        foreach (var cp in m_conduit_points) 
        e.Display.DrawPoint(cp.Point, PointStyle.Simple, 3, cp.Color);
    }
  }
}