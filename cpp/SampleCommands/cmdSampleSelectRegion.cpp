#include "stdafx.h"

/*
Description:
  Selects point cloud points using a 2D selection region.
Parameters:
  view     - [in]  The view in which the selection region was defined.
  cloud    - [in]  The point cloud to test.
  points3d - [in]  The 2D (screen) points that define the selection region.
  indices  - [out] The indices of the points in the point cloud
                   that lie inside of the selection region.
Returns:
  The number of indices added to the output array.
*/
static int RhRegionSelectPointCloudPoints(
  CRhinoView* view,
  const ON_PointCloud& cloud,
  ON_SimpleArray<CPoint>& points2d,
  ON_SimpleArray<int>& indices
)
{
  if (0 == view)
    return 0;

  const int index_count = indices.Count();

  CRgn rgn;
  if (rgn.CreatePolygonRgn(points2d.Array(), points2d.Count(), WINDING))
  {
    ON_Xform w2s;
    view->ActiveViewport().VP().GetXform(ON::world_cs, ON::screen_cs, w2s);

    int i;
    ON_3dPoint point;

    for (i = 0; i < cloud.m_P.Count(); i++)
    {
      point = cloud.m_P[i];
      point.Transform(w2s);
      if (rgn.PtInRegion((int)point.x, (int)point.y))
        indices.Append(i);
    }
  }

  return indices.Count() - index_count;
}

/*
Description:
  Selects point cloud points using a 2D selection window.
Parameters:
  view     - [in]  The view in which the selection region was defined.
  cloud    - [in]  The point cloud to test.
  rect2d   - [in]  The 2D (screen) rectangle that define the selection window.
  indices  - [out] The indices of the points in the point cloud
                   that lie inside of the selection region.
Returns:
  The number of indices added to the output array.
*/
static int RhWindowsSelectPointCloudPoints(
  CRhinoView* view,
  const ON_PointCloud& cloud,
  CRect rect2d,
  ON_SimpleArray<int>& indices
)
{
  if (0 == view)
    return 0;

  ON_SimpleArray<CPoint> points2d;
  points2d.SetCapacity(5);
  points2d.SetCount(5);

  points2d[0] = CPoint(rect2d.left, rect2d.top);
  points2d[1] = CPoint(rect2d.left, rect2d.bottom);
  points2d[2] = CPoint(rect2d.right, rect2d.bottom);
  points2d[3] = CPoint(rect2d.right, rect2d.top);
  points2d[4] = points2d[0];

  return RhRegionSelectPointCloudPoints(view, cloud, points2d, indices);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRegionPick command
//

class CRhGetRegionPoints : public CRhinoGetPoint
{
public:
  CRhGetRegionPoints();

  CRhinoGet::result GetPoints();

  void OnMouseDown(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* pt);
  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* pt);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point);

  bool PointIsValid(const CPoint& pt);

public:
  ON_UUID m_viewport_id;
  ON_SimpleArray<CPoint> m_points;
  CPoint m_prev_point;
  CPoint m_temp_point;
  int m_point_spacing;
};

CRhGetRegionPoints::CRhGetRegionPoints()
{
  m_viewport_id = ON_nil_uuid;
  m_points.SetCapacity(2048);
  m_prev_point = CPoint(-1, -1);
  m_temp_point = CPoint(-1, -1);
  m_point_spacing = 3; // pixels
}


CRhinoGet::result CRhGetRegionPoints::GetPoints()
{
  CRhinoGet::result rc = CRhinoGet::cancel;

  m_points.Reserve(2048);
  m_points.SetCount(0);
  m_prev_point = CPoint(-1, -1);

  AcceptNothing();
  ConstrainToTargetPlane();
  EnableObjectSnapCursors(false);
  PermitObjectSnap(false);
  m_2dClamp = true;

  for (;;)
  {
    m_temp_point = CPoint(-1, -1);

    rc = GetPoint(0, true);

    if (rc == CRhinoGet::point)
    {
      CPoint pt = Point2d();
      if (PointIsValid(pt))
      {
        m_prev_point = pt;
        m_points.Append(pt);
      }
      continue;
    }

    if (rc == CRhinoGet::nothing)
    {
      if (PointIsValid(m_temp_point))
        m_points.Append(m_temp_point);

      if (m_points.Count() > 2)
        rc = CRhinoGet::point;
    }

    break;
  }

  return rc;
}

