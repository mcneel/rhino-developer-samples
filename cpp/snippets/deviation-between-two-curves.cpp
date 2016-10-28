CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select curves to test" );
  go.SetGeometryFilter( ON::curve_object );
  go.EnableSubObjectSelect( true );
  go.AcceptNothing();
  go.GetObjects( 2, 2 );
  if( go.CommandResult() != success )
    return go.CommandResult();
  if( go.ObjectCount() != 2 )
    return failure;

  const ON_Curve* crv_a = go.Object(0).Curve();
  if( !crv_a )
    return failure;

  const ON_Curve* crv_b = go.Object(1).Curve();
  if( !crv_b )
    return failure;

  double tol = context.m_doc.AbsoluteTolerance();
  int count = 0;
  double int_a[3][2];
  double int_b[3][2];
  double max_a, max_b, max_d;
  double min_a, min_b, min_d;

  bool rc = RhinoGetOverlapDistance(
        crv_a,  // curve A
        0,      // optional domain for curve A
        crv_b,  // curve B
        0,      // optional domain for curve B
        tol,    // tolerance for common normal detection
        0.0,    // limits acceptable overlap distance
        &count, // overlap interval count (0 to 3)
        int_a,  // curve A interval(s)
        int_b,  // curve B interval(s)
        &max_a, // curve B parameter at maximum overlap distance point
        &max_b, // curve B parameter at maximum overlap distance point
        &max_d, // maximum overlap distance
        &min_a, // curve A parameter at minimum distance point
        &min_b, // curve B parameter at minimum distance point
        &min_d  // minimum distance between curves
        );

  if( min_d <= ON_ZERO_TOLERANCE )
    min_d = 0.0;

  if( rc )
  {
    if( count == 0 )
    {
      RhinoApp().Print( L"Curves do not overlap.\n" );
    }
    else
    {
      context.m_doc.AddPointObject( crv_a->PointAt(max_a) );
      context.m_doc.AddPointObject( crv_b->PointAt(max_b) );
      context.m_doc.AddPointObject( crv_a->PointAt(min_a) );
      context.m_doc.AddPointObject( crv_b->PointAt(min_b) );
      context.m_doc.Redraw();

      int i;
      for( i = 0; i < count; i++ )
      {
        ON_3dPoint sp_a = crv_a->PointAt( int_a[i][0] );
        ON_3dPoint sp_b = crv_b->PointAt( int_b[i][0] );
        double sd = sp_a.DistanceTo( sp_b );

        ON_3dPoint ep_a = crv_a->PointAt( int_a[i][1] );
        ON_3dPoint ep_b = crv_b->PointAt( int_b[i][1] );
        double ed = ep_a.DistanceTo( ep_b );

        RhinoApp().Print( L"Overlap interval %d of %d:\n", i+1, count );
        RhinoApp().Print( L"          start: distance = %g\n", sd );
        RhinoApp().Print( L"                 crvA(%10g) = (%g, %g, %g)\n",
                          int_a[i][0], sp_a.x, sp_a.y, sp_a.z );
        RhinoApp().Print( L"                 crvB(%10g) = (%g, %g, %g)\n",
                          int_b[i][0], sp_b.x, sp_b.y, sp_b.z );
        RhinoApp().Print( L"            end: distance = %g\n", ed );
        RhinoApp().Print( L"                 crvA(%10g) = (%g, %g, %g)\n",
                          int_a[i][1], ep_a.x, ep_a.y, ep_a.z );
        RhinoApp().Print( L"                 crvB(%10g) = (%g, %g, %g)\n",
                          int_b[i][1], ep_b.x, ep_b.y, ep_b.z );
      }

      RhinoApp().Print( L"Minimum deviation = %g\n", min_d );
      RhinoApp().Print( L"Maximum deviation = %g\n", max_d );
    }
  }
  else
  {
    RhinoApp().Print( L"Unable to find overlap intervals.\n" );
  }

  return success;
}
