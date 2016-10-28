partial class Examples
{
  public static Result SurfaceFromCorners(RhinoDoc doc)
  {
    var surface = NurbsSurface.CreateFromCorners(
      new Point3d(5, 0, 0),
      new Point3d(5, 5, 5),
      new Point3d(0, 5, 0),
      new Point3d(0, 0, 0));

    doc.Objects.AddSurface(surface);
    doc.Views.Redraw();

    return Rhino.Commands.Result.Success;
  }
}
