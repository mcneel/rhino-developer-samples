CRhinoCommand::result CCommandTest::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt( L"Start of line" );
  gp.GetPoint();
  if( gp.CommandResult() != CRhinoCommand::success )
    return gp.CommandResult();

  ON_3dPoint pt_start = gp.Point();

  gp.SetCommandPrompt( L"End of line" );
  gp.SetBasePoint( pt_start );
  gp.DrawLineFromPoint( pt_start, TRUE );
  gp.GetPoint();
  if( gp.CommandResult() != CRhinoCommand::success )
    return gp.CommandResult();

  ON_3dPoint pt_end = gp.Point();
  ON_3dVector v = pt_end - pt_start;
  if( v.IsTiny() )
    return CRhinoCommand::nothing;

  ON_Line line( pt_start, pt_end );

  context.m_doc.AddCurveObject( line );
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}
