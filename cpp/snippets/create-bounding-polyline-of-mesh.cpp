CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  // Select an open mesh object
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select open mesh" );
  go.SetGeometryFilter( CRhinoGetObject::mesh_object );
  go.SetGeometryAttributeFilter( CRhinoGetObject::open_mesh );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  // Validate the selection
  const CRhinoObjRef& ref = go.Object(0);
  const ON_Mesh* mesh = ref.Mesh();
  if( 0 == mesh  )
    return failure;

  // Get the mesh's topology
  const ON_MeshTopology& mesh_top = mesh->Topology();
  ON_SimpleArray<const ON_Curve*> lines( mesh_top.m_tope.Count() );

  // Find all of the mesh edges that have only a single mesh face
  int i;
  for( i = 0; i < mesh_top.m_tope.Count(); i++ )
  {
    const ON_MeshTopologyEdge& mesh_edge = mesh_top.m_tope[i];
    if( mesh_edge.m_topf_count == 1 )
    {
      ON_3fPoint p0 = mesh_top.TopVertexPoint( mesh_edge.m_topvi[0] );
      ON_3fPoint p1 = mesh_top.TopVertexPoint( mesh_edge.m_topvi[1] );
      ON_LineCurve* line = new ON_LineCurve( ON_3dPoint(p0), ON_3dPoint(p1) );
      lines.Append( line );
    }
  }

  ON_SimpleArray<ON_Curve*> output;
  double tolerance = 2.1 * context.m_doc.AbsoluteTolerance();

  // Join all of the line segments
  if( RhinoMergeCurves(lines, output, tolerance) )
  {
    for( i = 0; i < output.Count(); i++ )
    {
      CRhinoCurveObject* crv = new CRhinoCurveObject;
      crv->SetCurve( output[i] );
      if( context.m_doc.AddObject(crv) )
        crv->Select();
      else
        delete crv;
    }
  }

  // Clean up so we don't leak memory
  for( i = 0; i < lines.Count(); i++ )
    delete lines[i];

  return success;
}
