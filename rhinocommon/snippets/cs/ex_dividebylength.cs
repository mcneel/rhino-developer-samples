using Rhino.DocObjects;

partial class Examples
{
  public static Rhino.Commands.Result DivideByLengthPoints(Rhino.RhinoDoc doc)
  {
    const ObjectType filter = Rhino.DocObjects.ObjectType.Curve; 
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select curve to divide", false, filter, out objref);
    if (rc != Rhino.Commands.Result.Success || objref == null)
      return rc;

    Rhino.Geometry.Curve crv = objref.Curve();
    if (crv == null || crv.IsShort(Rhino.RhinoMath.ZeroTolerance))
      return Rhino.Commands.Result.Failure;

    double crv_length = crv.GetLength();
    string s = string.Format("Curve length is {0:f3}. Segment length", crv_length);

    double seg_length = crv_length / 2.0;
    rc = Rhino.Input.RhinoGet.GetNumber(s, false, ref seg_length, 0, crv_length);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    Rhino.Geometry.Point3d[] points;
    crv.DivideByLength(seg_length, true, out points);
    if (points == null)
      return Rhino.Commands.Result.Failure;

    foreach (Rhino.Geometry.Point3d point in points)
      doc.Objects.AddPoint(point);

    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
