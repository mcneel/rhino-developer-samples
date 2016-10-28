CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select first set of polysurfaces" );
  go.SetGeometryFilter( CRhinoGetObject::polysrf_object );
  go.EnablePreSelect( TRUE );
  go.GetObjects( 1, 0 );
  if( success != go.CommandResult() )
    return go.CommandResult();

  ON_SimpleArray<const ON_Brep*> InBreps0( go.ObjectCount() );
  int i;
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    const ON_Brep* brep = go.Object(i).Brep();
    if( brep )
      InBreps0.Append( brep );
  }

  go.SetCommandPrompt( L"Select second set of polysurfaces" );
  go.EnablePreSelect( FALSE );
  go.EnableDeselectAllBeforePostSelect( false );
  go.GetObjects( 1, 0 );
  if( success != go.CommandResult() )
    return go.CommandResult();

  ON_SimpleArray<const ON_Brep*> InBreps1( go.ObjectCount() );
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    const ON_Brep* brep = go.Object(i).Brep();
    if( brep )
      InBreps1.Append( brep );
  }

  ON_SimpleArray<ON_Brep*> OutBreps;
  ON_SimpleArray<int> InputIndexForOutput;
  bool something_happened = false;
  double tolerance = context.m_doc.AbsoluteTolerance();

  bool rc = RhinoBooleanDifference(
        InBreps0,
        InBreps1,
        tolerance,
        &something_happened,
        OutBreps,
        InputIndexForOutput
        );

  if( !rc | !something_happened )
  {
    for( i = 0; i < OutBreps.Count(); i++ )
    {
      delete OutBreps[i];
      OutBreps[i] = 0;
    }
    return nothing;
  }

  for( i = 0; i < OutBreps.Count(); i++ )
  {
    ON_Brep* brep = OutBreps[i];
    if( brep )
    {
      context.m_doc.AddBrepObject( *brep );
      brep = 0;
      delete OutBreps[i];
      OutBreps[i] = 0;
    }
  }

  context.m_doc.Redraw();
  return success;
}
