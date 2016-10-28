CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  ON_3dPoint center_point( 0.0, 0.0, 0.0 );
  double radius = 5.0;
  ON_3dPoint height_point( 0.0, 0.0, 10.0 );
  ON_3dVector zaxis = height_point - center_point;
  ON_Plane plane( center_point, zaxis );
  ON_Circle circle( plane, radius );
  ON_Cylinder cylinder( circle, zaxis.Length() );
  ON_Brep* brep = ON_BrepCylinder( cylinder, TRUE, TRUE );
  if( brep )
  {
    CRhinoBrepObject* cylinder_object = new CRhinoBrepObject();
    cylinder_object->SetBrep( brep );
    if( context.m_doc.AddObject(cylinder_object) )
      context.m_doc.Redraw();
    else
      delete cylinder_object;
  }

  return CRhinoCommand::success;
}
