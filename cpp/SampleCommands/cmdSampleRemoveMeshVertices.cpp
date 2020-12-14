#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRemoveMeshVertices command
//

#pragma region SampleRemoveMeshVertices command

class CMeshTopVertexHelper
{
public:
  CMeshTopVertexHelper();

public:
  const CRhinoMeshObject* m_mesh_obj;
  ON_SimpleArray<unsigned int> m_meshtop_vertices;
};

CMeshTopVertexHelper::CMeshTopVertexHelper()
  : m_mesh_obj(nullptr)
{
}

////////////////////////////////////////////////////////////////

class CCommandSampleRemoveMeshVertices : public CRhinoCommand
{
public:
  CCommandSampleRemoveMeshVertices() = default;
  UUID CommandUUID() override
  {
    // {7C47DDD5-E68B-4F3D-8355-FF9754AEAC16}
    static const GUID SampleRemoveMeshVerticesCommand_UUID =
    { 0x7C47DDD5, 0xE68B, 0x4F3D, { 0x83, 0x55, 0xFF, 0x97, 0x54, 0xAE, 0xAC, 0x16 } };
    return SampleRemoveMeshVerticesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleRemoveMeshVertices"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRemoveMeshVertices object
static class CCommandSampleRemoveMeshVertices theSampleRemoveMeshVerticesCommand;

CRhinoCommand::result CCommandSampleRemoveMeshVertices::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh vertices to remove");
  go.SetGeometryFilter(CRhinoGetObject::meshvertex_filter);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  ON_ClassArray<CMeshTopVertexHelper> helper_list;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef obj_ref = go.Object(i);

    const ON_MeshComponentRef* mesh_ref = obj_ref.MeshComponentRef();
    if (nullptr == mesh_ref)
      return CRhinoCommand::failure;

    const CRhinoMeshObject* mesh_obj = CRhinoMeshObject::Cast(obj_ref.Object());
    if (nullptr == mesh_obj)
      return CRhinoCommand::failure;

    unsigned int meshtop_vertex = mesh_ref->MeshTopologyVertexIndex();

    int index = -1;
    for (int j = 0; j < helper_list.Count(); j++)
    {
      if (helper_list[j].m_mesh_obj == mesh_obj)
      {
        index = j;
        break;
      }
    }

    if (index < 0)
    {
      CMeshTopVertexHelper& helper = helper_list.AppendNew();
      helper.m_mesh_obj = mesh_obj;
      index = helper_list.Count() - 1;
    }

    if (index >= 0)
      helper_list[index].m_meshtop_vertices.Append(meshtop_vertex);
  }

  for (int i = 0; i < helper_list.Count(); i++)
  {
    CMeshTopVertexHelper& helper = helper_list[i];
    if (nullptr == helper.m_mesh_obj || 0 == helper.m_meshtop_vertices.Count())
      continue;

    const ON_Mesh* mesh = helper.m_mesh_obj->Mesh();
    if (nullptr == mesh)
      continue;

    helper.m_meshtop_vertices.QuickSort(&ON_CompareIncreasing<unsigned int>);

    for (int j = helper.m_meshtop_vertices.Count() - 1; j > 0; j--)
    {
      if (helper.m_meshtop_vertices[j] == helper.m_meshtop_vertices[j - 1])
        helper.m_meshtop_vertices.Remove(j);
    }

    ON_SimpleArray<ON_COMPONENT_INDEX> ci_list;
    for (int i = 0; i < helper.m_meshtop_vertices.Count(); i++)
    {
      ON_COMPONENT_INDEX ci(ON_COMPONENT_INDEX::meshtop_vertex, helper.m_meshtop_vertices[i]);
      ci_list.Append(ci);
    }

    ON_Mesh new_mesh(*mesh);
    if (!new_mesh.HasMeshTopology())
      new_mesh.Topology();

    if (new_mesh.DeleteComponents(ci_list))
    {
      new_mesh.Compact();
      context.m_doc.ReplaceObject(CRhinoObjRef(helper.m_mesh_obj), new_mesh);
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRemoveMeshVertices command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
