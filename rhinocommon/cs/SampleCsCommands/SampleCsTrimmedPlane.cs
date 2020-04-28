using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsTrimmedPlane : Command
  {
    public override string EnglishName => "SampleCsTrimmedPlane";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var brep = MakeTrimmedPlane();
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
    ///  E-------C--------D
    ///  |       /\       | 
    ///  |      /  \      |
    ///  |     /    \     |
    ///  |    e2      e1  |     
    ///  |   /        \   |    
    ///  |  /          \  |  
    ///  | /            \ |  
    ///  A-----e0-------->B
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
    public static Brep MakeTrimmedPlane()
    {
      // Define the vertices
      var points = new Point3d[5];
      points[A] = new Point3d(0.0, 0.0, 0.0);   // point A = geometry for vertex 0 (and surface SW corner)
      points[B] = new Point3d(10.0, 0.0, 0.0);  // point B = geometry for vertex 1 (and surface SE corner)
      points[C] = new Point3d(5.0, 10.0, 0.0);  // point C = geometry for vertex 2
      points[D] = new Point3d(10.0, 10.0, 0.0); // point D (surface NE corner)
      points[E] = new Point3d(0.0, 10.0, 0.0);  // point E (surface NW corner)

      // Create the Brep
      var brep = new Brep();

      // Create three vertices located at the three points
      for (var vi = 0; vi < 3; vi++)
      {
        // This simple example is exact - for models with
        // non-exact data, set tolerance as explained in
        // definition of BrepVertex.
        brep.Vertices.Add(points[vi], 0.0);
      }

      // Create 3d curve geometry - the orientations are arbitrarily chosen
      // so that the end vertices are in alphabetical order.
      brep.Curves3D.Add(CreateLinearCurve(points[A], points[B])); // line AB
      brep.Curves3D.Add(CreateLinearCurve(points[B], points[C])); // line BC
      brep.Curves3D.Add(CreateLinearCurve(points[A], points[C])); // line CD

      // Create edge topology for each curve in the brep
      CreateEdges(brep);

      // Create 3d surface geometry - the orientations are arbitrarily chosen so
      // that some normals point into the cube and others point out of the cube.
      brep.AddSurface(CreatePlanarSurface(points[A], points[B], points[D], points[E])); // ABDE

      // Create face topology and 2d parameter space loops and trims
      CreateFaces(brep);

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
        RhinoApp.Write("Trimmed Brep face is not valid.");
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

    // Symbolic edge index constants to make code more readable
    private static int AB = 0;
    private static int BC = 1;
    private static int AC = 2;

    // Symbolic face index constants to make code more readable
    private static int ABC_i = 0;

    /// <summary>
    /// Create a 2d trimming curve.
    /// </summary>
    /// <param name="surface">The surface</param>
    /// <param name="side">The side</param>
    /// <returns>A 2d trimming curve is successful, null otherwise</returns>
    private static Curve CreateTrimmingCurve(Surface surface, int side)
    {
      // A trimming curve is a 2d curve whose image lies in the surface's domain.
      // The "active" portion of the surface is to the left of the trimming curve.
      // An outer trimming loop consists of a simple closed curve running 
      // counter-clockwise around the region it trims.
      //
      // An inner trimming loop consists of a simple closed curve running 
      // clockwise around the region the hole.

      var domain_u = surface.Domain(0);
      var domain_v = surface.Domain(1);

      var start = new Point2d();
      var end = new Point2d();

      switch (side)
      {
        case 0:  // SW to SE
          start.X = domain_u.T0;
          start.Y = domain_v.T0;
          end.X = domain_u.T1;
          end.Y = domain_v.T0;
          break;
        case 1: // diagonal
          start.X = domain_u.T1;
          start.Y = domain_v.T0;
          end.X = (domain_u.T0 + domain_u.T1) / 2;
          end.Y = domain_v.T1;
          break;
        case 2: // diagonal
          start.X = (domain_u.T0 + domain_u.T1) / 2;
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
    /// Creates a 3d edge curve
    /// </summary>
    /// <param name="start">The start point</param>
    /// <param name="end">The end point</param>
    /// <returns>The 3d edge curve if successful, null otherwise</returns>
    private static Curve CreateLinearCurve(Point3d start, Point3d end)
    {
      // Creates a 3d line segment to be used as a 3d curve in a Brep
      var curve = new LineCurve(start, end) { Domain = new Interval(0.0, 1.0) };
      return curve;
    }

    /// <summary>
    /// Creates a Brep surface
    /// </summary>
    /// <param name="sw">The first corner point</param>
    /// <param name="se">The second corner point</param>
    /// <param name="ne">The third corner point</param>
    /// <param name="nw">The forth corner point</param>
    /// <returns>The surface if successful, null otherwise</returns>
    private static Surface CreatePlanarSurface(Point3d sw, Point3d se, Point3d ne, Point3d nw)
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
      nurb.Points.SetPoint(0, 0, sw);
      nurb.Points.SetPoint(1, 0, se);
      nurb.Points.SetPoint(1, 1, ne);
      nurb.Points.SetPoint(0, 1, nw);

      // "u" knots
      nurb.KnotsU[0] = 0.0;
      nurb.KnotsU[1] = 1.0;

      // "v" knots
      nurb.KnotsV[0] = 0.0;
      nurb.KnotsV[1] = 1.0;

      return nurb;
    }

    /// <summary>
    /// Makes an edge of the Brep
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
    /// Makes the edges of the Brep
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

      // edge that runs from A to B
      CreateOneEdge(brep, A, B, AB);

      // edge that runs from B to C
      CreateOneEdge(brep, B, C, BC);

      // edge that runs from A to C
      CreateOneEdge(brep, A, C, AC);
    }

    /// <summary>
    /// Creates a trimming loop
    /// </summary>
    /// <param name="brep">The Brep</param>
    /// <param name="face">The face the loop is on</param>
    /// <param name="e0">Index of first edge</param>
    /// <param name="e0Dir">Orientation of edge</param>
    /// <param name="e1">Index of second edge</param>
    /// <param name="e1Dir">Orientation of edge</param>
    /// <param name="e2">Index of third edge</param>
    /// <param name="e2Dir">Orientation of edge</param>
    /// <returns>The index of the added loop</returns>
    private static int MakeTrimmingLoop(
        Brep brep,
        BrepFace face,
        int e0,
        int e0Dir,
        int e1,
        int e1Dir,
        int e2,
        int e2Dir
        )
    {
      var surface = brep.Surfaces[face.SurfaceIndex];
      var loop = brep.Loops.Add(BrepLoopType.Outer, face);

      // Create trimming curves running counter clockwise.
      // Note that trims of outer loops run counter clockwise
      // while trims of inner loops (holes) run clockwise.
      //
      // Also note that when trims locate on surface N,S,E or W ends, 
      // then trim_iso becomes N_iso, S_iso, E_iso and W_iso respectfully.  
      // While if trim is parallel to surface N,S or E,W, then trim is becomes
      // y_iso and x_iso respectfully, the rest are not_iso.

      // Start at the south side
      for (var side = 0; side < 3; side++)
      {
        var curve = CreateTrimmingCurve(surface, side);

        // Add trimming curve to brep trim curves array
        var c2i = brep.Curves2D.Add(curve);

        var ei = 0;
        var reverse = false;
        var iso = IsoStatus.None;

        switch (side)
        {
          case 0: // south
            ei = e0;
            reverse = (e0Dir == -1);
            iso = IsoStatus.South;
            break;
          case 1: // diagonal
            ei = e1;
            reverse = (e1Dir == -1);
            iso = IsoStatus.None;
            break;
          case 2: // diagonal
            ei = e2;
            reverse = (e2Dir == -1);
            iso = IsoStatus.None;
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
    /// Makes a Brep face
    /// </summary>
    /// <param name="brep">The Brep</param>
    /// <param name="si">Index of 3d surface</param>
    /// <param name="sDir">Orientation of surface with respect to Brep</param>
    /// <param name="v0">Index of the A corner vertex</param>
    /// <param name="v1">Index of the B corner vertex</param>
    /// <param name="v2">Index of the C corner vertex</param>
    /// <param name="e0">Index of first edge</param>
    /// <param name="e0Dir">Orientation of edge</param>
    /// <param name="e1">Index of second edge</param>
    /// <param name="e1Dir">Orientation of edge</param>
    /// <param name="e2">Index of third edge</param>
    /// <param name="e2Dir">Orientation of edge</param>
    private static void MakeTrimmedFace(
        Brep brep,
        int si,
        int sDir,
        int v0, 
        int v1,
        int v2,
        int e0,
        int e0Dir,
        int e1,
        int e1Dir,
        int e2,
        int e2Dir
        )
    {
      // Add new face to brep
      var face = brep.Faces.Add(si);

      // Create loop and trims for the face
      MakeTrimmingLoop(
        brep, face,
        e0, e0Dir,
        e1, e1Dir,
        e2, e2Dir
        );

      // Set face direction relative to surface direction
      face.OrientationIsReversed = (sDir == -1);
    }

    /// <summary>
    /// Adds faces to the Brep
    /// </summary>
    /// <param name="brep">The Brep</param>
    private static void CreateFaces(Brep brep)
    {
      MakeTrimmedFace(
        brep,
        ABC_i,   // Index of face
        +1,      // orientation of surface with respect to surface
        A, B, C, // Indices of vertices
        AB, +1,  // Side edge and its orientation with respect to to the trimming curve (AB)
        BC, +1,  // Side edge and its orientation with respect to to the trimming curve (BC)
        AC, -1   // Side edge and its orientation with respect to to the trimming curve (AC)
        );
    }
  }
}
