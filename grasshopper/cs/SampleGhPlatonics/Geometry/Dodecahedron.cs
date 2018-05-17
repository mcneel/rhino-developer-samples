using System;
using System.Collections.Generic;
using Rhino.Geometry;

namespace SampleGhPlatonics.Geometry
{
  public class Dodecahedron : PlatonicGeometryBase
  {
    public override string DisplayName => "Dodecahedron";

    public override int VertexCount => 20;

    public override int EdgeCount => 12;

    public override Point3d[] Vertices()
    {
      var c0 = (1.0 + Math.Sqrt(5.0)) / 4.0;
      var c1 = (3.0 + Math.Sqrt(5.0)) / 4.0;

      var v = new List<Point3d>(VertexCount)
      {
        new Point3d(0.0, 0.5, c1),
        new Point3d(0.0, 0.5, -c1),
        new Point3d(0.0, -0.5, c1),
        new Point3d(0.0, -0.5, -c1),
        new Point3d(c1, 0.0, 0.5),
        new Point3d(c1, 0.0, -0.5),
        new Point3d(-c1, 0.0, 0.5),
        new Point3d(-c1, 0.0, -0.5),
        new Point3d(0.5, c1, 0.0),
        new Point3d(0.5, -c1, 0.0),
        new Point3d(-0.5, c1, 0.0),
        new Point3d(-0.5, -c1, 0.0),
        new Point3d(c0, c0, c0),
        new Point3d(c0, c0, -c0),
        new Point3d(c0, -c0, c0),
        new Point3d(c0, -c0, -c0),
        new Point3d(-c0, c0, c0),
        new Point3d(-c0, c0, -c0),
        new Point3d(-c0, -c0, c0),
        new Point3d(-c0, -c0, -c0)
      };
      return v.ToArray();
    }

    public override PolylineCurve[] Edges()
    {
      var v = Vertices();
      if (VertexCount != v.Length)
        return null;

      var e = new List<PolylineCurve>(EdgeCount)
      {
        CreateEdge(v[0], v[2], v[14], v[4], v[12]),
        CreateEdge(v[0], v[12], v[8], v[10], v[16]),
        CreateEdge(v[0], v[16], v[6], v[18], v[2]),
        CreateEdge(v[7], v[6], v[16], v[10], v[17]),
        CreateEdge(v[7], v[17], v[1], v[3], v[19]),
        CreateEdge(v[7], v[19], v[11], v[18], v[6]),
        CreateEdge(v[9], v[11], v[19], v[3], v[15]),
        CreateEdge(v[9], v[15], v[5], v[4], v[14]),
        CreateEdge(v[9], v[14], v[2], v[18], v[11]),
        CreateEdge(v[13], v[1], v[17], v[10], v[8]),
        CreateEdge(v[13], v[8], v[12], v[4], v[5]),
        CreateEdge(v[13], v[5], v[15], v[3], v[1])
      };
      return e.ToArray();
    }
  }
}
