CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select edge of surface to extend" );
  go.SetGeometryFilter(CRhinoGetObject::edge_object);
  go.SetGeometryAttributeFilter( CRhinoGetObject::edge_curve );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const CRhinoObjRef& objref = go.Object(0);
  const ON_Surface* srf = objref.Surface();
  if( !srf )
  {
    RhinoApp().Print( L"Unable to extend polysurfaces.\n" );
    return CRhinoCommand::nothing;    
  }

  const ON_Brep* brep = objref.Brep();
  const ON_BrepFace* face = objref.Face();
  if( !brep | !face | face->m_face_index < 0 )
    return CRhinoCommand::failure;

  if( !brep->IsSurface() )
  {
    RhinoApp().Print( L"Unable to extend trimmed surfaces.\n" );
    return CRhinoCommand::nothing;    
  }

  const ON_BrepTrim* trim = objref.Trim();
  if( !trim )
    return CRhinoCommand::failure;

  ON_Surface::ISO edge_index( trim->m_iso );
  int dir = edge_index % 2;
  if( srf->IsClosed(1-dir) )
  {
    RhinoApp().Print(L"Unable to extend surface at seam.\n" );
    return CRhinoCommand::nothing;  
  }
  if( edge_index < ON_Surface::W_iso | edge_index > ON_Surface::N_iso )
  {
    RhinoApp().Print( L"Selected edge must be an underlying surface edge.\n" );
    return CRhinoCommand::nothing;  
  }

  ON_Surface* myface = srf->DuplicateSurface();
  if( !myface )
    return CRhinoCommand::failure;

  bool rc = RhinoExtendSurface( myface, edge_index, 5.0, true);  
  if( rc )
  {
    ON_Brep* mybrep = new ON_Brep();
    mybrep->Create( myface );
    CRhinoBrepObject* obj = new CRhinoBrepObject();
    obj->SetBrep( mybrep );
    context.m_doc.ReplaceObject( CRhinoObjRef(objref.Object()), obj );
    context.m_doc.Redraw();
  }
  return CRhinoCommand::success;
}
