CRhinoCommand::result CCommandTest::RunCommand(
      const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select curve to divide" );
  go.SetGeometryFilter( CRhinoGetObject::curve_object );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const CRhinoObjRef& objref = go.Object(0);
  const ON_Curve* crv = objref.Curve();
  if( !crv )
    return CRhinoCommand::failure;

  double crv_length = 0.0;
  crv->GetLength( &crv_length );
  if( crv_length < ON_ZERO_TOLERANCE )
    return CRhinoCommand::failure;

  ON_wString s;
  s.Format( L"Curve length is %g. Segment Length", crv_length );

  CRhinoGetNumber gn;
  gn.SetCommandPrompt( s );
  gn.SetLowerLimit( 0.0, TRUE );
  gn.SetUpperLimit( crv_length, TRUE );
  gn.GetNumber();
  if( gn.CommandResult() != CRhinoCommand::success )
    return gn.CommandResult();

  double seg_length = gn.Number();
  int seg_count = (int)floor( crv_length / seg_length );
  double fractional_end = (seg_count * seg_length) / crv_length;

  double t0, t1;
  crv->GetDomain( &t0, &t1 );
  crv->GetNormalizedArcLengthPoint( fractional_end, &t1 );

  seg_count++;
  ON_SimpleArray<double> t( seg_count );
  t.SetCount( seg_count );

  for( int i = 0; i < seg_count; i++ )
  {
    double param = (double)i / ((double)seg_count-1);
    t[i] = param;
  }

  ON_Interval sub_domain( t0, t1 );
  crv->GetNormalizedArcLengthPoints( seg_count,
      (double*)&t[0], (double*)&t[0],
      0.0, 1.0e-8, &sub_domain );

  for( int i = 0; i < seg_count; i++ )
  {
    ON_3dPoint pt = crv->PointAt( t[i] );
    context.m_doc.AddPointObject( pt );
  }

  context.m_doc.Redraw();
  return CRhinoCommand::success;
}
