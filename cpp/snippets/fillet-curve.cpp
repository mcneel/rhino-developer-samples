CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  ON_3dPoint point0( 10.0,  0.0, 0.0 );
  ON_3dPoint point1( 10.0, 10.0, 0.0 );
  ON_3dPoint point2(  0.0, 10.0, 0.0 );

  // Create the line curves to fillet
  ON_LineCurve curve0( point0, point1 );
  ON_LineCurve curve1( point2, point1 );

  // Fillet at the end points of the line curves
  double curve0_t = curve0.Domain().Max();
  double curve1_t = curve1.Domain().Max();

  // Fillet radius
  double radius = 1.0;

  // Do the fillet calculation
  double t0 = 0.0, t1 = 0.0;
  ON_Plane plane;
  if( RhinoGetFilletPoints(curve0, curve1, radius, curve0_t, curve1_t, t0, t1, plane) )
  {
    // Trim back the two line curves
    ON_Interval domain0( curve0.Domain().Min(), t0 );
    curve0.Trim( domain0 );

    ON_Interval domain1( curve1.Domain().Min(), t1 );
    curve1.Trim( domain1 );

    // Compute the fillet curve
    ON_3dVector radial0 = curve0.PointAt(t0) - plane.Origin();
    radial0.Unitize();

    ON_3dVector radial1 = curve1.PointAt(t1) - plane.Origin();
    radial1.Unitize();

    double angle = acos( radial0 * radial1 );
    ON_Plane fillet_plane( plane.Origin(), radial0, radial1 );
    ON_Arc fillet( fillet_plane, plane.Origin(), radius, angle );

    // Add the geometry
    context.m_doc.AddCurveObject( curve0 );
    context.m_doc.AddCurveObject( curve1 );
    context.m_doc.AddCurveObject( fillet );

    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}