void CRhGetRegionPoints::OnMouseDown(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* pt)
{
  if (pt && m_points.Count() == 0)
  {
    CRhinoView* view = vp.ParentView();
    if (view)
    {
      m_viewport_id = vp.ViewportId();
      m_prev_point = *pt;
      m_points.Append(*pt);
    }
  }

  CRhinoGetPoint::OnMouseDown(vp, flags, point, pt);
}

void CRhGetRegionPoints::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& point, const CPoint* pt)
{
  if (pt && m_points.Count() > 0)
  {
    if (flags & MK_LBUTTON)
    {
      if (PointIsValid(*pt))
      {
        m_prev_point = *pt;
        m_temp_point = *pt;
        m_points.Append(m_prev_point);
      }
    }
    else
    {
      m_temp_point = *pt;
    }
  }

  CRhinoGetPoint::OnMouseMove(vp, flags, point, pt);
}

void CRhGetRegionPoints::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& point)
{
  CRhinoViewport* vp = dp.GetRhinoVP();
  if (nullptr != vp)
  {
    if (0 == ON_UuidCompare(m_viewport_id, vp->ViewportId()))
    {
      const int point_count = m_points.Count();
      if (point_count > 0)
      {
        ON_Color color = ON_Color::Black;
        RhinoDashStyle style = RhinoDashStyle::Solid;
        int i;

        for (i = 0; i < point_count - 1; i++)
          dp.Draw2dLine(m_points[i].x, m_points[i].y, m_points[i + 1].x, m_points[i + 1].y, color, 1, style, true);

        if (m_temp_point != CPoint(-1, -1))
        {
          dp.Draw2dLine(m_points[point_count - 1].x, m_points[point_count - 1].y, m_temp_point.x, m_temp_point.y, color, 1, style, true);
          if (point_count > 1)
            dp.Draw2dLine(m_temp_point.x, m_temp_point.y, m_points[0].x, m_points[0].y, color, 1, style, true);
        }
        else
          dp.Draw2dLine(m_points[point_count - 1].x, m_points[point_count - 1].y, m_points[0].x, m_points[0].y, color, 1, style, true);
      }
    }
  }

  CRhinoGetPoint::DynamicDraw(dp, point);
}

