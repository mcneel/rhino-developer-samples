CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select curve for evaluation" );
  go.SetGeometryFilter( CRhinoGetObject::curve_object );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  const ON_Curve* crv = go.Object(0).Curve();
  if( 0 == crv )
    return failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt( L"Pick point on curve for evaluation" );
  gp.Constrain( *crv );
  gp.GetPoint();
  if( gp.CommandResult() != success )
    return gp.CommandResult();

  double t = 0.0;
  if( 0 == gp.PointOnCurve(&t) )
    return nothing;

  CRhinoGetInteger gi;
  gi.SetCommandPrompt( L"Number of derivatives to calculate" );
  gi.SetDefaultInteger( 1 );
  gi.SetLowerLimit( 0 );
  gi.SetUpperLimit( 4 );
  gi.GetNumber();
  if( gi.CommandResult() != success )
    return gi.CommandResult();

  int der_count = gi.Number();

  // Allocate memory for results
  int v_stride = 3;
  int v_count = v_stride * ( der_count + 1 );
  ON_SimpleArray<double> v_array( v_count );
  v_array.SetCount( v_count );

  // Do the curve evaluation
  if( crv->Evaluate(t, der_count, v_stride, v_array.Array()) )
  {
    const double* v = v_array.Array();
    int i;
    for( i = 0; i < der_count + 1; i++ )
    {
      RhinoApp().Print( L"Derivative %d = (%.3f,%.3f,%.3f)\n", i, v[0], v[1], v[2] );
      v += v_stride;
    }
  }
  else
  {
    RhinoApp().Print( L"Failed to evaluate curve.\n" );
  }

  return success;
}
