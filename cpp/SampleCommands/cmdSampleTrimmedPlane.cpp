/////////////////////////////////////////////////////////////////////////////
// cmdSampleTrimmedPlane.cpp : command file
//

#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTrimmedPlane command
//

// symbolic vertex index constants to make code more readable
static const int 
  A = 0,
  B = 1,
  C = 2,
  D = 3,
  E = 4;

// symbolic edge index constants to make code more readable
static const int
  AB =  0,
  BC =  1,
  AC =  2;

// symbolic face index constants to make code more readable
static const int
  ABC_i =  0;


static ON_Curve* CreateTrimmingCurve(
        const ON_Surface& s,
        int side // 0 = SW to SE
                  // 1 = SE to NE
                  // 2 = NE to NW
                  // 3 = NW to SW
        )
{
  // A trimming curve is a 2d curve whose image lies in the surface's domain.
  // The "active" portion of the surface is to the left of the trimming curve.
  // An outer trimming loop consists of a simple closed curve running 
  // counter-clockwise around the region it trims.
	// An inner trimming loop consists of a simple closed curve running 
  // clockwise around the region the hole.

  ON_2dPoint from, to;
  double u0, u1, v0, v1;

  s.GetDomain( 0, &u0, &u1 );
  s.GetDomain( 1, &v0, &v1 );

  switch( side ) 
  {
  case 0:  // SW to SE
    from.x = u0; from.y = v0;
    to.x   = u1; to.y   = v0;
    break;
  case 1: // diagonal
    from.x = u1; from.y = v0;
    to.x   = (u0+u1)/2; to.y = v1;
    break;
  case 2: // diagonal
    from.x = (u0+u1)/2; from.y = v1;
    to.x   = u0; to.y   = v0;
    break;
  default:
    return 0;
  }

  ON_Curve* c2d = new ON_LineCurve( from, to );
  c2d->SetDomain(0.0,1.0);

  return c2d;
}

static ON_Curve* CreateLinearCurve( const ON_3dPoint& from, const ON_3dPoint& to )
{
  // creates a 3d line segment to be used as a 3d curve in a ON_Brep
  ON_Curve* c3d = new ON_LineCurve( from, to );
  c3d->SetDomain( 0.0, 1.0 );
  return c3d;
}

static ON_Surface* CreatePlanarSurface( 
        const ON_3dPoint& SW, 
        const ON_3dPoint& SE,
        const ON_3dPoint& NE, 
        const ON_3dPoint& NW
        )
{
  ON_NurbsSurface* pNurbsSurface = new ON_NurbsSurface(
                                        3,     // dimension (>= 1)
                                        FALSE, // not rational
                                        2,     // "u" order (>= 2)
                                        2,     // "v" order (>= 2)
                                        2,     // number of control vertices in "u" dir (>= order)
                                        2      // number of control vertices in "v" dir (>= order)
                                        );
  // corner CVs in counter clockwise order starting in the south west
  pNurbsSurface->SetCV( 0,0, SW );
  pNurbsSurface->SetCV( 1,0, SE );
  pNurbsSurface->SetCV( 1,1, NE );
  pNurbsSurface->SetCV( 0,1, NW );
  // "u" knots
  pNurbsSurface->SetKnot( 0,0, 0.0 );
  pNurbsSurface->SetKnot( 0,1, 1.0 );
  // "v" knots
  pNurbsSurface->SetKnot( 1,0, 0.0 );
  pNurbsSurface->SetKnot( 1,1, 1.0 );

  return pNurbsSurface;
}

static void CreateOneEdge( 
        ON_Brep& brep,
        int vi0, // index of start vertex
        int vi1, // index of end vertex
        int c3i  // index of 3d curve
        )
{
  ON_BrepVertex& v0 = brep.m_V[vi0];
  ON_BrepVertex& v1 = brep.m_V[vi1];
  ON_BrepEdge& edge = brep.NewEdge(v0,v1,c3i);
  edge.m_tolerance = 0.0;  // this simple example is exact - for models with
                           // non-exact data, set tolerance as explained in
                           // definition of ON_BrepEdge.
}

