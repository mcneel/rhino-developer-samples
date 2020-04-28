using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsIntersectCircles : Command
  {
    public override string EnglishName => "SampleCsIntersectCircles";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const double radius = 5.0;

      Point3d point0 = new Point3d(5.0, 5.0, 0.0);
      Point3d point1 = new Point3d(10.0, 10.0, 0.0);

      Plane plane0 = Plane.WorldXY;
      plane0.Origin = point0;

      Plane plane1 = Plane.WorldXY;
      plane1.Origin = point1;

      Circle circle0 = new Circle(plane0, radius);
      Circle circle1 = new Circle(plane1, radius);

      doc.Objects.AddCircle(circle0);
      doc.Objects.AddCircle(circle1);

      Point3d int0 = new Point3d();
      Point3d int1 = new Point3d();
      if (CircleCircleIntersection(circle0, circle1, ref int0, ref int1))
      {
        Guid id0 = doc.Objects.AddPoint(int0);
        Guid id1 = doc.Objects.AddPoint(int1);

        doc.Objects.Select(id0);
        doc.Objects.Select(id1);
      }

      doc.Views.Redraw();
      
      return Result.Success;
    }

    /// <summary>
    /// Calculates the intersects two circles that lie on the world x-y plane.
    /// Reference: http://paulbourke.net/geometry/circlesphere/
    /// </summary>
    private bool CircleCircleIntersection(Circle c0, Circle c1, ref Point3d p0, ref Point3d p1)
    {
      double x0 = c0.Center.X;
      double y0 = c0.Center.Y;
      double r0 = c0.Radius;

      double x1 = c1.Center.X;
      double y1 = c1.Center.Y;
      double r1 = c1.Radius;

      // dx and dy are the vertical and horizontal distances between the circle centers.
      double dx = x1 - x0;
      double dy = y1 - y0;

      // Determine the straight-line distance between the centers.
      double d = Hypot(dx, dy);

      // Check for solvability.
      if (d > (r0 + r1))
      {
        // No solution. circles do not intersect.
        return false;
      }
      if (d < Math.Abs(r0 - r1))
      {
        // No solution. one circle is contained in the other
        return false;
      }

      // 'point 2' is the point where the line through the circle
      // intersection points crosses the line between the circle
      // centers.  

      // Determine the distance from point 0 to point 2.
      double a = ((r0 * r0) - (r1 * r1) + (d * d)) / (2.0 * d);

      // Determine the coordinates of point 2.
      double x2 = x0 + (dx * a / d);
      double y2 = y0 + (dy * a / d);

      // Determine the distance from point 2 to either of the intersection points.
      double h = Math.Sqrt((r0 * r0) - (a * a));

      // Now determine the offsets of the intersection points from point 2.
      double rx = -dy * (h / d);
      double ry = dx * (h / d);

      // Determine the absolute intersection points.
      p0.X = x2 + rx;
      p0.Y = y2 + ry;
      p1.X = x2 - rx;
      p1.Y = y2 - ry;

      return true;
    }

    /// <summary>
    /// Calculates hypotenuse using Newton-Raphson method.
    /// </summary>
    private double Hypot(double x, double y)
    {
     double r;
     if (x == 0) return y;
     if (y == 0) return x;
     if (x < 0) x = -x;
     if (y < 0) y = -y;
     if (x < y) {r = x; x = y; y = r;}
     r = y / x;
     return x * Math.Sqrt(1 + r * r);    
    }
  }
}
