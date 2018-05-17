using Rhino.Geometry;

namespace SampleGhPlatonics.Geometry
{
  /// <summary>
  /// The base class for all platonic solids
  /// </summary>
  public abstract class PlatonicGeometryBase
  {
    /// <summary>
    /// The name of the platonic solid
    /// </summary>
    public string Name => GetType().Name;

    /// <summary>
    /// The display, or alternate, name of the platonic solid
    /// </summary>
    public abstract string DisplayName { get; }

    /// <summary>
    /// The number of vertices
    /// </summary>
    public abstract int VertexCount { get; }

    /// <summary>
    /// The number of closed, planar edges
    /// </summary>
    public abstract int EdgeCount { get; }

    /// <summary>
    /// Returns the vertices
    /// </summary>
    public abstract Point3d[] Vertices();

    /// <summary>
    /// Returns the closed, planar edges
    /// </summary>
    public abstract PolylineCurve[] Edges();

    /// <summary>
    /// Returns the unjoined, planar faces
    /// </summary>
    public Brep[] Faces()
    {
      var e = Edges();
      if (EdgeCount != e.Length)
        return null;

      return Brep.CreatePlanarBreps(e, 0.001);
    }

    /// <summary>
    /// Creates a closed, planar polyline from 3 points
    /// </summary>
    protected PolylineCurve CreateEdge(
      Point3d v0,
      Point3d v1,
      Point3d v2
      )
    {
      var v = new Point3d[4];
      v[0] = v0;
      v[1] = v1;
      v[2] = v2;
      v[3] = v0;

      var e = new PolylineCurve(v);
      return e.IsValid ? e : null;
    }

    /// <summary>
    /// Creates a closed, planar polyline from 4 points
    /// </summary>
    protected PolylineCurve CreateEdge(
      Point3d v0,
      Point3d v1,
      Point3d v2,
      Point3d v3
      )
    {
      var v = new Point3d[5];
      v[0] = v0;
      v[1] = v1;
      v[2] = v2;
      v[3] = v3;
      v[4] = v0;

      var e = new PolylineCurve(v);
      return e.IsValid ? e : null;
    }

    /// <summary>
    /// Creates a closed, planar polyline from 5 points
    /// </summary>
    protected PolylineCurve CreateEdge(
      Point3d v0,
      Point3d v1,
      Point3d v2,
      Point3d v3,
      Point3d v4
      )
    {
      var v = new Point3d[6];
      v[0] = v0;
      v[1] = v1;
      v[2] = v2;
      v[3] = v3;
      v[4] = v4;
      v[5] = v0;

      var e = new PolylineCurve(v);
      return e.IsValid ? e : null;
    }
  }
}
