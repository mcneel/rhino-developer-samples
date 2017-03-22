using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Rhino;
using Rhino.Geometry;

namespace PlatonicsCommon
{
  public class Tetrahedron : PlatonicBase
  {
    public override string DisplayName
    {
      get { return "Tetrahedron"; }
    }

    public override string Prompt
    {
      get { return "Edge length"; }
    }

    public override int VertexCount
    {
      get { return 4; }
    }

    public override int EdgeCount
    {
      get { return 4; }
    }

    public override Point3d[] Vertices()
    {
      double c0 = Math.Sqrt(2.0) / 4.0;

      List<Point3d> v = new List<Point3d>(VertexCount)
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
      Point3d[] v = Vertices();
      if (VertexCount != v.Length)
        return null;

      List<PolylineCurve> e = new List<PolylineCurve>(EdgeCount)
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
