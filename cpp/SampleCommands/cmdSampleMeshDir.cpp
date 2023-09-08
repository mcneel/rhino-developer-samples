#include "stdafx.h"
#include "SampleDrawCallback.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshDir command
//

#include <memory>
#include <vector>

class CMeshDir
{
public:
  CMeshDir();
  CMeshDir(std::shared_ptr<const ON_Mesh>);
  void SetMesh(std::shared_ptr<const ON_Mesh>);

  ON_SimpleArray<ON_3dPoint> m_face_center;
  ON_SimpleArray<ON_3dVector> m_face_normal;
  std::shared_ptr<const ON_Mesh> m_mesh;
};

CMeshDir::CMeshDir()
{
  m_mesh = NULL;
}

CMeshDir::CMeshDir(std::shared_ptr<const ON_Mesh> mesh)
{
  SetMesh(mesh);
}

void CMeshDir::SetMesh(std::shared_ptr<const ON_Mesh> mesh)
{
  m_mesh = mesh;
  m_face_center.Destroy();
  m_face_normal.Destroy();
  if (m_mesh)
  {
    if (!m_mesh->HasFaceNormals())
    {
      std::const_pointer_cast<ON_Mesh>(m_mesh)->ComputeFaceNormals();
    }
    if (m_mesh->HasFaceNormals())
    {
      int fi, fcnt = m_mesh->m_F.Count();
      m_face_center.Reserve(fcnt);
      m_face_normal.Reserve(fcnt);
      ON_3dPoint p;
      ON_3dVector n;
      for (fi = 0; fi < fcnt; fi++)
      {
        const int* fvi = m_mesh->m_F[fi].vi;
        if (m_mesh->m_F[fi].IsQuad())
        {
          p = 0.25*(m_mesh->m_V[fvi[0]] + m_mesh->m_V[fvi[1]] + m_mesh->m_V[fvi[2]] + m_mesh->m_V[fvi[3]]);
        }
        else if (m_mesh->m_F[fi].IsTriangle())
        {
          p = (1.0f / 3.0f)*(m_mesh->m_V[fvi[0]] + m_mesh->m_V[fvi[1]] + m_mesh->m_V[fvi[2]]);
        }
        else
          continue;
        n = m_mesh->m_FN[fi];
        m_face_center.Append(p);
        m_face_normal.Append(n);
      }
    }
  }
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CMeshDirDrawCallback : public CSampleDrawCallback
{
public:
  CMeshDirDrawCallback();
  void DrawForeground(CRhinoViewport& vp, CRhinoDoc& doc) override;

  std::vector<CMeshDir> m_mesh_list;
  ON_Color m_face_normal_color;
  ON_Color m_vertex_normal_color;
  bool m_draw_face_normals;
  bool m_draw_vertex_normals;
};

CMeshDirDrawCallback::CMeshDirDrawCallback()
{
  m_face_normal_color = RGB(0, 0, 0);
  m_vertex_normal_color = RGB(255, 255, 255);
  m_draw_face_normals = true;
  m_draw_vertex_normals = true;
}

void CMeshDirDrawCallback::DrawForeground(CRhinoViewport& vp, CRhinoDoc&)
{
  CRhinoDisplayPipeline* dp = vp.DisplayPipeline();
  if (nullptr == dp)
    return;

  const auto mcnt = m_mesh_list.size();

  for (int mi = 0; mi < mcnt; mi++)
  {
    ON_Color saved_color = dp->ObjectColor();
    CMeshDir& md = m_mesh_list[mi];

    if (m_draw_face_normals)
    {
      const auto fcnt = md.m_face_center.Count();

      dp->SetObjectColor(m_face_normal_color);
      for (int fi = 0; fi < fcnt; fi++)
      {
        dp->DrawDirectionArrow(md.m_face_center[fi], md.m_face_normal[fi]);
      }
    }

    if (m_draw_vertex_normals)
    {
      if (md.m_mesh->HasVertexNormals())
      {
        dp->SetObjectColor(m_vertex_normal_color);
        
        const auto vcnt = md.m_mesh->m_V.Count();

        for (int vi = 0; vi < vcnt; vi++)
        {
          dp->DrawDirectionArrow(ON_3dPoint(md.m_mesh->m_V[vi]), ON_3dVector(md.m_mesh->m_N[vi]));
        }
      }
    }

    dp->SetObjectColor(saved_color);
  }
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMeshDir : public CRhinoCommand
{
public:
  CCommandSampleMeshDir() = default;
  ~CCommandSampleMeshDir() = default;
  UUID CommandUUID() override
  {
    // {E35209D2-AC0D-43D1-A79C-2B432F04F9BA}
    static const GUID SampleMeshDirCommand_UUID =
    { 0xE35209D2, 0xAC0D, 0x43D1, { 0xA7, 0x9C, 0x2B, 0x43, 0x2F, 0x04, 0xF9, 0xBA } };
    return SampleMeshDirCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMeshDir"; }
 CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleMeshDir object
static class CCommandSampleMeshDir theSampleMeshDirCommand;



CRhinoCommand::result CCommandSampleMeshDir::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetGeometryFilter(CRhinoObject::mesh_object | CRhinoObject::surface_object | CRhinoObject::polysrf_object | CRhinoObject::instance_reference);
  go.SetCommandPrompt(L"Select mesh, surface, or polysurface objects");
  go.GetObjects(1, 0);
  if (go.Result() != CRhinoGet::object)
    return CRhinoCommand::cancel;

  //For view dependent custom render primitives.
  const ON_Viewport vp = doc->ActiveView() ? doc->ActiveView()->ActiveViewport().VP() : ON_Viewport();

  CMeshDirDrawCallback callback;

  int count = 0;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObject* pObject = go.Object(i).Object();
    if (pObject)
    {
      auto render_meshes = pObject->RenderMeshes(ON::render_mesh, true, &vp);

      if (render_meshes)
      {
        for (const auto& instance : *render_meshes)
        {
          if (instance)
          {
            callback.m_mesh_list.push_back(instance->Geometry().Mesh());
            count++;
          }
        }
      }

    }
  }

  if (0 == count)
    return CRhinoCommand::failure;

  // Get persistent settings
  const wchar_t* psz_draw_face_normals = L"DrawFaceNormals";
  bool draw_face_normals = true;
  Settings().GetBool(psz_draw_face_normals, draw_face_normals, draw_face_normals);

  const wchar_t* psz_draw_vertex_normals = L"DrawVertexNormals";
  bool draw_vertex_normals = true;
  Settings().GetBool(psz_draw_vertex_normals, draw_vertex_normals, draw_vertex_normals);

  CRhinoGetOption gs;
  gs.SetCommandPrompt(L"Press Enter when done");
  gs.AcceptNothing();
  gs.AddCommandOptionToggle(RHCMDOPTNAME(L"FaceNormals"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), draw_face_normals, &draw_face_normals);
  gs.AddCommandOptionToggle(RHCMDOPTNAME(L"VertexNormals"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), draw_vertex_normals, &draw_vertex_normals);

  callback.Enable(doc->RuntimeSerialNumber());

  CRhinoGet::result res = CRhinoGet::nothing;
  for (;; )
  {
    callback.m_draw_face_normals = draw_face_normals;
    callback.m_draw_vertex_normals = draw_vertex_normals;

    doc->Redraw(CRhinoView::regenerate_display_hint);;

    res = gs.GetOption();
    if (res == CRhinoGet::option)
      continue;

    break;
  }

  callback.Disable();
  doc->Redraw(CRhinoView::regenerate_display_hint);

  // Save persistent settings
  Settings().SetBool(psz_draw_face_normals, draw_face_normals);
  Settings().SetBool(psz_draw_vertex_normals, draw_vertex_normals);

  return CRhinoCommand::success;
}

//
// END SampleMeshDir command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
