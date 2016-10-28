partial class Examples
{
  public static Rhino.Commands.Result AddLinearDimension(Rhino.RhinoDoc doc)
  {
    Rhino.Geometry.LinearDimension dimension;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetLinearDimension(out dimension);
    if (rc == Rhino.Commands.Result.Success && dimension != null)
    {
      if (doc.Objects.AddLinearDimension(dimension) == Guid.Empty)
        rc = Rhino.Commands.Result.Failure;
      else
        doc.Views.Redraw();
    }
    return rc;
  }
}
