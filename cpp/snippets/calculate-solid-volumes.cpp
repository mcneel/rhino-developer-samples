CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select solids for volume calculation" );
  go.SetGeometryFilter(
        CRhinoGetObject::surface_object |
        CRhinoGetObject::polysrf_object |
        CRhinoGetObject::mesh_object
        );
  go.SetGeometryAttributeFilter(
        CRhinoGetObject::closed_surface |
        CRhinoGetObject::closed_polysrf |
        CRhinoGetObject::closed_mesh
        );
  go.EnableSubObjectSelect( false );
  go.EnableGroupSelect( true );
  go.GetObjects( 1, 0 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  ON_SimpleArray<const ON_Geometry*> geom( go.ObjectCount() );
  int i;
  for( i = 0; i < go.ObjectCount(); i++ )
  {
    const ON_Geometry* geo = go.Object(i).Geometry();
    if( 0 == geo )
      return failure;
    geom.Append( geo );
  }

  // Get bounding box of all objects
  ON_BoundingBox bbox;
  for( i = 0; i < geom.Count(); i++ )
    geom[i]->GetBoundingBox( bbox, bbox.IsValid() );

  ON_3dPoint base_point = bbox.Center();

  ON_SimpleArray<ON_MassProperties> MassProp;
  MassProp.Reserve( geom.Count() );

  for( i = 0; i < geom.Count(); i++ )
  {
    ON_MassProperties* mp = &MassProp.AppendNew();

    if( const ON_Surface* srf = ON_Surface::Cast(geom[i]) )
      srf->VolumeMassProperties( *mp, true, false, false, false, base_point );       

    else if( const ON_Brep* brep = ON_Brep::Cast(geom[i]) )
      brep->VolumeMassProperties( *mp, true, false, false, false, base_point );

    else if( const ON_Mesh* mesh = ON_Mesh::Cast(geom[i]) )
      mesh->VolumeMassProperties( *mp, true, false, false, false, base_point );
  }

  ON_MassProperties results;
  results.Sum( MassProp.Count(), MassProp.Array() );
  RhinoApp().Print( L"Volume = %g\n", results.Volume() );

  return success;
}
