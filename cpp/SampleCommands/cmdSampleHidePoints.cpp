#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHidePoints command
//

class CRhGetPointCloudPoint : public CRhinoGetObject
{
public:
  CRhGetPointCloudPoint(const ON_PointCloud* point_cloud)
    : CRhinoGetObject()
    , m_point_cloud(point_cloud)
  {
  }

  bool CustomGeometryFilter(
    const CRhinoObject* rhino_object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX ci
  ) const override
  {
    if (m_point_cloud && rhino_object)
    {
      const ON_PointCloud* point_cloud = ON_PointCloud::Cast(rhino_object->Geometry());
      if (
        point_cloud == m_point_cloud
        && ON_COMPONENT_INDEX::pointcloud_point == ci.m_type
        && ci.m_index >= 0
        && ci.m_index < m_point_cloud->PointCount()
        )
        return true;
    }
    return false;
  }

private:
  const ON_PointCloud* m_point_cloud;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleHidePoints : public CRhinoCommand
{
public:
  CCommandSampleHidePoints() = default;
  UUID CommandUUID() override
  {
    // {8B4CCE6A-8CAD-4A12-BD72-F6A34344AD05}
    static const GUID SampleHidePointsCommand_UUID =
    { 0x8B4CCE6A, 0x8CAD, 0x4A12, { 0xBD, 0x72, 0xF6, 0xA3, 0x43, 0x44, 0xAD, 0x05 } };
    return SampleHidePointsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleHidePoints"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleHidePoints object
static class CCommandSampleHidePoints theSampleHidePointsCommand;

CRhinoCommand::result CCommandSampleHidePoints::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select point cloud to hide points");
  go.SetGeometryFilter(CRhinoGetObject::pointset_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& point_cloud_ref = go.Object(0);
  const ON_PointCloud* point_cloud = ON_PointCloud::Cast(point_cloud_ref.Geometry());
  if (nullptr == point_cloud)
    return CRhinoCommand::failure;

  CRhGetPointCloudPoint gp(point_cloud);
  gp.SetGeometryFilter(CRhinoGetObject::point_object);
  gp.SetCommandPrompt(L"Select points to hide");
  gp.EnableSubObjectSelect(true);
  gp.EnableHighlight(true);
  gp.GetObjects(1, 0);
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_SimpleArray<int> indices(gp.ObjectCount());
  for (int i = 0; i < gp.ObjectCount(); i++)
  {
    const CRhinoObjRef& cloud_ref = gp.Object(i);
    const ON_PointCloud* cloud = cloud_ref.Object() ? ON_PointCloud::Cast(cloud_ref.Object()->Geometry()) : nullptr;
    if (cloud != point_cloud)
      continue;

    const ON_COMPONENT_INDEX ci = cloud_ref.GeometryComponentIndex();
    if (ON_COMPONENT_INDEX::pointcloud_point != ci.m_type || ci.m_index < 0 || ci.m_index >= cloud->PointCount())
      continue;

    indices.Append(ci.m_index);
  }

  const int index_count = indices.Count();
  if (0 == index_count)
    return CRhinoCommand::nothing;

  ON_PointCloud new_cloud(*point_cloud);
  for (int i = 0; i < index_count; i++)
    new_cloud.SetHiddenPointFlag(indices[i], true);

  CRhinoPointCloudObject* new_cloud_obj = new CRhinoPointCloudObject();
  new_cloud_obj->SetPointCloud(new_cloud);
  doc->ReplaceObject(point_cloud_ref, new_cloud_obj);

  doc->Redraw();

  return CRhinoCommand::success;
}

//
// END SampleHidePoints command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
