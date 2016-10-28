partial class Examples
{
  public static Rhino.Commands.Result ExtrudeBrepFace(Rhino.RhinoDoc doc)
  {
    Rhino.Input.Custom.GetObject go0 = new Rhino.Input.Custom.GetObject();
    go0.SetCommandPrompt("Select surface to extrude");
    go0.GeometryFilter = Rhino.DocObjects.ObjectType.Surface;
    go0.SubObjectSelect = true;
    go0.Get();
    if (go0.CommandResult() != Rhino.Commands.Result.Success)
      return go0.CommandResult();

    Rhino.Geometry.BrepFace face = go0.Object(0).Face();
    if (null == face)
      return Rhino.Commands.Result.Failure;

    Rhino.Input.Custom.GetObject go1 = new Rhino.Input.Custom.GetObject();
    go1.SetCommandPrompt("Select path curve");
    go1.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go1.SubObjectSelect = true;
    go1.DeselectAllBeforePostSelect = false;
    go1.Get();
    if (go1.CommandResult() != Rhino.Commands.Result.Success)
      return go1.CommandResult();

    Rhino.Geometry.Curve curve = go1.Object(0).Curve();
    if (null == curve)
      return Rhino.Commands.Result.Failure;

    Rhino.Geometry.Brep brep = face.CreateExtrusion(curve, true);
    if (null != brep)
    {
      doc.Objects.Add(brep);
      doc.Views.Redraw();
    }

    return Rhino.Commands.Result.Success;
  }
}
