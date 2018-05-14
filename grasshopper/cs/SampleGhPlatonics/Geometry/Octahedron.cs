using System;
using System.Collections.Generic;
using Rhino.Geometry;

namespace SampleGhPlatonics.Geometry
{
  public class Octahedron : PlatonicGeometryBase
  {
    public override string DisplayName => "Octahedron";

    public override int VertexCount => 6;

    public override int EdgeCount => 8;

    public override Point3d[] Vertices()
    {
      var c0 = Math.Sqrt(2.0) / 2.0;

      var v = new List<Point3d>(VertexCount)
      {
        new Point3d(0.0, 0.0, c0),
        new Point3d(0.0, 0.0, -c0),
        new Point3d(c0, 0.0, 0.0),
        new Point3d(-c0, 0.0, 0.0),
        new Point3d(0.0, c0, 0.0),
        new Point3d(0.0, -c0, 0.0)
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
        CreateEdge(v[0], v[2], v[4]),
        CreateEdge(v[0], v[4], v[3]),
        CreateEdge(v[0], v[3], v[5]),
        CreateEdge(v[0], v[5], v[2]),
        CreateEdge(v[1], v[2], v[5]),
        CreateEdge(v[1], v[5], v[3]),
        CreateEdge(v[1], v[3], v[4]),
        CreateEdge(v[1], v[4], v[2])
      };
      return e.ToArray();
    }
  }
}
