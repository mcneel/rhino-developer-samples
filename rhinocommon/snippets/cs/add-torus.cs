partial class Examples
{
  public static Rhino.Commands.Result AddTorus(Rhino.RhinoDoc doc)
  {
    const double major_radius = 4.0;
    const double minor_radius = 2.0;

    Rhino.Geometry.Plane plane = Rhino.Geometry.Plane.WorldXY;
    Rhino.Geometry.Torus torus = new Rhino.Geometry.Torus(plane, major_radius, minor_radius);
    Rhino.Geometry.RevSurface revsrf = torus.ToRevSurface();
    if (doc.Objects.AddSurface(revsrf) != Guid.Empty)
    {
      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }
    return Rhino.Commands.Result.Failure;
  }
}
