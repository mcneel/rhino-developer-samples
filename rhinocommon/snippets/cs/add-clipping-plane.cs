partial class Examples
{
  public static Rhino.Commands.Result AddClippingPlane(Rhino.RhinoDoc doc)
  {
    // Define the corners of the clipping plane
    Rhino.Geometry.Point3d[] corners;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetRectangle(out corners);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    // Get the active view
    Rhino.Display.RhinoView view = doc.Views.ActiveView;
    if (view == null)
      return Rhino.Commands.Result.Failure;

    Rhino.Geometry.Point3d p0 = corners[0];
    Rhino.Geometry.Point3d p1 = corners[1];
    Rhino.Geometry.Point3d p3 = corners[3];

    // Create a plane from the corner points
    Rhino.Geometry.Plane plane = new Rhino.Geometry.Plane(p0, p1, p3);

    // Add a clipping plane object to the document
    Guid id = doc.Objects.AddClippingPlane(plane, p0.DistanceTo(p1), p0.DistanceTo(p3), view.ActiveViewportID);
    if (id != Guid.Empty)
    {
      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }
    return Rhino.Commands.Result.Failure;
  }
}
