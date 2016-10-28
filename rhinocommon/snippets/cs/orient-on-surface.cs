partial class Examples
{
  public static Rhino.Commands.Result OrientOnSrf(Rhino.RhinoDoc doc)
  {
    // Select objects to orient
    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select objects to orient");
    go.SubObjectSelect = false;
    go.GroupSelect = true;
    go.GetMultiple(1, 0);
    if (go.CommandResult() != Rhino.Commands.Result.Success)
      return go.CommandResult();

    // Point to orient from
    Rhino.Input.Custom.GetPoint gp = new Rhino.Input.Custom.GetPoint();
    gp.SetCommandPrompt("Point to orient from");
    gp.Get();
    if (gp.CommandResult() != Rhino.Commands.Result.Success)
      return gp.CommandResult();

    // Define source plane
    Rhino.Display.RhinoView view = gp.View();
    if (view == null)
    {
      view = doc.Views.ActiveView;
      if (view == null)
        return Rhino.Commands.Result.Failure;
    }
    Rhino.Geometry.Plane source_plane = view.ActiveViewport.ConstructionPlane();
    source_plane.Origin = gp.Point();

    // Surface to orient on
    Rhino.Input.Custom.GetObject gs = new Rhino.Input.Custom.GetObject();
    gs.SetCommandPrompt("Surface to orient on");
    gs.GeometryFilter = Rhino.DocObjects.ObjectType.Surface;
    gs.SubObjectSelect = true;
    gs.DeselectAllBeforePostSelect = false;
    gs.OneByOnePostSelect = true;
    gs.Get();
    if (gs.CommandResult() != Rhino.Commands.Result.Success)
      return gs.CommandResult();

    Rhino.DocObjects.ObjRef objref = gs.Object(0);
    // get selected surface object
    Rhino.DocObjects.RhinoObject obj = objref.Object();
    if (obj == null)
      return Rhino.Commands.Result.Failure;
    // get selected surface (face)
    Rhino.Geometry.Surface surface = objref.Surface();
    if (surface == null)
      return Rhino.Commands.Result.Failure;
    // Unselect surface
    obj.Select(false);

    // Point on surface to orient to
    gp.SetCommandPrompt("Point on surface to orient to");
    gp.Constrain(surface, false);
    gp.Get();
    if (gp.CommandResult() != Rhino.Commands.Result.Success)
      return gp.CommandResult();

    // Do transformation
    Rhino.Commands.Result rc = Rhino.Commands.Result.Failure;
    double u, v;
    if (surface.ClosestPoint(gp.Point(), out u, out v))
    {
      Rhino.Geometry.Plane target_plane;
      if (surface.FrameAt(u, v, out target_plane))
      {
        // Build transformation
        Rhino.Geometry.Transform xform = Rhino.Geometry.Transform.PlaneToPlane(source_plane, target_plane);

        // Do the transformation. In this example, we will copy the original objects
        const bool delete_original = false;
        for (int i = 0; i < go.ObjectCount; i++)
          doc.Objects.Transform(go.Object(i), xform, delete_original);

        doc.Views.Redraw();
        rc = Rhino.Commands.Result.Success;
      }
    }
    return rc;
  }
}
