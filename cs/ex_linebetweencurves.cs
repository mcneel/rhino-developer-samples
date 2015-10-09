using System;

partial class Examples
{
  public static Rhino.Commands.Result LineBetweenCurves(Rhino.RhinoDoc doc)
  {
    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select two curves");
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go.GetMultiple(2, 2);
    if (go.CommandResult() != Rhino.Commands.Result.Success)
      return go.CommandResult();

    Rhino.DocObjects.ObjRef objRef0 = go.Object(0);
    Rhino.DocObjects.ObjRef objRef1 = go.Object(1);

    double t0 = Rhino.RhinoMath.UnsetValue;
    double t1 = Rhino.RhinoMath.UnsetValue;
    Rhino.Geometry.Curve curve0 = objRef0.CurveParameter(out t0);
    Rhino.Geometry.Curve curve1 = objRef1.CurveParameter(out t1);
    if (null == curve0 || !Rhino.RhinoMath.IsValidDouble(t0) ||
        null == curve1 || !Rhino.RhinoMath.IsValidDouble(t1) )
      return Rhino.Commands.Result.Failure;

    Rhino.Geometry.Line line = Rhino.Geometry.Line.Unset;
    bool rc = Rhino.Geometry.Line.TryCreateBetweenCurves(curve0, curve1, ref t0, ref t1, false, false, out line);
    if (rc)
    {
      if (Guid.Empty != doc.Objects.AddLine(line))
      {
        doc.Views.Redraw();
        return Rhino.Commands.Result.Success;
      }
    }
    return Rhino.Commands.Result.Failure;
  }
}
