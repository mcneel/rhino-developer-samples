using Rhino.Geometry;

class MoveCPlanePoint : Rhino.Input.Custom.GetPoint
{
  readonly Rhino.DocObjects.ConstructionPlane m_cplane;
  public MoveCPlanePoint(Rhino.DocObjects.ConstructionPlane cplane)
  {
    m_cplane = cplane;
  }

  protected override void OnMouseMove(Rhino.Input.Custom.GetPointMouseEventArgs e)
  {
    Plane pl = m_cplane.Plane;
    pl.Origin = e.Point;
    m_cplane.Plane = pl;
  }

  protected override void OnDynamicDraw(Rhino.Input.Custom.GetPointDrawEventArgs e)
  {
    e.Display.DrawConstructionPlane(m_cplane);
  }
}

partial class Examples
{
  public static Rhino.Commands.Result MoveCPlane(Rhino.RhinoDoc doc)
  {
    Rhino.Display.RhinoView view = doc.Views.ActiveView;
    if (view == null)
      return Rhino.Commands.Result.Failure;

    Rhino.DocObjects.ConstructionPlane cplane = view.ActiveViewport.GetConstructionPlane();
    Point3d origin = cplane.Plane.Origin;

    MoveCPlanePoint gp = new MoveCPlanePoint(cplane);
    gp.SetCommandPrompt("CPlane origin");
    gp.SetBasePoint(origin, true);
    gp.DrawLineFromPoint(origin, true);
    gp.Get();

    if (gp.CommandResult() != Rhino.Commands.Result.Success)
      return gp.CommandResult();

    Point3d point = gp.Point();
    Vector3d v = origin - point;
    if (v.IsTiny())
      return Rhino.Commands.Result.Nothing;

    Plane pl = cplane.Plane;
    pl.Origin = point;
    cplane.Plane = pl;
    view.ActiveViewport.SetConstructionPlane(cplane);
    view.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
