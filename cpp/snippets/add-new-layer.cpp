CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  // Get reference to the document's layer table
  CRhinoLayerTable& layer_table = context.m_doc.m_layer_table;
  // Cook up an unused layer name
  ON_wString unused_name;
  layer_table.GetUnusedLayerName( unused_name );

  // Prompt the user to enter a layer name
  CRhinoGetString gs;
  gs.SetCommandPrompt( L"Name of layer to add" );
  gs.SetDefaultString( unused_name );
  gs.AcceptNothing( TRUE );
  gs.GetString();
  if( gs.CommandResult() != CRhinoCommand::success )
    return gs.CommandResult();

  // Was a layer named entered?
  ON_wString layer_name = gs.String();
  layer_name.TrimLeftAndRight();
  if( layer_name.IsEmpty() )
  {
    RhinoApp().Print( L"Layer name cannot be blank.\n" );
    return CRhinoCommand::cancel;
  }

  // Is the layer name valid?
  if( !RhinoIsValidName(layer_name) )
  {
    RhinoApp().Print( L"\"%s\" is not a valid layer name.\n", layer_name );
    return CRhinoCommand::cancel;
  }

  // Does a layer with the same name already exist?
  int layer_index = layer_table.FindLayer( layer_name );
  if( layer_index >= 0 )
  {
    RhinoApp().Print( L"A layer with the name \"%s\" already exists.\n", layer_name );
    return CRhinoCommand::cancel;
  }

  // Create a new layer
  ON_Layer layer;
  layer.SetLayerName( layer_name );
  // Add the layer to the layer table
  layer_index = layer_table.AddLayer( layer );
  if( layer_index < 0 )
  {
    RhinoApp().Print( L"Unable to add \"%s\" layer.\n", layer_name );
    return CRhinoCommand::failure;
  }
  return CRhinoCommand::success;
}
