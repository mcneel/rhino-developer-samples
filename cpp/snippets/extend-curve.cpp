CRhinoCommand::result CCommandTest::RunCommand(
        const CRhinoCommandContext& context )
{
  CRhinoGetObject gc;
  gc.SetCommandPrompt( L"Select line to extend" );
  gc.SetGeometryFilter( CRhinoGetObject::curve_object );
  gc.GetObjects( 1, 1 );
  if( gc.CommandResult() != CRhinoCommand::success )
    return gc.CommandResult();

  const CRhinoObjRef& objref = gc.Object(0);
  const ON_Curve* pC = ON_Curve::Cast( objref.Geometry() );
  if( !pC )
    return CRhinoCommand::failure;

  int side = 0; // start of curve
  ON_3dPoint pt;
  if( objref.SelectionPoint(pt) )
  {
    ON_3dPoint p0 = pC->PointAtStart();
    ON_3dPoint p1 = pC->PointAtEnd();
    if( (p0-pt).Length() > (p1-pt).Length() )
      side = 1; // end of curve
  }

  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select boundary surfaces" );
  go.SetGeometryFilter( CRhinoGetObject::surface_object |
                        CRhinoGetObject::polysrf_object );
  go.EnablePreSelect( false );
  go.EnableDeselectAllBeforePostSelect( false );
  go.GetObjects( 1, 0 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  int object_count = go.ObjectCount();
  ON_SimpleArray<const ON_Geometry*> geom( object_count );

  for( int i = 0; i < object_count; i++ )
  {
    const CRhinoObject* obj = go.Object(i).Object();
    if( obj )
      geom.Append( obj->Geometry() );
  }

  if( geom.Count() <= 0 )
    return CRhinoCommand::cancel;

  ON_Curve* crv = pC->DuplicateCurve();
  if( !crv )
    return CRhinoCommand::failure;

  // Do the curve extension
  bool rc = RhinoExtendCurve(crv, CRhinoExtend::Line, side, geom);

  if( rc )
  {
    // CRhinoDoc::ReplaceObject() will copy our curve
    // so, we will need to clean up when finshed.
    context.m_doc.ReplaceObject( objref, *crv );
    context.m_doc.Redraw();
  }

  // Clean up or leak...
  delete crv;
  crv = 0;
  return CRhinoCommand::success;
}
