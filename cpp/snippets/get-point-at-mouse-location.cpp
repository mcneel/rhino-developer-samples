static bool GetPointAtCursorPos( ON_3dPoint& point )
{
  bool rc = false;
  CRhinoView* view = RhinoApp().ActiveView();
  if (view)
  {
    POINT pt;
    if (::GetCursorPos(&pt) )
    {
      view->ScreenToClient( &pt );
      ON_Xform xform;
      if( view->ActiveViewport().VP().GetXform(ON::screen_cs, ON::world_cs, xform) )
      {
        point = ON_3dPoint( pt.x, pt.y, 0.0 );
        point.Transform( xform );
        rc = true;
      }
    }
  }
  return rc;
}
