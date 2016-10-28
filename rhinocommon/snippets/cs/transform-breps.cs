partial class Examples
{
  public static Rhino.Commands.Result TransformBrep(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef rhobj;
    var rc = RhinoGet.GetOneObject("Select brep", true, Rhino.DocObjects.ObjectType.Brep, out rhobj);
    if(rc!= Rhino.Commands.Result.Success)
      return rc;

    // Simple translation transformation
    var xform = Rhino.Geometry.Transform.Translation(18,-18,25);
    doc.Objects.Transform(rhobj, xform, true);
    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
