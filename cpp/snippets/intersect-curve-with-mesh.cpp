CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject gm;
  gm.SetCommandPrompt( L"Select mesh to intersect" );
  gm.SetGeometryFilter( CRhinoGetObject::mesh_object );
  gm.GetObjects( 1, 1 );
  if( gm.CommandResult() != CRhinoCommand::success )
    return gm.CommandResult();

  const ON_Mesh* mesh = gm.Object(0).Mesh();
  if( 0 == mesh )
    return CRhinoCommand::failure;

  CRhinoGetObject gc;
  gc.SetCommandPrompt( L"Select curve to intersect with mesh" );
  gc.SetGeometryFilter( CRhinoGetObject::curve_object );
  gc.EnablePreSelect( false );
  gc.EnableDeselectAllBeforePostSelect( false );
  gc.GetObjects( 1, 1 );
  if( gc.CommandResult() != CRhinoCommand::success )
    return gc.CommandResult();

  const ON_Curve* curve = gc.Object(0).Curve();
  if( 0 == curve )
    return CRhinoCommand::failure;

  double tol = context.m_doc.AbsoluteTolerance();

  ON_PolylineCurve pline;
  if( RhinoConvertCurveToPolyline(*curve, 0, 0, 0, 0, 0.0, tol, 0.0, 0, pline) )
  {
    const ON_MeshTree* mesh_tree = mesh->MeshTree(true);
    if( mesh_tree )
    {
      ON_SimpleArray<ON_CMX_EVENT> cmx;
      if( mesh_tree->IntersectPolyline(pline.m_pline.Count(), pline.m_pline.Array(), cmx) )
      {
        int i;
        for( i = 0; i < cmx.Count(); i++ )
          context.m_doc.AddPointObject( cmx[i].m_M[0].m_P );
        context.m_doc.Redraw();
      }
      else
      {
        RhinoApp().Print( L"Objects do not intersect.\n" );
      }
    }
  }

  return CRhinoCommand::success;
}