bool CRhGetRegionPoints::PointIsValid(const CPoint& pt)
{
  if (pt != CPoint(-1, -1))
  {
    if (abs(m_prev_point.x - pt.x) >= m_point_spacing || abs(m_prev_point.y - pt.y) >= m_point_spacing)
      return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleRegionPick : public CRhinoCommand
{
public:
  CCommandSampleRegionPick() {}
  ~CCommandSampleRegionPick() {}
  UUID CommandUUID()
  {
    // {3F0B7066-2EE3-45DF-989B-0124337F868E}
    static const GUID SampleRegionPickCommand_UUID =
    { 0x3F0B7066, 0x2EE3, 0x45DF, { 0x98, 0x9B, 0x01, 0x24, 0x33, 0x7F, 0x86, 0x8E } };
    return SampleRegionPickCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleRegionPick"; }
  const wchar_t* LocalCommandName() { return L"SampleRegionPick"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleRegionPick object
static class CCommandSampleRegionPick theSampleRegionPickCommand;

CRhinoCommand::result CCommandSampleRegionPick::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select point cloud");
  go.SetGeometryFilter(CRhinoGetObject::pointset_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoPointCloudObject* obj = CRhinoPointCloudObject::Cast(go.Object(0).Object());
  if (0 == obj)
    return CRhinoCommand::failure;

  const ON_PointCloud& cloud = obj->PointCloud();

  obj->Select(false);
  context.m_doc.Redraw();

  CRhGetRegionPoints gp;
  gp.SetCommandPrompt(L"Click and drag, or repeatedly click to lasso point cloud points. Press Enter when done");
  gp.AcceptNothing();
  gp.SetGetPointCursor(RhinoApp().m_default_cursor);
  gp.GetPoints();
  if (gp.Result() == CRhinoGet::point)
    return CRhinoCommand::cancel;

  ON_SimpleArray<int> indices;
  const int index_count = RhRegionSelectPointCloudPoints(gp.View(), cloud, gp.m_points, indices);
  if (0 == index_count)
    return CRhinoCommand::nothing;

  int i;
  for (i = 0; i < index_count; i++)
    obj->SelectSubObject(ON_COMPONENT_INDEX(ON_COMPONENT_INDEX::pointcloud_point, indices[i]), true);
  context.m_doc.Redraw();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  for (i = 0; i < index_count; i++)
    obj->SelectSubObject(ON_COMPONENT_INDEX(ON_COMPONENT_INDEX::pointcloud_point, indices[i]), false);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleRegionPick command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleWindowPick command
//

class CCommandSampleWindowPick : public CRhinoCommand
{
public:
  CCommandSampleWindowPick() {}
  ~CCommandSampleWindowPick() {}
  UUID CommandUUID()
  {
    // {9550F589-49BA-4126-B121-FFF91AF59755}
    static const GUID SampleWindowPickCommand_UUID =
    { 0x9550F589, 0x49BA, 0x4126, { 0xB1, 0x21, 0xFF, 0xF9, 0x1A, 0xF5, 0x97, 0x55 } };
    return SampleWindowPickCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleWindowPick"; }
  const wchar_t* LocalCommandName() { return L"SampleWindowPick"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleWindowPick object
static class CCommandSampleWindowPick theSampleWindowPickCommand;

CRhinoCommand::result CCommandSampleWindowPick::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select point cloud");
  go.SetGeometryFilter(CRhinoGetObject::pointset_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoPointCloudObject* obj = CRhinoPointCloudObject::Cast(go.Object(0).Object());
  if (0 == obj)
    return CRhinoCommand::failure;

  const ON_PointCloud& cloud = obj->PointCloud();

  obj->Select(false);
  context.m_doc.Redraw();

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Drag a window to select point cloud points");
  gp.ConstrainToTargetPlane();
  gp.AcceptNothing();
  gp.SetGetPointCursor(RhinoApp().m_default_cursor);
  gp.Get2dRectangle();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_SimpleArray<int> indices;
  const int index_count = RhWindowsSelectPointCloudPoints(gp.View(), cloud, gp.Rectangle2d(), indices);
  if (0 == index_count)
    return CRhinoCommand::nothing;

  int i;
  for (i = 0; i < index_count; i++)
    obj->SelectSubObject(ON_COMPONENT_INDEX(ON_COMPONENT_INDEX::pointcloud_point, indices[i]), true);
  context.m_doc.Redraw();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  for (i = 0; i < index_count; i++)
    obj->SelectSubObject(ON_COMPONENT_INDEX(ON_COMPONENT_INDEX::pointcloud_point, indices[i]), false);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleWindowPick command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRemovePoints command
//

#pragma region SampleRemovePoints command

class CCommandSampleRemovePoints : public CRhinoCommand
{
public:
  CCommandSampleRemovePoints() {}
  ~CCommandSampleRemovePoints() {}
  UUID CommandUUID()
  {
    // {DEED29F0-EDCA-4163-9068-462AD201EDFB}
    static const GUID SampleRemovePointsCommand_UUID =
    { 0xDEED29F0, 0xEDCA, 0x4163, { 0x90, 0x68, 0x46, 0x2A, 0xD2, 0x01, 0xED, 0xFB } };
    return SampleRemovePointsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleRemovePoints"; }
  const wchar_t* LocalCommandName() const { return L"SampleRemovePoints"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleRemovePoints object
static class CCommandSampleRemovePoints theSampleRemovePointsCommand;

CRhinoCommand::result CCommandSampleRemovePoints::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select point cloud");
  go.SetGeometryFilter(CRhinoGetObject::pointset_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const CRhinoPointCloudObject* obj = CRhinoPointCloudObject::Cast(obj_ref.Object());
  if (0 == obj)
    return CRhinoCommand::failure;

  const ON_PointCloud& cloud = obj->PointCloud();

  obj->Select(false);
  context.m_doc.Redraw();

  CRhGetRegionPoints gp;
  gp.SetCommandPrompt(L"Click and drag, or repeatedly click to lasso point cloud points. Press Enter when done");
  gp.AcceptNothing();
  gp.SetGetPointCursor(RhinoApp().m_default_cursor);
  gp.GetPoints();
  if (gp.Result() == CRhinoGet::point)
    return CRhinoCommand::cancel;

  ON_SimpleArray<int> indices;
  const int index_count = RhRegionSelectPointCloudPoints(gp.View(), cloud, gp.m_points, indices);
  if (0 == index_count)
    return CRhinoCommand::nothing;

  indices.QuickSort(&ON_CompareIncreasing<int>);

  const CRhinoObjectAttributes& atts = obj->Attributes();
  bool bColors = cloud.HasPointColors();
  bool bNormals = cloud.HasPointNormals();

  ON_PointCloud new_cloud;
  new_cloud.m_P.SetCapacity(index_count);
  new_cloud.m_P.SetCount(index_count);
  if (bColors)
  {
    new_cloud.m_C.SetCapacity(index_count);
    new_cloud.m_C.SetCount(index_count);
  }
  if (bNormals)
  {
    new_cloud.m_N.SetCapacity(index_count);
    new_cloud.m_N.SetCount(index_count);
  }

  ON_PointCloud dup_cloud(cloud);
  dup_cloud.DestroyHiddenPointArray();

  const int cloud_count = dup_cloud.PointCount();
  int last_point_index = indices[indices.Count() - 1];
  for (int i = cloud_count - 1; i >= 0; i--)
  {
    if (i == last_point_index)
    {
      int last_array_index = indices.Count() - 1;

      new_cloud.m_P[last_array_index] = dup_cloud.m_P[i];
      if (bColors)
        new_cloud.m_C[last_array_index] = dup_cloud.m_C[i];
      if (bNormals)
        new_cloud.m_N[last_array_index] = dup_cloud.m_N[i];

      dup_cloud.m_P.Remove(i);
      if (bColors)
        dup_cloud.m_C.Remove(i);
      if (bNormals)
        dup_cloud.m_N.Remove(i);

      indices.Remove(last_array_index);

      if (0 == indices.Count())
        break;

      last_point_index = indices[indices.Count() - 1];
    }
  }

  CRhinoPointCloudObject* new_cloud_obj = new CRhinoPointCloudObject(atts);
  new_cloud_obj->SetPointCloud(new_cloud);
  new_cloud.Destroy();
  if (context.m_doc.AddObject(new_cloud_obj))
  {
    new_cloud_obj->Select();
  }
  else
  {
    delete new_cloud_obj;
    return CRhinoCommand::failure;
  }

  dup_cloud.m_P.Shrink();
  if (bColors)
    dup_cloud.m_C.Shrink();
  if (bNormals)
    dup_cloud.m_N.Shrink();

  CRhinoPointCloudObject* dup_cloud_obj = new CRhinoPointCloudObject(atts);
  dup_cloud_obj->SetPointCloud(dup_cloud);
  if (!context.m_doc.ReplaceObject(obj_ref, dup_cloud_obj))
    delete dup_cloud_obj;

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRemovePoints command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
