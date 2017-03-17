#include "StdAfx.h"
#include "SampleSpaceGizmoView.h"

bool CSampleSpaceGizmoView::InDynamicViewChange()
{
  return this->m_bInDynamicViewChange;
}

void CSampleSpaceGizmoView::SetInDynamicViewChange(bool bSet)
{
  this->m_bInDynamicViewChange = bSet;
}

bool CSampleSpaceGizmoView::UpdateTransforms()
{
  //this->ActiveViewport().UpdateTransforms();
  return true;
}