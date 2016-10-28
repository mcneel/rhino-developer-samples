partial class Examples
{
  public static Rhino.Commands.Result AddSphere(Rhino.RhinoDoc doc)
  {
    Rhino.Geometry.Point3d center = new Rhino.Geometry.Point3d(0, 0, 0);
    const double radius = 5.0;
    Rhino.Geometry.Sphere sphere = new Rhino.Geometry.Sphere(center, radius);
    if( doc.Objects.AddSphere(sphere) != Guid.Empty )
    {
      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }
    return Rhino.Commands.Result.Failure;
  }
}
