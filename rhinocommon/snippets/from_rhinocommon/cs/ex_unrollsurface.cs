using Rhino.Commands;
using Rhino.DocObjects;

partial class Examples
{
  public static Rhino.Commands.Result UnrollSurface(Rhino.RhinoDoc doc)
  {
    const ObjectType filter = Rhino.DocObjects.ObjectType.Brep | Rhino.DocObjects.ObjectType.Surface;
    Rhino.DocObjects.ObjRef objref;
    Result rc = Rhino.Input.RhinoGet.GetOneObject("Select surface or brep to unroll", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success)
      return rc;
    Rhino.Geometry.Unroller unroll=null;
    Rhino.Geometry.Brep brep = objref.Brep();
    if (brep != null)
      unroll = new Rhino.Geometry.Unroller(brep);
    else
    {
      Rhino.Geometry.Surface srf = objref.Surface();
      if (srf != null)
        unroll = new Rhino.Geometry.Unroller(srf);
    }
    if (unroll == null)
      return Rhino.Commands.Result.Cancel;

    unroll.AbsoluteTolerance = 0.01;
    unroll.RelativeTolerance = 0.01;

    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select points, curves, and dots to unroll with surface");
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Point | Rhino.DocObjects.ObjectType.Curve | Rhino.DocObjects.ObjectType.TextDot;
    go.AcceptNothing(true);
    go.GetMultiple(0, 0);
    if (go.CommandResult() != Rhino.Commands.Result.Success)
      return go.CommandResult();
    for (int i = 0; i < go.ObjectCount; i++)
    {
      objref = go.Object(i);
      Rhino.Geometry.GeometryBase g = objref.Geometry();
      Rhino.Geometry.Point pt = g as Rhino.Geometry.Point;
      Rhino.Geometry.Curve crv = g as Rhino.Geometry.Curve;
      Rhino.Geometry.TextDot dot = g as Rhino.Geometry.TextDot;
      if (pt != null)
        unroll.AddFollowingGeometry(pt.Location);
      else if (crv != null)
        unroll.AddFollowingGeometry(crv);
      else if (dot != null)
        unroll.AddFollowingGeometry(dot);
    }

    unroll.ExplodeOutput = false;
    Rhino.Geometry.Curve[] curves;
    Rhino.Geometry.Point3d[] points;
    Rhino.Geometry.TextDot[] dots;
    Rhino.Geometry.Brep[] breps = unroll.PerformUnroll(out curves, out points, out dots);
    if (breps == null || breps.Length < 1)
      return Rhino.Commands.Result.Failure;

    for (int i = 0; i < breps.Length; i++)
      doc.Objects.AddBrep(breps[i]);
    for (int i = 0; i < curves.Length; i++)
      doc.Objects.AddCurve(curves[i]);
    doc.Objects.AddPoints(points);
    for (int i = 0; i < dots.Length; i++)
      doc.Objects.AddTextDot(dots[i]);
    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
