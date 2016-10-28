CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select curve" );
  go.SetGeometryFilter( CRhinoGetObject::curve_object );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const ON_Curve* crv = go.Object(0).Curve();
  if( 0 == crv )
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt( L"Point on surface" );
  gp.Constrain( *crv );
  gp.GetPoint();
  if( gp.CommandResult() != CRhinoCommand::success )
    return gp.CommandResult();

  ON_3dPoint pt = gp.Point();

  double t = 0.0;
  if( crv->GetClosestPoint(pt, &t) )
  {
    ON_Interval domain = crv->Domain();
    ON_Interval sub_domain( domain.Min(), t );
    double length = 0.0;
    if( crv->GetLength(&length, 0.0, &sub_domain) )
      RhinoApp().Print( L"Distance from start of curve = %f.\n", length );
  }

  return CRhinoCommand::success;
}