static void CreateEdges( ON_Brep& brep )
{
  // In this simple example, the edge indices exactly match the 3d
  // curve indices.  In general,the correspondence between edge and
  // curve indices can be arbitrary.  It is permitted for multiple
  // edges to use different portions of the same 3d curve.  The 
  // orientation of the edge always agrees with the natural 
  // parametric orientation of the curve.
  
  // edge that runs from A to B
  CreateOneEdge( brep, A, B, AB );
  
  // edge that runs from B to C
  CreateOneEdge( brep, B, C, BC );

  // edge that runs from A to C
  CreateOneEdge( brep, A, C, AC );

}

static int MakeTrimmingLoop(  
        ON_Brep& brep, // returns index of loop
        ON_BrepFace& face,  // face loop is on
        int e0,     // index of first edge
        int e0_dir, // orientation of edge
        int e1,     // index second edgee
        int e1_dir, // orientation of edge
        int e2,     // index third edge
        int e2_dir  // orientation of edge
        )
{
  const ON_Surface& srf = *brep.m_S[face.m_si];

	// Create new loop
  ON_BrepLoop& loop = brep.NewLoop( ON_BrepLoop::outer, face );

  // Create trimming curves running counter clockwise.
	// Note that trims of outer loops run counter clockwise while trims of inner loops (holes) run clockwise.
	// Also note that when trims locate on surface N,S,E or W ends, then trim_iso becomes N_iso, S_iso, E_iso and W_iso respectfully.  
	// While if trim is parallel to surface N,S or E,W, then trim is becomes y_iso and x_iso respectfully, the rest are not_iso.

	// Start at the south side
  ON_Curve* c2 = 0;
  int c2i, ei=0, bRev3d=0;
  ON_Surface::ISO iso = ON_Surface::not_iso;

  for( int side = 0; side < 3; side++ ) 
  {

    c2 = CreateTrimmingCurve( srf, side );

		//Add trimming curve to brep trmming curves array
    c2i = brep.m_C2.Count();
    brep.m_C2.Append(c2);

    switch( side ) 
    {
    case 0: // south
      ei = e0;
      bRev3d = (e0_dir == -1);
      iso = ON_Surface::S_iso;
      break;
    case 1: // diagonal
      ei = e1;
      bRev3d = (e1_dir == -1);
      iso = ON_Surface::not_iso;
      break;
    case 2: // diagonal
      ei = e2;
      bRev3d = (e2_dir == -1);
      iso = ON_Surface::not_iso;
      break;
    }

		//Create new trim topology that references edge, direction reletive to edge, loop and trim curve geometry
    ON_BrepTrim& trim = brep.NewTrim( brep.m_E[ei], bRev3d, loop, c2i );
    trim.m_iso = iso;
    trim.m_type = ON_BrepTrim::boundary; // This one b-rep face, so all trims are boundary ones.
    trim.m_tolerance[0] = 0.0; // This simple example is exact - for models with non-exact
    trim.m_tolerance[1] = 0.0; // data, set tolerance as explained in definition of ON_BrepTrim.
  }

  return loop.m_loop_index;
}

static void MakeTrimmedFace( ON_Brep& brep,
        int si,      // index of 3d surface
        int s_dir,   // orientation of surface with respect to surfce
        int e0,     // index of first edge
        int e0_dir,  // orientation of edge
        int e1,     // index of second edge
        int e1_dir,  // orientation of edge
        int e2,     // index of third edge
        int e2_dir  // orientation of edge
        )
{
	//Add new face to brep
  ON_BrepFace& face = brep.NewFace(si);

	//Create loop and trims for the face
  MakeTrimmingLoop( brep, face,
                e0, e0_dir, 
                e1, e1_dir, 
                e2, e2_dir 
                );

	//Set face direction relative to surface direction
  face.m_bRev = (s_dir == -1);
}

static void CreateFaces( ON_Brep& brep )
{
  MakeTrimmedFace( brep,
    ABC_i,      // Index of face
    +1,         // orientation of surface with respect to surface
    AB,+1,      // Side edge and its orientation with respect to
                // to the trimming curve.  (AB)
    BC,+1,      // Side edge and its orientation with respect to
                // to the trimming curve.  (BC)
    AC,-1       // Side edge and its orientation with respect to
                // to the trimming curve   (AC)
    );
}


