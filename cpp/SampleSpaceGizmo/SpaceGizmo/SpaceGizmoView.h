/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoView.h

#pragma once

// Description:
//   Space Gizmo view (hack) that is used to call protected members
//   on CRhinoView.

class CSpaceGizmoView : public CRhinoView
{
public:
  bool InDynamicViewChange();
  void SetInDynamicViewChange( bool bSet );
  bool UpdateTransforms();

private:
  friend class CSpaceGizmoWnd;
};