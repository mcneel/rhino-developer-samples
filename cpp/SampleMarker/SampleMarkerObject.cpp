#include "stdafx.h"
#include "SampleMarkerObject.h"

ON_OBJECT_IMPLEMENT(CSampleMarkerObject, CRhinoPointObject, "291AA3E4-21C4-4E2B-865D-F3BC12BCE4EB")

CSampleMarkerObject::CSampleMarkerObject()
  : CRhinoPointObject()
{
  CSampleMarkerUserData::AddMarkerUserData(this);
}

CSampleMarkerObject::CSampleMarkerObject(const ON_3dmObjectAttributes& att)
  : CRhinoPointObject(att)
{
  CSampleMarkerUserData::AddMarkerUserData(this);
}

CSampleMarkerObject::~CSampleMarkerObject()
{
}

CSampleMarkerObject::CSampleMarkerObject(const CSampleMarkerObject& src)
  : CRhinoPointObject(src)
{
}

CSampleMarkerObject::CSampleMarkerObject(const CRhinoPointObject& src)
  : CRhinoPointObject(src)
{
  CSampleMarkerUserData::AddMarkerUserData(this);
}

CSampleMarkerObject& CSampleMarkerObject::operator=(const CSampleMarkerObject& src)
{
  if (&src != this)
    CRhinoPointObject::operator=(src);
  return *this;
}

CSampleMarkerObject& CSampleMarkerObject::operator=(const CRhinoPointObject& src)
{
  if (&src != this)
    CRhinoPointObject::operator=(src);
  CSampleMarkerUserData::AddMarkerUserData(this);
  return *this;
}

CSampleMarkerObject& CSampleMarkerObject::operator=(const ON_3dPoint& pt)
{
  CRhinoPointObject::operator=(pt);
  CSampleMarkerUserData::AddMarkerUserData(this);
  return *this;
}

void CSampleMarkerObject::Draw(CRhinoDisplayPipeline& dp) const
{
  const CRhinoViewport* vp = dp.GetRhinoVP();
  if (nullptr != vp)
  {
    const int size_in_pixels = 5;
    double pixels_per_unit = 0.0;
    if (vp->View().m_vp.GetWorldToScreenScale(vp->View().TargetPoint(), &pixels_per_unit))
    {
      double units_per_pixel = 1.0 / pixels_per_unit;
      const ON_Plane& plane = vp->ConstructionPlane().m_plane;
      ON_3dVector vx = units_per_pixel * size_in_pixels * plane.xaxis;
      ON_3dVector vy = units_per_pixel * size_in_pixels * plane.yaxis;
      dp.DrawLine(m_point.point - vx, m_point.point + vx);
      dp.DrawLine(m_point.point - vy, m_point.point + vy);
    }
  }
}

void CSampleMarkerObject::Draw(CRhinoViewport& vp) const
{
  CRhinoDisplayPipeline* dp = vp.DisplayPipeline();
  if (dp)
    dp->DrawObject(this);
}

const wchar_t* CSampleMarkerObject::ShortDescription(bool bPlural) const
{
  return bPlural ? L"sample marker objects" : L"sample marker object";
}

int CSampleMarkerObject::Pick(const CRhinoPickContext& pick_context, CRhinoObjRefArray& pick_list) const
{
  int rc = 0;
  double hit_depth = 0.0;
  double hit_dist = 0.0;
  if (pick_context.PickPoint(m_point.point, &hit_depth, &hit_dist))
  {
    CRhinoObjRef obj_ref(this, &m_point, false);
    obj_ref.SetSelectionPoint(m_point.point);
    obj_ref.SetSelectionDepth(hit_depth);
    obj_ref.SetSelectionDistance(hit_dist);
    obj_ref.SetSelectionView(pick_context.m_view);
    pick_list.Append(obj_ref);
    rc = 1;
  }
  return rc;
}
