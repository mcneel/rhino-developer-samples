using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Rhino;
using Rhino.Geometry;

namespace PlatonicsCommon
{
  public class Octahedron : PlatonicBase
  {
    public override string DisplayName
    {
      get { return "Octahedron"; }
    }

    public override string Prompt
    {
      get { return "Edge length"; }
    }

    public override int VertexCount
    {
      get { return 6; }
    }

    public override int EdgeCount
    {
      get { return 8; }
    }

    public override Point3d[] Vertices()
    {
      double c0 = Math.Sqrt(2.0) / 2.0;

      List<Point3d> v = new List<Point3d>(VertexCount)
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
      Point3d[] v = Vertices();
      if (VertexCount != v.Length)
        return null;

      List<PolylineCurve> e = new List<PolylineCurve>(EdgeCount)
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
