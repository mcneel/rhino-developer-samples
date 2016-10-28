static bool CreateSurfacesExample( CRhinoDoc& doc )
{
  const int bIsRational = false;
  const int dim = 3;
  const int u_degree = 2;
  const int v_degree = 3;
  const int u_cv_count = 3;
  const int v_cv_count = 5;

  // The knot vectors do NOT have the 2 superfluous knots
  // at the start and end of the knot vector.  If you are
  // coming from a system that has the 2 superfluous knots,
  // just ignore them when creating NURBS surfaces.
  double u_knot[ u_cv_count + u_degree - 1 ];
  double v_knot[ v_cv_count + v_degree - 1 ];

  // make up a quadratic knot vector with no interior knots
  u_knot[0] = u_knot[1] = 0.0;
  u_knot[2] = u_knot[3] = 1.0;

  // make up a cubic knot vector with one simple interior knot
  v_knot[0] = v_knot[1] = v_knot[2] = 0.0;
  v_knot[3] = 1.5;
  v_knot[4] = v_knot[5] = v_knot[6] = 2.0;

  // Rational control points can be in either homogeneous
  // or euclidean form. Non-rational control points do not
  // need to specify a weight.  
  ON_3dPoint CV[u_cv_count][v_cv_count];

  int i, j;
  for ( i = 0; i < u_cv_count; i++ ) {
    for ( j = 0; j < v_cv_count; j++ ) {
      CV[i][j].x = i;
      CV[i][j].y = j;
      CV[i][j].z = i-j;
    }
  }

  ON_NurbsSurface nurbs_surface( dim, bIsRational,
                        u_degree+1, v_degree+1,
                        u_cv_count, v_cv_count );

  for ( i = 0; i < nurbs_surface.KnotCount(0); i++ )
    nurbs_surface.SetKnot( 0, i, u_knot[i] );

  for ( j = 0; j < nurbs_surface.KnotCount(1); j++ )
    nurbs_surface.SetKnot( 1, j, v_knot[j] );

  for ( i = 0; i < nurbs_surface.CVCount(0); i++ ) {
    for ( j = 0; j < nurbs_surface.CVCount(1); j++ ) {
      nurbs_surface.SetCV( i, j, CV[i][j] );
    }
  }

  bool ok = false;
  if ( nurbs_surface.IsValid() )
  {
    doc.AddSurfaceObject( nurbs_surface );
    doc.Redraw();
    ok = true;
  }

  return ok;
}
