CRhinoCommand::result CCommandTest::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select curve to divide" );
  go.SetGeometryFilter( CRhinoGetObject::curve_object );
  go.GetObjects( 1, 1 );

  if( go.Result() == CRhinoGet::cancel )
    return CRhinoCommand::cancel;

  if( go.Result() != CRhinoGet::object | go.ObjectCount() <= 0 )
    return CRhinoCommand::failure;

  CRhinoObjRef& objref = go.Object(0);
  const ON_Curve* crv = objref.Curve();
  if( !crv )
    return CRhinoCommand::failure;

  CRhinoGetInteger gi;
  gi.SetCommandPrompt( L"Number of segments" );
  gi.SetDefaultInteger( 2 );
  gi.SetLowerLimit( 2 );
  gi.SetUpperLimit( 100 );
  gi.GetInteger();

  if( gi.Result() == CRhinoGet::cancel )
    return CRhinoCommand::cancel;

  if( gi.Result() != CRhinoGet::number )
    return CRhinoCommand::failure;

  int count = gi.Number();
  count++;
  ON_SimpleArray<double> t( count );
  t.SetCount( count );

  int i;
  for( i = 0; i < count; i++ )
  {
    double param = (double)i / ((double)count-1);
    t[i] = param;
  }
  if( crv->GetNormalizedArcLengthPoints(count, (double*)&t[0], (double*)&t[0]) )
  {
    for( i = 0; i < count; i++ )
    {
      ON_3dPoint pt = crv->PointAt( t[i] );
      context.m_doc.AddPointObject( pt );
    }
    context.m_doc.Redraw();
  }
  return CRhinoCommand::success;
}
