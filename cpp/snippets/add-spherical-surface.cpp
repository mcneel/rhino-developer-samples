CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoCommand::result rc = CRhinoCommand::cancel;

  ON_3dPoint center( 0.0, 0.0, 0.0 );
  double radius = 5.0;
  ON_Sphere sphere( center, radius );
  ON_RevSurface* sphere_srf = sphere.RevSurfaceForm();
  if( !sphere_srf )
    return rc;

  CRhinoSurfaceObject* sphere_obj = new CRhinoSurfaceObject();
  sphere_obj->SetSurface( sphere_srf );

  if( context.m_doc.AddObject(sphere_obj) )
  {
    context.m_doc.Redraw();
    rc = CRhinoCommand::success;
  }
  else
  {
    delete sphere_obj;
    sphere_obj = NULL;
  }

  return rc;
}
