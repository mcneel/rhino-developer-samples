using System;
using Rhino.Geometry;

partial class Examples
{
  public static Rhino.Commands.Result AddLinearDimension2(Rhino.RhinoDoc doc)
  {
    Point3d origin = new Point3d(1,1,0);
    Point3d offset = new Point3d(11,1,0);
    Point3d pt = new Point3d((offset.X-origin.X)/2,3,0);

    Plane plane = Plane.WorldXY;
    plane.Origin = origin;

    double u,v;
    plane.ClosestParameter(origin, out u, out v);
    Point2d ext1 = new Point2d(u, v);

    plane.ClosestParameter(offset, out u, out v);
    Point2d ext2 = new Point2d(u, v);

    plane.ClosestParameter(pt, out u, out v);
    Point2d linePt = new Point2d(u, v);

    LinearDimension dimension = new LinearDimension(plane, ext1, ext2, linePt);
    if (doc.Objects.AddLinearDimension(dimension) != Guid.Empty)
    {
      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }
    return Rhino.Commands.Result.Failure;
  }
}
