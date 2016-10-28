CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  int i = 0;

  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select first set of meshes" );
  go.SetGeometryFilter( CRhinoGetObject::mesh_object );
  go.GetObjects( 1, 0 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  ON_SimpleArray<const ON_Mesh*> InMeshes0( go.ObjectCount() );
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    if( 0 == mesh )
      return CRhinoCommand::failure;
    InMeshes0.Append( mesh );
  }

  go.SetCommandPrompt( L"Select second set of meshes" );
  go.EnablePreSelect( false );
  go.EnableDeselectAllBeforePostSelect( false );
  go.GetObjects( 1, 0 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  ON_SimpleArray<const ON_Mesh*> InMeshes1( go.ObjectCount() );
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    if( 0 == mesh )
      return CRhinoCommand::failure;
    InMeshes1.Append( mesh );
  }

  double intersection_tolerance = ON_SQRT_EPSILON * 10;
  double overlap_tolerance = ON_SQRT_EPSILON * 10;

  ON_SimpleArray<ON_Mesh*> OutMeshes;
  bool bSomethingHappened = false;
  bool rc = RhinoMeshBooleanIntersection(
        InMeshes0,
        InMeshes1,
        intersection_tolerance,
        overlap_tolerance,
        &bSomethingHappened,
        OutMeshes
        );

  if( !rc )
  {
    RhinoApp().Print( L"No mesh intersections found.\n" );
    return CRhinoCommand::nothing;
  }

  for( i = 0; i < OutMeshes.Count(); i++ )
  {
    CRhinoMeshObject* mesh_obj = new CRhinoMeshObject();
    mesh_obj->SetMesh( OutMeshes[i] );

    OutMeshes[i] = 0;

    if( context.m_doc.AddObject(mesh_obj) )
      mesh_obj->Select( true );
    else
      delete mesh_obj;
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}
