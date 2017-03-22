using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Rhino;
using Rhino.Geometry;

namespace PlatonicsCommon
{
  public class Cube : PlatonicBase
  {
    public override string DisplayName
    {
      get { return "Cube"; }
    }

    public override string Prompt
    {
      get { return "Edge length"; }
    }

    public override int VertexCount
    {
      get { return 8; }
    }

    public override int EdgeCount
    {
      get { return 6; }
    }

    public override Point3d[] Vertices()
    {
      List<Point3d> v = new List<Point3d>(VertexCount)
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
      Point3d[] v = Vertices();
      if (VertexCount != v.Length)
        return null;

      List<PolylineCurve> e = new List<PolylineCurve>(EdgeCount)
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
