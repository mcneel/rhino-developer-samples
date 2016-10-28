CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  // Pick a grip object
  CRhinoGetObject go;
  go.SetCommandPrompt( RHSTR(L"Select control point for weight editing") );
  go.SetGeometryFilter( CRhinoGetObject::grip_object );
  go.EnableReferenceObjectSelect( false );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  // Add the object to an xform object list
  CRhinoXformObjectList objlist;
  if( objlist.AddObjects(go, true) < 1 )
    return CRhinoCommand::failure;

  // Get the grip's weight
  double weight = objlist.m_grips[0]->Weight();

  // Prompt for a new value
  CRhinoGetNumber gn;
  gn.SetCommandPrompt( RHSTR(L"New control point weight") );
  gn.SetDefaultNumber( weight );
  gn.AcceptNothing();

  // Validate the results
  CRhinoGet::result res = gn.GetNumber();
  if( res == CRhinoGet::number )
    weight = gn.Number();
  else if( res == CRhinoGet::nothing )
    return CRhinoCommand::nothing;
  else
    return CRhinoCommand::cancel;

  // Do nothing if weights are the same
  if( weight == objlist.m_grips[0]->Weight() )
    return CRhinoCommand::nothing;

  // Modify the grip's weight
  objlist.m_grips[0]->SetWeight( weight );

  // Get the grip object's owning object
  CRhinoObject* old_object = objlist.m_grip_owners[0];
  if( old_object && old_object->m_grips )
  {
    // Create a new object based on the updated grip information
    CRhinoObject* new_object = old_object->m_grips->NewObject();
    if( new_object )
    {
      // Replace the old object with the new object
      context.m_doc.ReplaceObject( CRhinoObjRef(old_object), new_object );
      context.m_doc.Redraw();
    }
  }
  return CRhinoCommand::success;
}
