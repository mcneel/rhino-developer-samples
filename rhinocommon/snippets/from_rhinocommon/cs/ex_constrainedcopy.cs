using System;

partial class Examples
{
  public static Rhino.Commands.Result ConstrainedCopy(Rhino.RhinoDoc doc)
  {
    // Get a single planar closed curve
    var go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select curve");
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve;
    go.Get();
    if( go.CommandResult() != Rhino.Commands.Result.Success )
      return go.CommandResult();
    var objref = go.Object(0);
    var base_curve = objref.Curve();
    var first_point = objref.SelectionPoint();
    if( base_curve==null || !first_point.IsValid )
      return Rhino.Commands.Result.Cancel;

    Rhino.Geometry.Plane plane;
    if( !base_curve.TryGetPlane(out plane) )
      return Rhino.Commands.Result.Cancel;

    // Get a point constrained to a line passing through the initial selection
    // point and parallel to the plane's normal
    var gp = new Rhino.Input.Custom.GetPoint();
    gp.SetCommandPrompt("Offset point");
    gp.DrawLineFromPoint(first_point, true);
    var line = new Rhino.Geometry.Line(first_point, first_point+plane.Normal);
    gp.Constrain(line);
    gp.Get();
    if( gp.CommandResult() != Rhino.Commands.Result.Success )
      return gp.CommandResult();
    var second_point = gp.Point();
    Rhino.Geometry.Vector3d vec = second_point - first_point;
    if( vec.Length > 0.001 )
    {
      var xf = Rhino.Geometry.Transform.Translation(vec);
      Guid id = doc.Objects.Transform(objref, xf, false);
      if( id!=Guid.Empty )
      {
        doc.Views.Redraw();
        return Rhino.Commands.Result.Success;
      }
    }
    return Rhino.Commands.Result.Cancel;
  }
}
