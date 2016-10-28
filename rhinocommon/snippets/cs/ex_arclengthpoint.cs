partial class Examples
{
  public static Rhino.Commands.Result ArcLengthPoint(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objref;
    Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select curve",
      true, Rhino.DocObjects.ObjectType.Curve,out objref);
    if(rc!= Rhino.Commands.Result.Success)
      return rc;
    Rhino.Geometry.Curve crv = objref.Curve();
    if( crv==null )
      return Rhino.Commands.Result.Failure;
 
    double crv_length = crv.GetLength();
    double length = 0;
    rc = Rhino.Input.RhinoGet.GetNumber("Length from start", true, ref length, 0, crv_length);
    if(rc!= Rhino.Commands.Result.Success)
      return rc;
 
    Rhino.Geometry.Point3d pt = crv.PointAtLength(length);
    if (pt.IsValid)
    {
      doc.Objects.AddPoint(pt);
      doc.Views.Redraw();
    }
    return Rhino.Commands.Result.Success;
  }
}
