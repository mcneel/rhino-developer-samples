#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExplodePointCloud command
//

#pragma region SampleExplodePointCloud command

class CCommandSampleExplodePointCloud : public CRhinoCommand
{
public:
  CCommandSampleExplodePointCloud() {}
  ~CCommandSampleExplodePointCloud() {}
  UUID CommandUUID()
  {
    // {C11373EE-8093-4EDB-89E3-B4418A512FD7}
    static const GUID SampleExplodePointCloudCommand_UUID =
    { 0xC11373EE, 0x8093, 0x4EDB, { 0x89, 0xE3, 0xB4, 0x41, 0x8A, 0x51, 0x2F, 0xD7 } };
    return SampleExplodePointCloudCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExplodePointCloud"; }
  const wchar_t* LocalCommandName() const { return L"SampleExplodePointCloud"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExplodePointCloud object
static class CCommandSampleExplodePointCloud theSampleExplodePointCloudCommand;

CRhinoCommand::result CCommandSampleExplodePointCloud::RunCommand(const CRhinoCommandContext& context)
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
  bool bColors = cloud.HasPointColors();

  CRhinoObjectAttributes atts;
  context.m_doc.GetDefaultObjectAttributes(atts);
  atts.SetColorSource(ON::color_from_object);

  for (int i = 0; i < cloud.PointCount(); i++)
  {
    if (bColors)
      atts.m_color = cloud.m_C[i];

    CRhinoPointObject* point_obj = context.m_doc.AddPointObject(cloud.m_P[i], bColors ? &atts : 0);
    if (go.ObjectsWerePreSelected() && point_obj)
      point_obj->Select();
  }

  context.m_doc.DeleteObject(obj_ref);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleExplodePointCloud command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
