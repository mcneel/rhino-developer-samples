using System;

partial class Examples
{
  public static Rhino.Commands.Result TweenCurve(Rhino.RhinoDoc doc)
  {
    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select two curves");
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
    go.GetMultiple(2, 2);
    if (go.CommandResult() != Rhino.Commands.Result.Success)
      return go.CommandResult();

    Rhino.Geometry.Curve curve0 = go.Object(0).Curve();
    Rhino.Geometry.Curve curve1 = go.Object(1).Curve();
    if (null != curve0 && null != curve1)
    {
      Rhino.Geometry.Curve[] curves = Rhino.Geometry.Curve.CreateTweenCurves(curve0, curve1, 1);
      if (null != curves)
      {
        for (int i = 0; i < curves.Length; i++)
          doc.Objects.AddCurve(curves[i]);

        doc.Views.Redraw();
        return Rhino.Commands.Result.Success;
      }
    }

    return Rhino.Commands.Result.Failure;
  }
}
