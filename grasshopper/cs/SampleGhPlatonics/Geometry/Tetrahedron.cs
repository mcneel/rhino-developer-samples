using System;
using System.Collections.Generic;
using Rhino.Geometry;

namespace SampleGhPlatonics.Geometry
{
  public class Tetrahedron : PlatonicGeometryBase
  {
    public override string DisplayName => "Tetrahedron";

    public override int VertexCount => 4;

    public override int EdgeCount => 4;

    public override Point3d[] Vertices()
    {
      var c0 = Math.Sqrt(2.0) / 4.0;

      var v = new List<Point3d>(VertexCount)
      {
        new Point3d(c0, -c0, c0),
        new Point3d(c0, c0, -c0),
        new Point3d(-c0, c0, c0),
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
        CreateEdge(v[0], v[1], v[2]),
        CreateEdge(v[0], v[2], v[3]),
        CreateEdge(v[0], v[3], v[1]),
        CreateEdge(v[1], v[3], v[2])
      };
      return e.ToArray();
    }
  }
}
