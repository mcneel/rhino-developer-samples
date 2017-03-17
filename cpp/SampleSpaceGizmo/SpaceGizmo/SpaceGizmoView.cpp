/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoView.cpp

#include "StdAfx.h"
#include "SpaceGizmoView.h"

bool CSpaceGizmoView::InDynamicViewChange()
{
  return this->m_bInDynamicViewChange;
}

void CSpaceGizmoView::SetInDynamicViewChange( bool bSet )
{
  this->m_bInDynamicViewChange = bSet;
}

bool CSpaceGizmoView::UpdateTransforms()
{
  //this->ActiveViewport().UpdateTransforms();
  return true;
}