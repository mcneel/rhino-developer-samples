#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshVertexLocation command
//

#pragma region SampleMeshVertexLocation command

class CCommandSampleMeshVertexLocation : public CRhinoCommand
{
public:
  CCommandSampleMeshVertexLocation() {}
  ~CCommandSampleMeshVertexLocation() {}
  UUID CommandUUID()
  {
    // {D82219DF-58B2-454D-BCB6-3B9095BEB8BB}
    static const GUID SampleMeshVertexLocationCommand_UUID =
    { 0xD82219DF, 0x58B2, 0x454D, { 0xBC, 0xB6, 0x3B, 0x90, 0x95, 0xBE, 0xB8, 0xBB } };
    return SampleMeshVertexLocationCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshVertexLocation"; }
  const wchar_t* LocalCommandName() const { return L"SampleMeshVertexLocation"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshVertexLocation object
static class CCommandSampleMeshVertexLocation theSampleMeshVertexLocationCommand;

CRhinoCommand::result CCommandSampleMeshVertexLocation::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  // Select a mesh vertex
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh vertex");
  go.SetGeometryFilter(CRhinoGetObject::meshvertex_filter);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Get the mesh vertex reference
  const ON_MeshComponentRef* mesh_vertex_ref = go.Object(0).MeshComponentRef();
  if (0 == mesh_vertex_ref)
    return CRhinoCommand::failure;

  // Get the mesh topology
  const ON_MeshTopology* mesh_top = mesh_vertex_ref->MeshTopology();

  // Get the topological mesh vertex
  const ON_MeshTopologyVertex* mesh_top_vertex = mesh_vertex_ref->MeshTopologyVertex();
  if (0 == mesh_top || 0 == mesh_top_vertex)
    return CRhinoCommand::failure;

  // Iterate through all of of the topological mesh edges, that reference
  // this topological mesh vertex, and see if one of the edges is a naked edge.
  bool bBoundaryVertex = false;
  for (int i = 0; i < mesh_top_vertex->m_tope_count; i++)
  {
    const ON_MeshTopologyEdge& mesh_top_edge = mesh_top->m_tope[mesh_top_vertex->m_topei[i]];
    // If the edge has only one face, which means it is a naked edge,
    // then the vertex must be on a boundary edge.
    if (1 == mesh_top_edge.m_topf_count)
    {
      bBoundaryVertex = true;
      break;
    }
  }

  // Report results
  if (bBoundaryVertex)
    RhinoApp().Print(L"Mesh vertex is on a boundary edge.\n");
  else
    RhinoApp().Print(L"Mesh vertex is on an interior edge.\n");

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMeshVertexLocation command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
