partial class Examples
{
  public static Rhino.Commands.Result AddBrepBox(Rhino.RhinoDoc doc)
  {
    Rhino.Geometry.Point3d pt0 = new Rhino.Geometry.Point3d(0, 0, 0);
    Rhino.Geometry.Point3d pt1 = new Rhino.Geometry.Point3d(10, 10, 10);
    Rhino.Geometry.BoundingBox box = new Rhino.Geometry.BoundingBox(pt0, pt1);
    Rhino.Geometry.Brep brep = box.ToBrep();
    Rhino.Commands.Result rc = Rhino.Commands.Result.Failure;
    if( doc.Objects.AddBrep(brep) != System.Guid.Empty )
    {
      rc = Rhino.Commands.Result.Success;
      doc.Views.Redraw();
    }
    return rc;
  }
}
