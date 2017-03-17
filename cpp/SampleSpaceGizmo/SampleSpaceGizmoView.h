#pragma once

class CSampleSpaceGizmoView : public CRhinoView
{
public:
  bool InDynamicViewChange();
  void SetInDynamicViewChange(bool bSet);
  bool UpdateTransforms();

private:
  friend class CSampleSpaceGizmoWnd;
};