CRhinoCommand::result CCommandTest::RunCommand(
      const CRhinoCommandContext& context)
{
  // Allow for picking of either a surface or a brep face.
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select surface to attach data" );
  go.SetGeometryFilter( CRhinoGetObject::surface_object );
  go.EnableSubObjectSelect( TRUE );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  // Get first object reference. Note, this reference
  // reprsents the picked surface or face, not the
  // top level brep.
  const CRhinoObjRef& objref = go.Object(0);

  // Get face on brep that was picked
  const ON_BrepFace* face = objref.Face();
  if( !face )
    return failure;

  // Get the faces underlying surface
  const ON_Surface* srf = face->SurfaceOf();
  if( !srf )
    return failure;

  // Query surface for user data
  CTestUserData* ud = CTestUserData::Cast( srf->GetUserData(ud->m_uuid) );
  if( ud )
  {
    RhinoApp().Print( L"Data already attached.\n" );
    return nothing;
  }

  // Get the top level object
  const CRhinoBrepObject* obj = CRhinoBrepObject::Cast( objref.Object() );
  if( !obj )
    return failure;

  // Duplicate the top level object.
  CRhinoBrepObject* dupe_obj = CRhinoBrepObject::Cast( obj->DuplicateObject() );
  if( !dupe_obj )
    return failure;

  // Get the brep
  ON_Brep* dupe_brep = dupe_obj->Brep();
  if( !dupe_brep )
    return failure;

  // Get the surface
  ON_Surface* dupe_srf = dupe_brep->m_S[face->SurfaceIndexOf()];
  if( !dupe_srf )
    return failure;

  // New up the user data
  ud = new CTestUserData();
  if( !ud )
  {
    delete dupe_obj;
    return failure;
  }

  // TODO: Initialize user data object here

  // Attach user data to surface, not face. Note, ud
  // is now owned by dupe_srf, not by us. So, we are
  // not responsible for deleting it;
  if( !dupe_srf->AttachUserData(ud) )
  {
    delete ud;
    delete dupe_obj;
    return failure;
  }

  // Replace object. Note, we cannot reuse objref for it references
  // the picked face, not the top level brep.
  // Note, dupe_obj is now owned by Rhino, so we are not
  // responsible for deleting it.
  if( !context.m_doc.ReplaceObject(CRhinoObjRef(obj), dupe_obj) )
  {
    delete dupe_obj;
    return failure;
  }

  // Done deal
  RhinoApp().Print( L"Data attached successfully.\n" );
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}
