CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoView* view = RhinoApp().ActiveView();
  if( 0 == view )
    return CRhinoCommand::failure;

  CRhinoViewport& vp = view->ActiveViewport();

  const CDisplayPipelineAttributes* pStdAttrs = CRhinoDisplayAttrsMgr::StdGhostedAttrs();
  if( pStdAttrs )
  {
    vp.SetDisplayMode( pStdAttrs->Id() );
    view->Redraw();
  }

  return CRhinoCommand::success;
}
