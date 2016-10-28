CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select arc to convert" );
  go.SetGeometryFilter( CRhinoGetObject::curve_object );
  go.SetGeometryAttributeFilter( CRhinoGetObject::open_curve );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const CRhinoObject* obj = obj_ref.Object();
  if( !obj )
    return failure;

  const ON_ArcCurve* arc_crv = ON_ArcCurve::Cast( obj_ref.Geometry() );
  if( !arc_crv )
  {
    RhinoApp().Print( L"Curve is not an arc.\n" );
    return nothing;
  }

  ON_NurbsCurve nurbs_crv;
  if( arc_crv->GetNurbForm(nurbs_crv) && nurbs_crv.IsValid() )
  {
    ON_3dmObjectAttributes attribs = obj->Attributes();
    context.m_doc.AddCurveObject( nurbs_crv, &attribs );
    context.m_doc.DeleteObject( obj_ref );
    context.m_doc.Redraw();
    return success;
  }

  return failure;
}
