using System.Collections.Generic;
using Rhino.Geometry;

namespace SampleGhPlatonics.Geometry
{
  public class Cube : PlatonicGeometryBase
  {
    public override string DisplayName => "Cube";

    public override int VertexCount => 8;

    public override int EdgeCount => 6;

    public override Point3d[] Vertices()
    {
      var v = new List<Point3d>(VertexCount)
      {
        new Point3d(0.5, 0.5, 0.5),
        new Point3d(0.5, 0.5, -0.5),
        new Point3d(0.5, -0.5, 0.5),
        new Point3d(0.5, -0.5, -0.5),
        new Point3d(-0.5, 0.5, 0.5),
        new Point3d(-0.5, 0.5, -0.5),
        new Point3d(-0.5, -0.5, 0.5),
        new Point3d(-0.5, -0.5, -0.5)
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
        CreateEdge(v[0], v[1], v[5], v[4]),
        CreateEdge(v[0], v[4], v[6], v[2]),
        CreateEdge(v[0], v[2], v[3], v[1]),
        CreateEdge(v[7], v[3], v[2], v[6]),
        CreateEdge(v[7], v[6], v[4], v[5]),
        CreateEdge(v[7], v[5], v[1], v[3])
      };
      return e.ToArray();
    }
  }
}
