partial class Examples
{
  public static Result GetPointDynamicDraw(RhinoDoc doc)
  {
    var gp = new GetPoint();
    gp.SetCommandPrompt("Center point");
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var center_point = gp.Point();
    if (center_point == Point3d.Unset)
      return Result.Failure;

    var gcp = new GetCircleRadiusPoint(center_point);
    gcp.SetCommandPrompt("Radius");
    gcp.ConstrainToConstructionPlane(false);
    gcp.SetBasePoint(center_point, true);
    gcp.DrawLineFromPoint(center_point, true);
    gcp.Get();
    if (gcp.CommandResult() != Result.Success)
      return gcp.CommandResult();

    var radius = center_point.DistanceTo(gcp.Point());
    var cplane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane();
    doc.Objects.AddCircle(new Circle(cplane, center_point, radius));
    doc.Views.Redraw();
    return Result.Success;
  }
}

public class GetCircleRadiusPoint : GetPoint
{
  private Point3d m_center_point;

  public GetCircleRadiusPoint(Point3d centerPoint)
  {
    m_center_point = centerPoint;
  }

  protected override void OnDynamicDraw(GetPointDrawEventArgs e)
  {
    base.OnDynamicDraw(e);
    var cplane = e.RhinoDoc.Views.ActiveView.ActiveViewport.ConstructionPlane();
    var radius = m_center_point.DistanceTo(e.CurrentPoint);
    var circle = new Circle(cplane, m_center_point, radius);
    e.Display.DrawCircle(circle, System.Drawing.Color.Black);
  }
}
