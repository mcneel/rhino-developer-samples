CArgsRhinoGetPlane args;
args.SetFirstPointPromptCorners( L"First corner of plane" );
args.SetSecondPointPromptCorners( L"Other corner or length" );
args.SetFirstPointPrompt3Point( L"Start of edge" );
args.SetSecondPointPrompt3Point( L"End of edge" );
args.SetThirdPointPrompt3Point( L"Width. Press Enter to use length" );
args.SetFirstPointPromptVertical( L"Start of edge" );
args.SetSecondPointPromptVertical( L"End of edge" );
args.SetThirdPointPromptVertical( L"Height. Press Enter to use width" );
args.SetFirstPointPromptCenter( L"Center of plane" );
args.SetSecondPointPromptCenter( L"Other corner or length" );
args.SetAllow3Point();
args.SetAllowCenter();
args.SetAllowVertical();
args.SetAllowRounded( false );
args.SetAllowDeformable( false );

ON_3dPoint corners[4];
CRhinoCommand::result rc = RhinoGetRectangle( args, corners );

if( rc == CRhinoCommand::success)
{
  ON_3dPoint& p0 = corners[0];
  ON_3dPoint& p1 = corners[1];
  ON_3dPoint& p3 = corners[3];

  ON_Interval domain0, domain1;
  domain0.Set( 0.0, p0.DistanceTo(p1) );
  domain1.Set( 0.0, p0.DistanceTo(p3) );

  ON_Plane plane( p0, p1, p3 );

  ON_PlaneSurface ps( plane );
  ps.SetExtents( 0, domain0, true );
  ps.SetExtents( 1, domain1, true );
  ps.SetDomain( 0, domain0.Min(), domain0.Max() );
  ps.SetDomain( 1, domain1.Min(), domain1.Max() );

  context.m_doc.AddSurfaceObject( ps );
  context.m_doc.Redraw();
}
