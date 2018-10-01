#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshPreviewImage command
//

const static UINT PREVIEW_IMAGE_CHANNELS = 
  CSupportChannels::SC_CALCBOUNDINGBOX |
  CSupportChannels::SC_PREDRAWOBJECTS  |
  CSupportChannels::SC_DRAWOBJECT      |
  CSupportChannels::SC_POSTDRAWOBJECTS |
  CSupportChannels::SC_PREOBJECTDRAW   |
  CSupportChannels::SC_POSTOBJECTDRAW;

class CSampleMeshPreviewConduit : public CRhinoDisplayConduit
{
public:
  CSampleMeshPreviewConduit(const ON_SimpleArray<const ON_Mesh*>& meshes, const ON_SimpleArray<ON_Color>& colors, ON_BoundingBox bbox);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel);
  const ON_SimpleArray<const ON_Mesh*>& m_meshes;
  const ON_SimpleArray<ON_Color>& m_colors;
  ON_BoundingBox m_bbox;
};

CSampleMeshPreviewConduit::CSampleMeshPreviewConduit(const ON_SimpleArray<const ON_Mesh*>& meshes, const ON_SimpleArray<ON_Color>& colors, ON_BoundingBox bbox)
  : CRhinoDisplayConduit(PREVIEW_IMAGE_CHANNELS)
  , m_meshes(meshes)
  , m_colors(colors)
  , m_bbox(bbox)
{
  EnableConstantBinding(false);
}

bool CSampleMeshPreviewConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  switch (nActiveChannel)
  {
  case CSupportChannels::SC_CALCBOUNDINGBOX:
  {
    if (m_bbox.IsValid())
      m_pChannelAttrs->m_BoundingBox.Union(m_bbox);
  }
  break;

  case CSupportChannels::SC_POSTDRAWOBJECTS:
  {
    for (int i = 0; i < m_meshes.Count(); i++)
    {
      if (nullptr != m_meshes[i])
      {
        CDisplayPipelineMaterial material;
        dp.SetupDisplayMaterial(material, m_colors[i]);
        dp.DrawShadedMesh(*m_meshes[i], &material);
        dp.DrawWireframeMesh(*m_meshes[i], m_colors[i]);
      }
    }
  }
  break;

  case CSupportChannels::SC_DRAWOBJECT:
  {
    m_pChannelAttrs->m_bDrawObject = false;
  }
  break;
  }

  // Disallow any and all other conduits from executing.
  bTerminateChannel = true;

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

bool SampleMeshPreviewImage(
  const ON_SimpleArray<const ON_Mesh*>& meshes,
  const ON_SimpleArray<ON_Color>& colors,
  CSize size,
  CRhinoDib& dib
)
{
  // Validate input
  const int mesh_count = meshes.Count();
  const int color_count = colors.Count();
  if (0 == mesh_count || 0 == color_count || mesh_count != color_count)
    return false;

  if (size.cx < 1 || size.cy < 1)
    return false;

  if (!dib.CreateDib(size.cx, size.cy, 32, true))
    return false;

  HDC hDC = dib;
  if (0 == hDC)
    return false;

  // Create a dummy view
  CRhinoViewport vp;
  vp.SetToPerspectiveView();
  vp.SetScreenSize(size.cx, size.cy);

  // Set shaded display
  const CDisplayPipelineAttributes* dma = CRhinoDisplayAttrsMgr::StdShadedAttrs();
  vp.SetDisplayMode(dma->Id());

  // Calculate bounding box
  ON_BoundingBox world_bbox;
  bool bGrowBox = false;
  for (int i = 0; i < mesh_count; i++)
  {
    if (nullptr != meshes[i])
    {
      meshes[i]->GetTightBoundingBox(world_bbox, bGrowBox);
      if (!bGrowBox)
        bGrowBox = world_bbox.IsValid();
    }
  }

  // Zoom extents bounding box
  const ON_Viewport& current_vp = vp.VP();
  ON_Xform world_to_camera;
  current_vp.GetXform(ON::world_cs, ON::camera_cs, world_to_camera);

  ON_BoundingBox camcoord_bbox(world_bbox);
  camcoord_bbox.Transform(world_to_camera);

  // Nudge camera in a bit
  if (camcoord_bbox.IsValid())
  {
    double border = 0.1;
    double dx = (camcoord_bbox.m_max.x - camcoord_bbox.m_min.x) * border;
    double dy = (camcoord_bbox.m_max.y - camcoord_bbox.m_min.y) * border;
    camcoord_bbox.m_min.x += dx;
    camcoord_bbox.m_min.y += dy;
    camcoord_bbox.m_max.x -= dx;
    camcoord_bbox.m_max.y -= dy;
  }

  ON_Viewport zoomed_vp;
  if (RhinoDollyExtents(current_vp, camcoord_bbox, zoomed_vp))
    vp.SetVP(zoomed_vp, true);

  // Setup the pipeline
  CRhinoDisplayPipeline_OGL dp(hDC, vp);
  CDisplayPipelineAttributes da(*dma);

  da.m_bUseDocumentGrid = false;
  da.m_bDrawGrid = false;
  da.m_bDrawAxes = false;
  da.m_bDrawWorldAxes = false;

  // Create the conduit
  CSampleMeshPreviewConduit conduit(meshes, colors, world_bbox);
  conduit.Bind(vp);
  conduit.Enable(CRhinoDoc::NullRuntimeSerialNumber);

  // Render
  da.m_contextDrawToDC = CDisplayPipelineAttributes::ContextsForDrawToDC::kUIPreview;
  bool rc = dp.DrawToDib(dib, size.cx, size.cy, da);
  conduit.Disable();

  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMeshPreviewImage : public CRhinoCommand
{
public:
  CCommandSampleMeshPreviewImage() = default;
  UUID CommandUUID() override
  {
    // {55364E82-D795-44F0-A086-ECC4B26F5D66}
    static const GUID SampleMeshPreviewImageCommand_UUID =
    { 0x55364E82, 0xD795, 0x44F0, { 0xA0, 0x86, 0xEC, 0xC4, 0xB2, 0x6F, 0x5D, 0x66 } };
    return SampleMeshPreviewImageCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMeshPreviewImage"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMeshPreviewImage object
static class CCommandSampleMeshPreviewImage theSampleMeshPreviewImageCommand;

CRhinoCommand::result CCommandSampleMeshPreviewImage::RunCommand(const CRhinoCommandContext& context)
{
  ON_Sphere sphere;
  CArgsRhinoGetSphere args;
  CRhinoCommand::result rc = RhinoGetSphere(args, sphere);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_Mesh* mesh = RhinoMeshSphere(sphere, 10, 10);
  if (nullptr == mesh)
    return failure;

  ON_SimpleArray<const ON_Mesh*> meshes;
  meshes.Append(mesh);

  ON_SimpleArray<ON_Color> colors;
  colors.Append(ON_Color(RGB(100, 149, 237)));

  CRhinoDib dib;
  bool success = SampleMeshPreviewImage(meshes, colors, CSize(1024, 768), dib);
  if (success)
  {
    const wchar_t* file_name = L"\\sample_mesh_preview_image.png";
    ON_wString path;
    CRhinoFileUtilities::GetWindowsSpecialFolderPath(CSIDL_DESKTOPDIRECTORY, path);
    path += file_name;
    dib.WriteToFile(path);
  }

  delete mesh; // Don't leak...

  return CRhinoCommand::success;
}

//
// END SampleMeshPreviewImage command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
