using Rhino.Geometry;

/* Demonstrates how to construct a Brep with the topology shown below.
           H-------e6-------G
          /                /|
         / |              / |
        /  e7            /  e5
       /   |            /   |
      /                e10  |
     /     |          /     |
    e11    E- - e4- -/- - - F
   /                /      /
  /      /         /      /
 D---------e2-----C      e9
 |     /          |     /
 |    e8          |    /
 e3  /            e1  /
 |                |  /
 | /              | /
 |                |/
 A-------e0-------B     */

partial class Examples
{
  // Symbolic vertex index constants to make code more readable
  const int A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7;

  // Symbolic edge index constants to make code more readable
  const int AB = 0, BC = 1, CD = 2, AD = 3, EF = 4, FG = 5, GH = 6,
    EH = 7, AE = 8, BF = 9, CG = 10, DH = 11;

  // Symbolic face index constants to make code more readable
  const int ABCD = 0, BCGF = 1, CDHG = 2, ADHE = 3, ABFE = 4, EFGH = 5;


  public static Rhino.Commands.Result AddTwistedCube(Rhino.RhinoDoc doc)
  {
    Point3d[] points = new Point3d[8];
    points[0] = new Point3d(0.0, 0.0, 0.0);   // point A = geometry for vertex 0
    points[1] = new Point3d(10.0, 0.0, 0.0);  // point B = geometry for vertex 1
    points[2] = new Point3d(10.0, 8.0, -1.0); // point C = geometry for vertex 2
    points[3] = new Point3d(0.0, 6.0, 0.0);   // point D = geometry for vertex 3
    points[4] = new Point3d(1.0, 2.0, 11.0);  // point E = geometry for vertex 4
    points[5] = new Point3d(10.0, 0.0, 12.0); // point F = geometry for vertex 5
    points[6] = new Point3d(10.0, 7.0, 13.0); // point G = geometry for vertex 6
    points[7] = new Point3d(0.0, 6.0, 12.0);  // point H = geometry for vertex 7

    Brep brep = new Brep();

    // Create eight vertices located at the eight points
    for (int vi = 0; vi < 8; vi++)
      brep.Vertices.Add(points[vi], 0.0);

    // Create 3d curve geometry - the orientations are arbitrarily chosen
    // so that the end vertices are in alphabetical order.
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[A], points[B])); // line AB
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[B], points[C])); // line BC
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[C], points[D])); // line CD
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[A], points[D])); // line AD
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[E], points[F])); // line EF
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[F], points[G])); // line FG
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[G], points[H])); // line GH
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[E], points[H])); // line EH
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[A], points[E])); // line AE
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[B], points[F])); // line BF
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[C], points[G])); // line CG
    brep.Curves3D.Add(TwistedCubeEdgeCurve(points[D], points[H])); // line DH

    // Create the 12 edges that connect the corners of the cube.
    MakeTwistedCubeEdges(ref brep);

    // Create 3d surface geometry - the orientations are arbitrarily chosen so
    // that some normals point into the cube and others point out of the cube.
    brep.AddSurface(TwistedCubeSideSurface(points[A], points[B], points[C], points[D])); // ABCD
    brep.AddSurface(TwistedCubeSideSurface(points[B], points[C], points[G], points[F])); // BCGF
    brep.AddSurface(TwistedCubeSideSurface(points[C], points[D], points[H], points[G])); // CDHG
    brep.AddSurface(TwistedCubeSideSurface(points[A], points[D], points[H], points[E])); // ADHE
    brep.AddSurface(TwistedCubeSideSurface(points[A], points[B], points[F], points[E])); // ABFE
    brep.AddSurface(TwistedCubeSideSurface(points[E], points[F], points[G], points[H])); // EFGH

    // Create the CRhinoBrepFaces
    MakeTwistedCubeFaces(ref brep);

    if (brep.IsValid)
    {
      doc.Objects.AddBrep(brep);
      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }
    return Rhino.Commands.Result.Failure;
  }

  static Curve TwistedCubeEdgeCurve(Point3d from, Point3d to)
  {
    // Create a 3d line segment to be used as a 3d curve in a Brep
    return new LineCurve(from, to) { Domain = new Interval(0, 1) };
  }

  static void MakeTwistedCubeEdges(ref Brep brep)
  {
    // In this simple example, the edge indices exactly match the 3d curve
    // indices.  In general,the correspondence between edge and curve indices
    // can be arbitrary.  It is permitted for multiple edges to use different
    // portions of the same 3d curve.  The orientation of the edge always
    // agrees with the natural parametric orientation of the curve.
    brep.Edges.Add(A, B, AB, 0); // Edge that runs from A to B
    brep.Edges.Add(B, C, BC, 0); // Edge that runs from B to C
    brep.Edges.Add(C, D, CD, 0); // Edge that runs from C to D
    brep.Edges.Add(A, D, AD, 0); // Edge that runs from A to D
    brep.Edges.Add(E, F, EF, 0); // Edge that runs from E to F
    brep.Edges.Add(F, G, FG, 0); // Edge that runs from F to G
    brep.Edges.Add(G, H, GH, 0); // Edge that runs from G to H
    brep.Edges.Add(E, H, EH, 0); // Edge that runs from E to H
    brep.Edges.Add(A, E, AE, 0); // Edge that runs from A to E
    brep.Edges.Add(B, F, BF, 0); // Edge that runs from B to F
    brep.Edges.Add(C, G, CG, 0); // Edge that runs from C to G
    brep.Edges.Add(D, H, DH, 0); // Edge that runs from D to H
  }

  static Surface TwistedCubeSideSurface(Point3d southwest, Point3d southeast, Point3d northeast, Point3d northwest)
  {
    var ns = NurbsSurface.Create(3, false, 2, 2, 2, 2);

    // Corner CVs in counter clockwise order starting in the south west
    ns.Points.SetControlPoint(0, 0, southwest);
    ns.Points.SetControlPoint(1, 0, southeast);
    ns.Points.SetControlPoint(1, 1, northeast);
    ns.Points.SetControlPoint(0, 1, northwest);

    // "u" knots
    ns.KnotsU[0] = 0.0;
    ns.KnotsU[1] = 1.0;

    // "v" knots
    ns.KnotsV[0] = 0;
    ns.KnotsV[1] = 1;
    return ns;
  }

  static void MakeTwistedCubeFaces(ref Brep brep)
  {
    MakeTwistedCubeFace(ref brep,
      ABCD,       // Index of surface ABCD
      +1, // Indices of vertices listed in SW,SE,NW,NE order
      AB, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (AB)
      BC, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (BC)
      CD, +1,     // South side edge and its orientation with respect to
      // to the trimming curve   (CD)
      AD, -1      // South side edge and its orientation with respect to
      // to the trimming curve   (AD)
      );

    MakeTwistedCubeFace(ref brep,
      BCGF,       // Index of surface BCGF
      -1, // Indices of vertices listed in SW,SE,NW,NE order
      BC, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (BC)
      CG, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (CG)
      FG, -1,     // South side edge and its orientation with respect to
      // to the trimming curve   (FG)
      BF, -1      // South side edge and its orientation with respect to
      // to the trimming curve   (BF)
      );

    MakeTwistedCubeFace(ref brep,
      CDHG,       // Index of surface CDHG
      -1, // Indices of vertices listed in SW,SE,NW,NE order
      CD, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (CD)
      DH, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (DH)
      GH, -1,     // South side edge and its orientation with respect to
      // to the trimming curve   (GH)
      CG, -1      // South side edge and its orientation with respect to
      // to the trimming curve   (CG)
      );

    MakeTwistedCubeFace(ref brep,
      ADHE,       // Index of surface ADHE
      +1, // Indices of vertices listed in SW,SE,NW,NE order
      AD, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (AD)
      DH, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (DH)
      EH, -1,     // South side edge and its orientation with respect to
      // to the trimming curve   (EH)
      AE, -1      // South side edge and its orientation with respect to
      // to the trimming curve   (AE)
      );

    MakeTwistedCubeFace(ref brep,
      ABFE,       // Index of surface ABFE
      -1, // Indices of vertices listed in SW,SE,NW,NE order
      AB, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (AB)
      BF, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (BF)
      EF, -1,     // South side edge and its orientation with respect to
      // to the trimming curve   (EF)
      AE, -1      // South side edge and its orientation with respect to
      // to the trimming curve   (AE)
      );

    MakeTwistedCubeFace(ref brep,
      EFGH,       // Index of surface EFGH
      -1, // Indices of vertices listed in SW,SE,NW,NE order
      EF, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (EF)
      FG, +1,     // South side edge and its orientation with respect to
      // to the trimming curve.  (FG)
      GH, +1,     // South side edge and its orientation with respect to
      // to the trimming curve   (GH)
      EH, -1      // South side edge and its orientation with respect to
      // to the trimming curve   (EH)
      );
  }

  static void MakeTwistedCubeFace( ref Brep brep, int surfaceIndex,
    int s_dir,   // Indices of corner vertices listed in SW, SE, NW, NE order
    int southEdgeIndex,
    int eS_dir,  // orientation of edge with respect to surface trim
    int eastEdgeIndex,
    int eE_dir,  // orientation of edge with respect to surface trim
    int northEdgeIndex,
    int eN_dir,  // orientation of edge with respect to surface trim
    int westEdgeIndex,
    int eW_dir   // orientation of edge with respect to surface trim
    )
  {
    var face = brep.Faces.Add(surfaceIndex);

    MakeTwistedCubeTrimmingLoop(
      ref brep,
      ref face,
      southEdgeIndex, eS_dir,
      eastEdgeIndex, eE_dir,
      northEdgeIndex, eN_dir,
      westEdgeIndex, eW_dir
      );

    face.OrientationIsReversed = (s_dir == -1);
  }

  static void MakeTwistedCubeTrimmingLoop( ref Brep brep, ref BrepFace face, // Indices of corner vertices listed in SW, SE, NW, NE order
  int eSi,     // index of edge on south side of surface
  int eS_dir,  // orientation of edge with respect to surface trim
  int eEi,     // index of edge on south side of surface
  int eE_dir,  // orientation of edge with respect to surface trim
  int eNi,     // index of edge on south side of surface
  int eN_dir,  // orientation of edge with respect to surface trim
  int eWi,     // index of edge on south side of surface
  int eW_dir   // orientation of edge with respect to surface trim
                               )
  {
    Surface srf = brep.Surfaces[face.SurfaceIndex];
    var loop = brep.Loops.Add(BrepLoopType.Outer, face);

    // Create trimming curves running counter clockwise around the surface's domain.
    // Start at the south side
    // side: 0=south, 1=east, 2=north, 3=west
    for (int side = 0; side < 4; side++)
    {
      Curve trimming_curve = TwistedCubeTrimmingCurve(srf, side);
      int curve_index = brep.Curves2D.Add(trimming_curve);

      int ei = 0;
      bool reverse = false;
      IsoStatus iso = IsoStatus.None;
      switch (side)
      {
        case 0: // south
          ei = eSi;
          reverse = (eS_dir == -1);
          iso = IsoStatus.South;
          break;
        case 1: // east
          ei = eEi;
          reverse = (eE_dir == -1);
          iso = IsoStatus.East;
          break;
        case 2: // north
          ei = eNi;
          reverse = (eN_dir == -1);
          iso = IsoStatus.North;
          break;
        case 3: // west
          ei = eWi;
          reverse = (eW_dir == -1);
          iso = IsoStatus.West;
          break;
      }

      BrepEdge edge = brep.Edges[ei];
      BrepTrim trim = brep.Trims.Add(edge, reverse, loop, curve_index);
      trim.IsoStatus = iso;
      trim.TrimType = BrepTrimType.Mated; // This b-rep is closed, so all trims have mates.
      trim.SetTolerances(0, 0); // This simple example is exact - for models with
      // non-exact data, set tolerance as explained in
      // definition of BrepTrim.
    }
  }

  static Curve TwistedCubeTrimmingCurve(Surface s, int side // 0 = SW to SE
    // 1 = SE to NE
    // 2 = NE to NW
    // 3 = NW to SW
    )
  {
    // A trimming curve is a 2d curve whose image lies in the surface's domain.
    // The "active" portion of the surface is to the left of the trimming curve.
    // An outer trimming loop consists of a simple closed curve running 
    // counter-clockwise around the region it trims.
    var u_domain = s.Domain(0);
    var v_domain = s.Domain(1);
    double u0 = u_domain[0];
    double u1 = u_domain[1];
    double v0 = v_domain[0];
    double v1 = v_domain[1];

    Point2d from;
    Point2d to;

    switch (side)
    {
      case 0:  // SW to SE
        from = new Point2d(u0, v0);
        to = new Point2d(u1, v0);
        break;
      case 1: // SE to NE
        from = new Point2d(u1, v0);
        to = new Point2d(u1, v1);
        break;
      case 2: // NE to NW
        from = new Point2d(u1, v1);
        to = new Point2d(u0, v1);
        break;
      case 3: // NW to SW
        from = new Point2d(u0, v1);
        to = new Point2d(u0, v0);
        break;
      default:
        return null;
    }

    return new LineCurve(from, to) { Domain = new Interval(0, 1) };
  }

}
