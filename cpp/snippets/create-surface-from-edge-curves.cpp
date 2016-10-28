CRhinoCommand::result CCommandTest::RunCommand(const CRhinoCommandContext& context)
{
  // Pick four curve objects
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select 4 curves" );
  go.SetGeometryFilter( CRhinoGetObject::curve_object);
  go.GetObjects( 4, 4 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  // Validate results
  int i, count = go.ObjectCount();
  if( count != 4 )
    return CRhinoCommand::failure;

  ON_NurbsCurve nc[4];
  // Get nurb form of each curve
  for( i = 0; i < count; i++)
  {
    const ON_Curve* crv = go.Object(i).Curve();
    if( !crv )
      return CRhinoCommand::failure;
    if( !crv->GetNurbForm(nc[i]) )
      return CRhinoCommand::failure;
  }

  // Create the surface
  ON_Brep* brep = RhinoCreateEdgeSrf( 4, nc );
  if( !brep )
  {
    RhinoApp().Print( L"Unable to create surface.\n" );
    return CRhinoCommand::failure;
  }

  // Ready new brep object
  CRhinoBrepObject* obj = new CRhinoBrepObject;
  obj->SetBrep( brep );

  // Add new objet to document
  if( !context.m_doc.AddObject( obj ) )
  {
    delete obj;
    RhinoApp().Print( L"Unable to create surface.\n" );
    return CRhinoCommand::failure;
  }

  context.m_doc.Redraw();
  return CRhinoCommand::success;
}
