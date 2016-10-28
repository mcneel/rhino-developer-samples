CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  ON_Plane plane = ON_xy_plane;
  double height = 10.0;
  double radius = 5.0;
  BOOL bCapBottom = FALSE;

  ON_Cone cone( plane, height, radius );
  if( cone.IsValid() )
  {
    ON_Brep* cone_brep = ON_BrepCone( cone, bCapBottom );
    if( cone_brep )
    {
      CRhinoBrepObject* cone_object = new CRhinoBrepObject();
      cone_object->SetBrep( cone_brep );
      context.m_doc.AddObject( cone_object );
      context.m_doc.Redraw();
    }
  }

  return CRhinoCommand::success;
}
