CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  // Get the active view
  CRhinoView* active_view = ::RhinoApp().ActiveView();
  if( !active_view )
    return CRhinoCommand::failure;

  // Get the name of the active view
  ON_wString active_view_name = active_view->Viewport().Name();

  // Get a list of all views
  ON_SimpleArray<CRhinoView*> view_list;
  context.m_doc.GetViewList( view_list );

  // Build a list of view names
  ON_ClassArray<ON_wString> view_names;
  int i;

  for( i = 0; i < view_list.Count(); i++ )
  {
    CRhinoView* view = view_list[i];
    if( view && view != active_view )
      view_names.Append( view->Viewport().Name() );
  }

  // Prompt the user for the view to set active
  CRhinoGetString gs;
  gs.SetCommandPrompt( L"Name of view to set active" );
  gs.AcceptNothing();
  gs.SetDefaultString( active_view_name );
  // Add view names as pickable command options
  for( i = 0; i < view_names.Count(); i++ )
    gs.AddCommandOption( RHCMDOPTNAME(view_names[i]) );

  // Do the getstring
  CRhinoGet::result res = gs.GetString();

  // User pressed ESC
  if( res == CRhinoGet::cancel )
    return CRhinoCommand::cancel;

  // User pressed ENTER
  if( res == CRhinoGet::nothing )
    return CRhinoCommand::nothing;

  ON_wString new_view_name;

  // New typed a string
  if( res == CRhinoGet::string )
    new_view_name = gs.String();

  // User clicked or typed an option
  if( res == CRhinoGet::option )
  {
    const CRhinoCommandOption* option = gs.Option();
    if( !option )
      return CRhinoCommand::failure;

    int option_index = option->m_option_index;
    if( option_index < 1 && option_index > view_names.Count() )
      return CRhinoCommand::failure;

    new_view_name = view_names[option_index-1];
  }

  // Compare view names
  if( new_view_name.CompareNoCase(active_view_name) == 0 )
    return CRhinoCommand::nothing;

  // Find the specified view by name
  active_view = 0;
  for( i = 0; i < view_list.Count(); i++ )
  {
    CRhinoView* view = view_list[i];
    if( view && view != active_view )
    {
      if( new_view_name.CompareNoCase(view->Viewport().Name()) == 0 )
      {
        active_view = view;
        break;
      }
    }
  }

  // Set the new active view
  if( active_view )
    ::RhinoApp().SetActiveView( active_view );
  else
    ::RhinoApp().Print( L"The \"%s\" view was not found.\n", new_view_name );
  return CRhinoCommand::success;
}
