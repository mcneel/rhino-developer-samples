partial class Examples
{
  public static Rhino.Commands.Result ObjectDecoration(Rhino.RhinoDoc doc)
  {
    // Define a line
    var line = new Rhino.Geometry.Line(new Rhino.Geometry.Point3d(0, 0, 0), new Rhino.Geometry.Point3d(10, 0, 0));

    // Make a copy of Rhino's default object attributes
    var attribs = doc.CreateDefaultAttributes();

    // Modify the object decoration style
    attribs.ObjectDecoration = Rhino.DocObjects.ObjectDecoration.BothArrowhead;

    // Create a new curve object with our attributes
    doc.Objects.AddLine(line, attribs);
    doc.Views.Redraw();

    return Rhino.Commands.Result.Success;
  }
}
