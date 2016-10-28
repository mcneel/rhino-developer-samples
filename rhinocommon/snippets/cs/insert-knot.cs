partial class Examples
{
  public static Rhino.Commands.Result InsertKnot(Rhino.RhinoDoc doc)
  {
    const ObjectType filter = Rhino.DocObjects.ObjectType.Curve;
    Rhino.DocObjects.ObjRef objref;
    Result rc = Rhino.Input.RhinoGet.GetOneObject("Select curve for knot insertion", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success)
      return rc;
    Rhino.Geometry.Curve curve = objref.Curve();
    if (null == curve)
      return Rhino.Commands.Result.Failure;
    Rhino.Geometry.NurbsCurve nurb = curve.ToNurbsCurve();
    if (null == nurb)
      return Rhino.Commands.Result.Failure;

    Rhino.Input.Custom.GetPoint gp = new Rhino.Input.Custom.GetPoint();
    gp.SetCommandPrompt("Point on curve to add knot");
    gp.Constrain(nurb, false);
    gp.Get();
    if (gp.CommandResult() == Rhino.Commands.Result.Success)
    {
      double t;
      Rhino.Geometry.Curve crv = gp.PointOnCurve(out t);
      if( crv!=null && nurb.Knots.InsertKnot(t) )
      {
        doc.Objects.Replace(objref, nurb);
        doc.Views.Redraw();
      }
    }
    return Rhino.Commands.Result.Success;  
  }
}
