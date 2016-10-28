CRhinoCommand::result CCommandTest::RunCommand(const CRhinoCommandContext& context)
{
  ON_3dPoint bottom_pt( 0.0, 0.0, 0.0 );
  double bottom_radius = 2;
  ON_Circle bottom_circle( bottom_pt, bottom_radius );

  ON_3dPoint top_pt( 0.0, 0.0, 10.0 );
  double top_radius = 6;
  ON_Circle top_circle( top_pt, top_radius );

  ON_RevSurface* revsrf = new ON_RevSurface;
  ON_LineCurve* pShapeCurve = new ON_LineCurve;
  revsrf->m_curve = pShapeCurve;
  pShapeCurve->m_dim = 3;
  pShapeCurve->m_line.from = bottom_circle.PointAt(0);
  pShapeCurve->m_line.to = top_circle.PointAt(0);
  pShapeCurve->m_t.Set(0, pShapeCurve->m_line.from.DistanceTo(pShapeCurve->m_line.to));
  revsrf->m_axis.from = bottom_circle.Center();
  revsrf->m_axis.to = top_circle.Center();
  revsrf->m_angle[0] = revsrf->m_t[0] = 0.0;
  revsrf->m_angle[1] = revsrf->m_t[1] = 2.0*ON_PI;

  ON_Brep* tcone_brep = ON_BrepRevSurface(revsrf, TRUE, TRUE );
  if( tcone_brep )
  {
    CRhinoBrepObject* tcone_object = new CRhinoBrepObject();
    tcone_object->SetBrep( tcone_brep );
    if( context.m_doc.AddObject(tcone_object) )
      context.m_doc.Redraw();
    else
      delete tcone_object;
  }

  return CRhinoCommand::success;
}
