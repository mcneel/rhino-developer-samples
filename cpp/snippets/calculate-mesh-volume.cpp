CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select solid meshes for volume calculation" );
  go.SetGeometryFilter( CRhinoGetObject::mesh_object );
  go.SetGeometryAttributeFilter( CRhinoGetObject::closed_mesh );
  go.EnableSubObjectSelect( FALSE );
  go.EnableGroupSelect();
  go.GetObjects( 1, 0 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  int i;
  ON_SimpleArray<const ON_Mesh*> meshes;
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    if( mesh )
      meshes.Append( mesh );
  }

  const int mesh_count = meshes.Count();
  if( 0 == mesh_count )
    return nothing;

  ON_BoundingBox bbox;
  for( i = 0; i < mesh_count; i++ )
    meshes[i]->GetBoundingBox( bbox, TRUE );
  ON_3dPoint base_point = bbox.Center();

  double total_volume = 0.0;
  double total_error_estimate = 0.0;
  for( i = 0; i < mesh_count; i++ )
  {
    double error_estimate = 0.0;
    double volume = meshes[i]->Volume( base_point, &error_estimate );
    RhinoApp().Print( L"Mesh%d = %f (+/- %f)\n", i, volume, error_estimate );
    total_volume += volume;
    total_error_estimate += error_estimate;
  }

  RhinoApp().Print( L"Total volume = %f (+/- %f)\n",
                    total_volume,
                    total_error_estimate );

  return CRhinoCommand::success;
}
