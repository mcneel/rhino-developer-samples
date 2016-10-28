partial class Examples
{
  public static Rhino.Commands.Result BlockInsertionPoint(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objref;
    Result rc = Rhino.Input.RhinoGet.GetOneObject("Select instance", true, Rhino.DocObjects.ObjectType.InstanceReference, out objref);
    if (rc != Rhino.Commands.Result.Success)
      return rc;
    Rhino.DocObjects.InstanceObject instance = objref.Object() as Rhino.DocObjects.InstanceObject;
    if (instance != null)
    {
      Rhino.Geometry.Point3d pt = instance.InsertionPoint;
      doc.Objects.AddPoint(pt);
      doc.Views.Redraw();
    }
    return Rhino.Commands.Result.Success;
  }
}
