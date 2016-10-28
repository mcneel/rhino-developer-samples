CRhinoCommand::result CCommandBrenton::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select closed planar curve" );
  go.SetGeometryFilter( CRhinoGetObject::curve_object );
  go.SetGeometryAttributeFilter( CRhinoGetObject::closed_curve );
  go.EnableSubObjectSelect( false );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  const ON_Curve* crv = go.Object(0).Curve();
  if( 0 == crv | !crv->IsClosed() | !crv->IsPlanar() )
    return failure;

  CRhinoHatchPatternTable& table = context.m_doc.m_hatchpattern_table;

  CRhinoGetString gs;
  gs.SetCommandPrompt( L"Hatch pattern" );
  gs.SetDefaultString( table.CurrentHatchPattern().Name() );
  gs.GetString();
  if( gs.CommandResult() != success )
    return gs.CommandResult();

  ON_wString name = gs.String();
  name.TrimLeftAndRight();
  if( name.IsEmpty() )
    return nothing;

  int index = table.FindHatchPattern( name );
  if( index < 0 )
  {
    RhinoApp().Print( L"Hatch pattern does not exist.\n" );
    return nothing;
  }

  CArgsRhinoHatch args;
  args.m_loops.Append( crv );
  args.SetPatternIndex( index );
  args.SetPatternScale( 1.0 ); // default
  args.SetPatternRotation( 0.0 ); // default

  ON_SimpleArray<ON_Hatch*> hatches;
  bool rc = RhinoCreateHatches( args, hatches );
  if( rc && hatches.Count() )
  {
    int i, num_added = 0;
    for( i = 0; i < hatches.Count(); i++ )
    {
      ON_Hatch* hatch = hatches[0];
      if( hatch )
      {
        CRhinoHatch* hatch_obj = new CRhinoHatch();
        if( hatch_obj )
        {
          hatch_obj->SetHatch( hatch );
          if( context.m_doc.AddObject(hatch_obj) )
            num_added++;
          else
            delete hatch_obj;
        }
        else
          delete hatch;
      }
    }

    if( num_added )
      context.m_doc.Redraw();
  }

  return rc ? success : failure;
}
