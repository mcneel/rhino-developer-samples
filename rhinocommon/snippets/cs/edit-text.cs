partial class Examples
{
  public static Rhino.Commands.Result EditText(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select text", false, Rhino.DocObjects.ObjectType.Annotation, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.DocObjects.TextObject textobj = objref.Object() as Rhino.DocObjects.TextObject;
    if (textobj == null)
      return Rhino.Commands.Result.Failure;

    Rhino.Geometry.TextEntity textentity = textobj.Geometry as Rhino.Geometry.TextEntity;
    if (textentity == null)
      return Rhino.Commands.Result.Failure;
    string str = textentity.Text;
    rc = Rhino.Input.RhinoGet.GetString("New text", false, ref str);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    textentity.Text = str;
    textobj.CommitChanges();
    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
