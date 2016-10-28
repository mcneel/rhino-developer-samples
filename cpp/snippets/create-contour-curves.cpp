CRhinoCommand::result CCommandTest::RunCommand(
      const CRhinoCommandContext& context )
{
  unsigned int filter = CRhinoGetObject::surface_object |
                        CRhinoGetObject::polysrf_object |
                        CRhinoGetObject::mesh_object;
  // Prompt for object to contour
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select object to contour" );
  go.SetGeometryFilter( filter );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  // Validate selection
  const CRhinoObjRef& objref = go.Object(0);
  const ON_Geometry* geom = objref.Geometry();
  if( !geom )
    return failure;

  // Prompt for base point
  CRhinoGetPoint gp;
  gp.SetCommandPrompt( L"Contour plane base point" );
  gp.GetPoint();
  if( gp.CommandResult() != success )
    return gp.CommandResult();

  ON_3dPoint basept = gp.Point();

  // Prompt for end point
  gp.DrawLineFromPoint( basept, true );
  gp.SetCommandPrompt( L"Direction perpendicular to contour planes" );
  gp.GetPoint();
  if( gp.CommandResult() != success )
    return gp.CommandResult();

  // Verify base and end points are not the same
  ON_3dPoint endpt = gp.Point();
  if( basept.DistanceTo(endpt) < ON_ZERO_TOLERANCE )
    return nothing;

  // Prompt for distance between contours
  CRhinoGetDistance gd;
  gd.SetCommandPrompt( L"Distance between contours" );
  gd.SetDefaultNumber( 1.0 );
  gd.GetDistance();
  if( gd.CommandResult() != success )
    return gd.CommandResult();

  // Make sure interval is reasonable
  double interval = fabs( gd.Distance() );
  if( interval < ON_ZERO_TOLERANCE )
    return nothing;

  // Create contour input
  CRhinoContourInput contour;
  contour.m_geom.Append( geom );
  contour.m_basept = basept;
  contour.m_endpt = endpt;
  contour.m_interval = interval;
  contour.m_limit_range = false;

  // Create arrays for contour output
  ON_SimpleArray<ON_Polyline*> pline_array;
  ON_SimpleArray<ON_Curve*> crv_array;

  // Make the contours. Note, this function allocates memory
  // for new curves and polylines. Thus, we are responsible
  // for clean up the memory.
  bool rc = MakeRhinoContours( contour, pline_array, crv_array );
  if( !rc )
    return failure;
  context.m_doc.UnselectAll();

  // Process crves created by contouring surfaces and polysurfaces
  for( int i = 0; i < crv_array.Count(); i++)
  {
    ON_Curve* crv = crv_array[i];
    if( crv )
    {
      CRhinoCurveObject* crvobj = context.m_doc.AddCurveObject( *crv );
      if( crvobj )
        crvobj->Select();

      // CRhinoDoc::AddCurveObject() makes a copy of the input.
      // Thus, we must delete crv, otherwise we will leak memory.
      delete crv;
      crv_array[i] = 0;
    }
  }

  // Process polylines created by contouring meshes
  for( int i = 0; i < pline_array.Count(); i++)
  {
    ON_Polyline* pline = pline_array[i];
    if( pline )
    {
      CRhinoCurveObject* crvobj = context.m_doc.AddCurveObject( *pline );
      if( crvobj )
        crvobj->Select();

      // CRhinoDoc::AddCurveObject() makes a copy of the input.
      // Thus, we must delete pline, otherwise we will leak memory.
      delete pline;
      pline_array[i] = 0;
    }
  }
  context.m_doc.Redraw();
  return success;
}