static ON_Brep* MakeTrimmedPlane( ON_TextLog& error_log )
{
  // This example demonstrates how to construct a ON_Brep
  // with the topology shown below.
	//
	//
  //    E-------C--------D
  //    |       /\       | 
  //    |      /  \      |
  //    |     /    \     |
  //    |    e2      e1  |     
  //    |   /        \   |    
  //    |  /          \  |  
  //    | /            \ |  
  //    A-----e0-------->B
  //
  //
  //  Things need to be defined in a valid brep:
	//   1- Vertices
	//   2- 3D Curves (geometry)
	//   3- Edges (topology - reference curve geometry)
	//   4- Surface (geometry)
	//   5- Faces (topology - reference surface geometry)
	//   6- Loops (2D parameter space of faces)
	//   4- Trims and 2D curves (2D parameter space of edges)
	//

  ON_3dPoint point[5] = {
    ON_3dPoint( 0.0, 0.0, 0.0 ),   // point A = geometry for vertex 0 (and surface SW corner)
    ON_3dPoint( 10.0, 0.0, 0.0 ),  // point B = geometry for vertex 1 (and surface SE corner)
    ON_3dPoint( 5.0, 10.0, 0.0 ),  // point C = geometry for vertex 2
		ON_3dPoint( 10.0, 10.0, 0.0 ), // point D (surface NE corner)
		ON_3dPoint( 0.0, 10.0, 0.0 ),  // point E (surface NW corner)

  };

  ON_Brep* brep = new ON_Brep();

  // create three vertices located at the three points
  int vi;
  for ( vi = 0; vi < 3; vi++ ) {
    ON_BrepVertex& v = brep->NewVertex(point[vi]);
    v.m_tolerance = 0.0; // this simple example is exact - for models with
                         // non-exact data, set tolerance as explained in
                         // definition of ON_BrepVertex.
  }

  // Create 3d curve geometry - the orientations are arbitrarily chosen
  // so that the end vertices are in alphabetical order.
  brep->m_C3.Append( CreateLinearCurve( point[A], point[B] ) ); // line AB
  brep->m_C3.Append( CreateLinearCurve( point[B], point[C] ) ); // line BC
  brep->m_C3.Append( CreateLinearCurve( point[A], point[C] ) ); // line CD

  // Create edge topology for each curve in the brep.
  CreateEdges( *brep );

  // Create 3d surface geometry - the orientations are arbitrarily chosen so
  // that some normals point into the cube and others point out of the cube.
  brep->m_S.Append( CreatePlanarSurface( point[A], point[B], point[D], point[E] ) ); // ABDE

  // Create face topology and 2d parameter space loops and trims.
  CreateFaces( *brep );

	// Make sure b-rep is valid
  if ( !brep->IsValid() ) 
  {
    error_log.Print("Trimmed b-rep face is not valid.\n");
    delete brep;
    brep = 0;
  }

  return brep;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleTrimmedPlane : public CRhinoCommand
{
public:
  CCommandSampleTrimmedPlane() {}
  ~CCommandSampleTrimmedPlane() {}
  UUID CommandUUID()
  {
    // {A74D1534-29D7-4B14-A9FD-B3C679839D74}
    static const GUID SampleTrimmedPlaneCommand_UUID =
    { 0xA74D1534, 0x29D7, 0x4B14, { 0xA9, 0xFD, 0xB3, 0xC6, 0x79, 0x83, 0x9D, 0x74 } };
    return SampleTrimmedPlaneCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleTrimmedPlane"; }
  const wchar_t* LocalCommandName() { return L"SampleTrimmedPlane"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleTrimmedPlane object
static class CCommandSampleTrimmedPlane theSampleTrimmedPlaneCommand;

CRhinoCommand::result CCommandSampleTrimmedPlane::RunCommand( const CRhinoCommandContext& context )
{
	ON_TextLog error_log;
  ON_Brep* brep = MakeTrimmedPlane( error_log );
  if( 0 == brep )
		return CRhinoCommand::failure;

  CRhinoBrepObject* brep_object = new CRhinoBrepObject();
  brep_object->SetBrep( brep );
  context.m_doc.AddObject( brep_object );
  context.m_doc.Redraw();	
 
  return CRhinoCommand::success;
}

//
// END SampleTrimmedPlane command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
