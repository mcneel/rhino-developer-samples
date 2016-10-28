/*
Description:
  Intersects two meshes.
Parameters:
  meshA     - [in]  The first mesh.
  meshB     - [in]  The second mesh.
  OutCurves - [out] The resulting polyline curves.
                    NOTE: THE CALLER IS RESPONSIBLE FOR
                    DESTROYING THESE CURVES.
  tolerance - [in]  The intersection tolerance.
Returns:
  True if successful, false otherwise.
*/
static bool MeshMeshIntersection(
        const ON_Mesh* meshA,
        const ON_Mesh* meshB,
        ON_SimpleArray<ON_Curve*>& OutCurves,
        double tolerance = ON_ZERO_TOLERANCE
        )
{
  if( 0 == meshA | 0 == meshB )
    return false;

  ON_ClassArray<ON_MMX_Polyline> transverse;
  ON_ClassArray<ON_MMX_Polyline> overlap;
  bool rc = ON_MeshMeshIntersect( meshA, meshB, transverse, overlap, tolerance, tolerance );
  if( !rc )
    return rc;

  ON_ClassArray<ON_MMX_Polyline>& arr = transverse;
  int i, j, k;
  for( i = 0; i < 2; i++ )
  {
    if( i == 1 )
      arr = overlap;

    int ct = arr.Count();
    for( j = 0; j < ct; j++ )
    {
      ON_MMX_Polyline& mmxpline = arr[j];
      if( 0 == mmxpline.Count() )
        continue;

      ON_Polyline polyline;
      for( k = 0; k < mmxpline.Count(); k++ )
      {
        double x = mmxpline[k].m_A.m_P.x;
        double y = mmxpline[k].m_A.m_P.y;
        double z = mmxpline[k].m_A.m_P.z;
        polyline.Append( ON_3dPoint(x,y,z) );
      }

      if( false == polyline.IsValid() )
        continue;

      ON_PolylineCurve* polyline_crv = new ON_PolylineCurve( polyline );
      if( 0 == polyline_crv )
        continue;

      OutCurves.Append( polyline_crv );
    }
  }

  return ( OutCurves.Count() ) ? true : false;
}

CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select meshes to intersect" );
  go.SetGeometryFilter( CRhinoGetObject::mesh_object );
  go.GetObjects( 2, 2 );
  if( go.CommandResult() != success )
    return go.CommandResult();

  const ON_Mesh* meshA = go.Object(0).Mesh();
  const ON_Mesh* meshB = go.Object(1).Mesh();
  if( 0 == meshA | 0 == meshA )
    return failure;

  ON_SimpleArray<ON_Curve*> OutCurves;
  bool rc = MeshMeshIntersection( meshA, meshB, OutCurves );
  if( rc )
  {
    int i;
    for( i = 0; i < OutCurves.Count(); i++ )
    {
      CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
      curve_obj->SetCurve( OutCurves[i] );
      context.m_doc.AddObject( curve_obj );
    }
    context.m_doc.Redraw();
  }

  return success;
}
