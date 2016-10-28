CRhinoCommand::result CCommandCPlaneMode::RunCommand( const CRhinoCommandContext& context )
{
  BOOL bUPlane = RhinoApp().AppSettings().ModelAidSettings().m_uplane_mode;

  ON_wString str;
  if( bUPlane )
    str = L"Universal construction planes enabled. New value";
  else
    str = L"Standard construction planes enabled. New value";

  CRhinoGetOption go;
  go.SetCommandPrompt( str );
  int c_option = go.AddCommandOption( RHCMDOPTNAME(L"CPlane") );
  int u_option = go.AddCommandOption( RHCMDOPTNAME(L"UPlane") );
  int t_option = go.AddCommandOption( RHCMDOPTNAME(L"Toggle") );
  go.GetOption();
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const CRhinoCommandOption* option = go.Option();
  if( 0 == option )
    return CRhinoCommand::failure;

  CRhinoAppModelAidSettings settings = RhinoApp().AppSettings().ModelAidSettings();

  int option_index = option->m_option_index;
  if( c_option == option_index )
  {
    if( bUPlane )
    {
      settings.m_uplane_mode = FALSE;
      RhinoApp().AppSettings().SetModelAidSettings( settings );
    }
  }
  else if( u_option == option_index )
  {
    if( !bUPlane )
    {
      settings.m_uplane_mode = TRUE;
      RhinoApp().AppSettings().SetModelAidSettings( settings );
    }
  }
  else if( t_option == option_index )
  {
    settings.m_uplane_mode = !bUPlane;
    RhinoApp().AppSettings().SetModelAidSettings( settings );
  }

  return CRhinoCommand::success;
}
