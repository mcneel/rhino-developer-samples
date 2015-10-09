using Rhino.Geometry;

partial class Examples
{
  public static Rhino.Commands.Result IntersectLines(Rhino.RhinoDoc doc)
  {
    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt( "Select lines" );
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go.GetMultiple( 2, 2);
    if( go.CommandResult() != Rhino.Commands.Result.Success )
      return go.CommandResult();
    if( go.ObjectCount != 2 )
      return Rhino.Commands.Result.Failure;

    LineCurve crv0 = go.Object(0).Geometry() as LineCurve;
    LineCurve crv1 = go.Object(1).Geometry() as LineCurve;
    if( crv0==null || crv1==null )
      return Rhino.Commands.Result.Failure;

    Line line0 = crv0.Line;
    Line line1 = crv1.Line;
    Vector3d v0 = line0.Direction;
    v0.Unitize();
    Vector3d v1 = line1.Direction;
    v1.Unitize();

    if( v0.IsParallelTo(v1) != 0 )
    {
      Rhino.RhinoApp.WriteLine("Selected lines are parallel.");
      return Rhino.Commands.Result.Nothing;
    }

    double a, b;
    if( !Rhino.Geometry.Intersect.Intersection.LineLine(line0, line1, out a, out b))
    {
      Rhino.RhinoApp.WriteLine("No intersection found.");
      return Rhino.Commands.Result.Nothing;
    }

    Point3d pt0 = line0.PointAt(a);
    Point3d pt1 = line1.PointAt(b);
    // pt0 and pt1 should be equal, so we will only add pt0 to the document
    doc.Objects.AddPoint( pt0 );
    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
