using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsFaceWithHole : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsFaceWithHole"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var brep = MakeBrepFace();
      if (null == brep)
        return Result.Failure;

      doc.Objects.AddBrep(brep);
      doc.Views.Redraw();

      return Result.Success;
    }

    /// <summary>
    /// This example demonstrates how to construct a ON_Brep
    /// with the topology shown below.
    ///
    ///  D---------e2-----C      
    ///  |                |     
    ///  |  G----e6---H   |
    ///  |  |         |   |
    ///  e3 e5        e7  |
    ///  |  |         |   |
    ///  |  F----e4---E   |
    ///  |                |
    ///  A-------e0-------B
    ///
    ///  Things need to be defined in a valid brep:
    ///   1- Vertices
    ///   2- 3D Curves (geometry)
    ///   3- Edges (topology - reference curve geometry)
    ///   4- Surface (geometry)
    ///   5- Faces (topology - reference surface geometry)
    ///   6- Loops (2D parameter space of faces)
    ///   4- Trims and 2D curves (2D parameter space of edges)
    /// </summary>
    /// <returns>A Brep if successful, null otherwise</returns>
    public static Brep MakeBrepFace()
    {
      // Define the vertices
      var points = new Point3d[8];
      points[A] = new Point3d(0.0, 0.0, 0.0);   // point A = geometry for vertex 0
      points[B] = new Point3d(10.0, 0.0, 0.0);  // point B = geometry for vertex 1
      points[C] = new Point3d(10.0, 10.0, 0.0); // point C = geometry for vertex 2
      points[D] = new Point3d(0.0, 10.0, 0.0);  // point D = geometry for vertex 3

      points[E] = new Point3d(8.0, 2.0, 0.0);   // point E = geometry for vertex 4
      points[F] = new Point3d(2.0, 2.0, 0.0);   // point F = geometry for vertex 5
      points[G] = new Point3d(2.0, 8.0, 0.0);   // point G = geometry for vertex 6
      points[H] = new Point3d(8.0, 8.0, 0.0);   // point H = geometry for vertex 7

      // Create the Brep
      var brep = new Brep();

      // Create four vertices of the outer edges
      for (var vi = 0; vi < 8; vi++)
      {
        // This simple example is exact - for models with
        // non-exact data, set tolerance as explained in
        // definition of BrepVertex.
        brep.Vertices.Add(points[vi], 0.0);
      }

      // Create 3d curve geometry of the outer boundary 
      // The orientations are arbitrarily chosen
      // so that the end vertices are in alphabetical order.
      brep.Curves3D.Add(CreateLinearCurve(points[A], points[B])); // line AB
      brep.Curves3D.Add(CreateLinearCurve(points[B], points[C])); // line BC
      brep.Curves3D.Add(CreateLinearCurve(points[C], points[D])); // line CD
      brep.Curves3D.Add(CreateLinearCurve(points[A], points[D])); // line AD

      // Create 3d curve geometry of the hole 
      brep.Curves3D.Add(CreateLinearCurve(points[E], points[F])); // line EF
      brep.Curves3D.Add(CreateLinearCurve(points[F], points[G])); // line GH
      brep.Curves3D.Add(CreateLinearCurve(points[G], points[H])); // line HI
      brep.Curves3D.Add(CreateLinearCurve(points[E], points[H])); // line EI

      // Create edge topology for each curve in the brep.
      CreateEdges(brep);

      // Create 3d surface geometry - the orientations are arbitrarily chosen so
      // that some normals point into the cube and others point out of the cube.
      brep.AddSurface(CreateNurbsSurface(points[A], points[B], points[C], points[D])); // ABCD

      // Create face topology and 2d parameter space loops and trims.
      CreateFace(brep, ABCD);

#if DEBUG
      string tlog;
      var rc = brep.IsValidTopology(out tlog);
      if (!rc)
      {
        RhinoApp.WriteLine(tlog);
        return null;
      }

      string glog;
      rc = brep.IsValidGeometry(out glog);
      if (!rc)
      {
        RhinoApp.WriteLine(glog);
        return null;
      }
#endif

      // Validate the results
      if (!brep.IsValid)
      {
        RhinoApp.Write("Brep face is not valid.");
        return null;
      }

      return brep;
    }

    // Symbolic vertex index constants to make code more readable
    private static int A = 0;
    private static int B = 1;
    private static int C = 2;
    private static int D = 3;
    private static int E = 4;
    private static int F = 5;
    private static int G = 6;
    private static int H = 7;

    // Symbolic edge index constants to make code more readable
    private static int AB = 0;
    private static int BC = 1;
    private static int CD = 2;
    private static int AD = 3;
    private static int EF = 4;
    private static int FG = 5;
    private static int GH = 6;
    private static int EH = 7;

    // Symbolic face index constants to make code more readable
    private static int ABCD = 0;

    /// <summary>
    /// Creates a 3d edge curve
    /// </summary>
    /// <param name="start">The start point</param>
    /// <param name="end">The end point</param>
    /// <returns>The 3d edge curve if successful, null otherwise</returns>
    private static Curve CreateLinearCurve(Point3d start, Point3d end)
    {
      // Creates a 3d line segment to be used as a 3d curve in a Brep
      var curve = new LineCurve(start, end) { Domain = new Interval(0.0, 10.0) };
      return curve;
    }

    /// <summary>
    /// Creates a NURBS surface
    /// </summary>
    /// <param name="sw">The first corner point</param>
    /// <param name="se">The second corner point</param>
    /// <param name="ne">The third corner point</param>
    /// <param name="nw">The forth corner point</param>
    /// <returns>The surface if successful, null otherwise</returns>
    private static Surface CreateNurbsSurface(Point3d sw, Point3d se, Point3d ne, Point3d nw)
    {
      var nurb = NurbsSurface.Create(
        3,     // dimension (>= 1)
        false, // not rational
        2,     // "u" order (>= 2)
        2,     // "v" order (>= 2)
        2,     // number of control vertices in "u" dir (>= order)
        2      // number of control vertices in "v" dir (>= order)
        );

      // Corner CVs in counter clockwise order starting in the south west
      nurb.Points.SetControlPoint(0, 0, sw);
      nurb.Points.SetControlPoint(1, 0, se);
      nurb.Points.SetControlPoint(1, 1, ne);
      nurb.Points.SetControlPoint(0, 1, nw);

      // "u" knots
      nurb.KnotsU[0] = 0.0;
      nurb.KnotsU[1] = 1.0;

      // "v" knots
      nurb.KnotsV[0] = 0.0;
      nurb.KnotsV[1] = 1.0;

      return nurb;
    }

    /// <summary>
    /// Creates an edge of the Brep
    /// </summary>
    /// <param name="brep">The Brep</param>
    /// <param name="vi0">Index of the start vertex</param>
    /// <param name="vi1">Index of the end vertex</param>
    /// <param name="c3i">Index of the 3d curve</param>
    private static void CreateOneEdge(Brep brep, int vi0, int vi1, int c3i)
    {
      var start_vertex = brep.Vertices[vi0];
      var end_vertex = brep.Vertices[vi1];

      // This simple example is exact - for models with
      // non-exact data, set tolerance as explained in
      // definition of BrepEdge.
      brep.Edges.Add(start_vertex, end_vertex, c3i, 0.0);
    }

    /// <summary>
    /// Creates the edges of the Brep
    /// </summary>
    /// <param name="brep">The Brep</param>
    private static void CreateEdges(Brep brep)
    {
      // In this simple example, the edge indices exactly match the 3d
      // curve indices.  In general,the correspondence between edge and
      // curve indices can be arbitrary.  It is permitted for multiple
      // edges to use different portions of the same 3d curve.  The 
      // orientation of the edge always agrees with the natural 
      // parametric orientation of the curve.

      // outer edges

      // edge that runs from A to B
      CreateOneEdge(brep, A, B, AB);

      // edge that runs from B to C
      CreateOneEdge(brep, B, C, BC);

      // edge that runs from C to D
      CreateOneEdge(brep, C, D, CD);

      // edge that runs from A to D
      CreateOneEdge(brep, A, D, AD);

      // Inner Edges

      // edge that runs from E to F
      CreateOneEdge(brep, E, F, EF);

      // edge that runs from F to G
      CreateOneEdge(brep, F, G, FG);

      // edge that runs from G to H
      CreateOneEdge(brep, G, H, GH);

      // edge that runs from E to H
      CreateOneEdge(brep, E, H, EH);
    }

    /// <summary>
    /// Create a 2d outer trimming curve.
    /// </summary>
    /// <param name="surface">The surface</param>
    /// <param name="side">The side</param>
    /// <returns>A 2d trimming curve is successful, null otherwise</returns>
    private static Curve CreateOuterTrimmingCurve(Surface surface, int side)
    {
      // A trimming curve is a 2d curve whose image lies in the surface's domain.
      // The "active" portion of the surface is to the left of the trimming curve.
      // An outer trimming loop consists of a simple closed curve running 
      // counter-clockwise around the region it trims.

      // In cases when trim curve is not easily defined in surface domain, 
      // use Surface.Pullback only be careful about curve direction to ensure
      // loop trims run anti-clockwise for outer loop and clockwise for inner loop.

      // In this case, trim curves lie on the four edges of the surface

      var domain_u = surface.Domain(0);
      var domain_v = surface.Domain(1);

      var start = new Point2d();
      var end = new Point2d();

      switch (side)
      {
        case 0: // SW to SE
          start.X = domain_u.T0;
          start.Y = domain_v.T0;
          end.X = domain_u.T1;
          end.Y = domain_v.T0;
          break;
        case 1: // SE to NE
          start.X = domain_u.T1;
          start.Y = domain_v.T0;
          end.X = domain_u.T1;
          end.Y = domain_v.T1;
          break;
        case 2: // NE to NW
          start.X = domain_u.T1;
          start.Y = domain_v.T1;
          end.X = domain_u.T0;
          end.Y = domain_v.T1;
          break;
        case 3: // NW to SW
          start.X = domain_u.T0;
          start.Y = domain_v.T1;
          end.X = domain_u.T0;
          end.Y = domain_v.T0;
          break;
        default:
          return null;
      }

      var curve = new LineCurve(start, end) { Domain = new Interval(0.0, 1.0) };

      return curve;
    }

    /// <summary>
    /// Creates the outer trimming loop
    /// </summary>
    /// <param name="brep">The Brep</param>
    /// <param name="face">The BrepFace</param>
    /// <param name="eSi">Index of edge close to south side of surface</param>
    /// <param name="eSDir">Orientation of edge with respect to surface trim</param>
    /// <param name="eEi">Index of edge close to east side of surface</param>
    /// <param name="eEDir">Orientation of edge with respect to surface trim</param>
    /// <param name="eNi">Index of edge close to north side of surface</param>
    /// <param name="eNDir">Orientation of edge with respect to surface trim</param>
    /// <param name="eWi">Index of edge close to west side of surface</param>
    /// <param name="eWDir">Orientation of edge with respect to surface trim</param>
    /// <returns></returns>
    private static int MakeOuterTrimmingLoop(
        Brep brep,
        BrepFace face,
        int eSi,
        int eSDir,
        int eEi,
        int eEDir,
        int eNi,
        int eNDir,
        int eWi,
        int eWDir
        )
    {
      var surface = brep.Surfaces[face.SurfaceIndex];

      // Create new loop
      var loop = brep.Loops.Add(BrepLoopType.Outer, face);

      // Create trimming curves running counter clockwise around the
      // surface's domain. Note that trims of outer loops run counter
      // clockwise while trims of inner loops (holes) run clockwise.

      // Also note that when trims locate on surface N,S,E or W ends, 
      // then trim_iso becomes N_iso, S_iso, E_iso and W_iso respectfully.  
      // While if trim is parallel to surface N,S or E,W, then trim is becomes
      // y_iso and x_iso respectfully.

      // Start at the south side
      for (var side = 0; side < 4; side++)
      {
        // side: 0=south, 1=east, 2=north, 3=west
        var curve = CreateOuterTrimmingCurve(surface, side);

        // Add trimming curve to brep trimming curves array
        var c2i = brep.Curves2D.Add(curve);

        var ei = 0;
        var reverse = false;
        var iso = IsoStatus.None;

        switch (side)
        {
          case 0: // south
            ei = eSi;
            reverse = (eSDir == -1);
            iso = IsoStatus.South;
            break;
          case 1: // east
            ei = eEi;
            reverse = (eEDir == -1);
            iso = IsoStatus.East;
            break;
          case 2: // north
            ei = eNi;
            reverse = (eNDir == -1);
            iso = IsoStatus.North;
            break;
          case 3: // west
            ei = eWi;
            reverse = (eWDir == -1);
            iso = IsoStatus.West;
            break;
        }

        // Create new trim topology that references edge, direction
        // reletive to edge, loop and trim curve geometry
        var trim = brep.Trims.Add(brep.Edges[ei], reverse, loop, c2i);
        trim.IsoStatus = iso;

        // This one Brep face, so all trims are boundary ones.
        trim.TrimType = BrepTrimType.Boundary;

        // This simple example is exact - for models with
        // non-exact data, set tolerance as explained in
        // definition of BrepTrim.
        trim.SetTolerances(0.0, 0.0);
      }

      return loop.LoopIndex;
    }

    /// <summary>
    /// Create the inner 2d trimming curve.
    /// </summary>
    /// <param name="surface">The surface</param>
    /// <param name="side">The side</param>
    /// <returns>A 2d trimming curve is successful, null otherwise</returns>
    private static Curve CreateInnerTrimmingCurve(Surface surface, int side)
    {
      // A trimming curve is a 2d curve whose image lies in the surface's domain.
      // The "active" portion of the surface is to the left of the trimming curve.
      // An inner trimming loop consists of a simple closed curve running 
      // clockwise around the region the hole.

      // In this case, trim curves lie with 0.2 domain distance from surface edge
      var domain_u = surface.Domain(0);
      var domain_v = surface.Domain(1);

      var du = 0.2 * (domain_u.T1 - domain_u.T0);
      var dv = 0.2 * (domain_v.T1 - domain_v.T0);

      domain_u.T0 += du;
      domain_u.T1 -= du;
      domain_v.T0 += dv;
      domain_v.T1 -= dv;

      var start = new Point2d();
      var end = new Point2d();

      switch (side)
      {
        case 0: // near SE to SW
          start.X = domain_u.T1;
          start.Y = domain_v.T0;
          end.X = domain_u.T0;
          end.Y = domain_v.T0;
          break;
        case 1: // near SW to NW
          start.X = domain_u.T0;
          start.Y = domain_v.T0;
          end.X = domain_u.T0;
          end.Y = domain_v.T1;
          break;
        case 2: // near NW to NE
          start.X = domain_u.T0;
          start.Y = domain_v.T1;
          end.X = domain_u.T1;
          end.Y = domain_v.T1;
          break;
        case 3: // near NE to SE
          start.X = domain_u.T1;
          start.Y = domain_v.T1;
          end.X = domain_u.T1;
          end.Y = domain_v.T0;
          break;
        default:
          return null;
      }

      var curve = new LineCurve(start, end) { Domain = new Interval(0.0, 1.0) };

      return curve;
    }

    /// <summary>
    /// Creates the inner trimming loop
    /// </summary>
    /// <param name="brep">The Brep</param>
    /// <param name="face">The BrepFace</param>
    /// <param name="eSi">Index of edge close to south side of surface</param>
    /// <param name="eSDir">Orientation of edge with respect to surface trim</param>
    /// <param name="eEi">Index of edge close to east side of surface</param>
    /// <param name="eEDir">Orientation of edge with respect to surface trim</param>
    /// <param name="eNi">Index of edge close to north side of surface</param>
    /// <param name="eNDir">Orientation of edge with respect to surface trim</param>
    /// <param name="eWi">Index of edge close to west side of surface</param>
    /// <param name="eWDir">Orientation of edge with respect to surface trim</param>
    /// <returns></returns>
    private static int MakeInnerTrimmingLoop(
        Brep brep,
        BrepFace face,
        int eSi,
        int eSDir,
        int eEi,
        int eEDir,
        int eNi,
        int eNDir,
        int eWi,
        int eWDir
        )
    {
      var surface = brep.Surfaces[face.SurfaceIndex];

      // Create new inner loop
      var loop = brep.Loops.Add(BrepLoopType.Inner, face);

      // Create trimming curves running counter clockwise around the
      // surface's domain. Note that trims of outer loops run counter
      // clockwise while trims of inner loops (holes) run clockwise.

      // Also note that when trims locate on surface N,S,E or W ends, 
      // then trim_iso becomes N_iso, S_iso, E_iso and W_iso respectfully.  
      // While if trim is parallel to surface N,S or E,W, then trim is becomes
      // y_iso and x_iso respectfully.

      // Start near the south side
      for (var side = 0; side < 4; side++)
      {
        // side: 0=near south(y_iso), 1=near west(x_iso), 2=near north(y_iso), 3=near east(x_iso)

        // Create trim 2d curve
        var curve = CreateInnerTrimmingCurve(surface, side);

        // Add trimming curve to brep trimming curves array
        var c2i = brep.Curves2D.Add(curve);

        var ei = 0;
        var reverse = false;
        var iso = IsoStatus.None;

        switch (side)
        {
          case 0: // near south
            ei = eSi;
            reverse = (eSDir == -1);
            iso = IsoStatus.Y;
            break;
          case 1: // near west
            ei = eEi;
            reverse = (eEDir == -1);
            iso = IsoStatus.X;
            break;
          case 2: // near north
            ei = eNi;
            reverse = (eNDir == -1);
            iso = IsoStatus.Y;
            break;
          case 3: // near east
            ei = eWi;
            reverse = (eWDir == -1);
            iso = IsoStatus.X;
            break;
        }

        // Create new trim topology that references edge, direction
        // reletive to edge, loop and trim curve geometry
        var trim = brep.Trims.Add(brep.Edges[ei], reverse, loop, c2i);
        trim.IsoStatus = iso;

        // This one Brep face, so all trims are boundary ones.
        trim.TrimType = BrepTrimType.Boundary;

        // This simple example is exact - for models with
        // non-exact data, set tolerance as explained in
        // definition of BrepTrim.
        trim.SetTolerances(0.0, 0.0);
      }

      return loop.LoopIndex;
    }

    /// <summary>
    /// Adds faces to the Brep
    /// </summary>
    /// <param name="brep">The Brep</param>
    /// <param name="si">The surface index</param>
    private static void CreateFace(Brep brep, int si)
    {
      // Add new face to brep
      var face = brep.Faces.Add(si);

      // Create outer loop and trims for the face
      MakeOuterTrimmingLoop(brep, face,
        AB, +1,      // South side edge and its orientation with respect to
                      // to the trimming curve.  (AB)
        BC, +1,      // East side edge and its orientation with respect to
                      // to the trimming curve.  (BC)
        CD, +1,      // North side edge and its orientation with respect to
                      // to the trimming curve   (CD)
        AD, -1       // West side edge and its orientation with respect to
                      // to the trimming curve   (AD)
        );


      // Create loop and trims for the face
      MakeInnerTrimmingLoop(brep, face,
        EF, +1,      // Parallel to south side edge and its orientation with respect to
                      // to the trimming curve.  (EF)
        FG, +1,      // Parallel to east side edge and its orientation with respect to
                      // to the trimming curve.  (FG)
        GH, +1,      // Parallel to north side edge and its orientation with respect to
                      // to the trimming curve   (GH)
        EH, -1       // Parallel to west side edge and its orientation with respect to
                      // to the trimming curve   (EH)
        );

      // Set face direction relative to surface direction
      face.OrientationIsReversed = false;
    }
  }
}
