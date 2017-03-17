#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDupMeshBorder command
//

#pragma region SampleDupMeshBorder command

class CCommandSampleDupMeshBorder : public CRhinoCommand
{
public:
  CCommandSampleDupMeshBorder() {}
  ~CCommandSampleDupMeshBorder() {}
  UUID CommandUUID()
  {
    // {465B9FB5-542F-4AAF-A540-511217FA81FE}
    static const GUID SampleDupMeshBorderCommand_UUID =
    { 0x465B9FB5, 0x542F, 0x4AAF, { 0xA5, 0x40, 0x51, 0x12, 0x17, 0xFA, 0x81, 0xFE } };
    return SampleDupMeshBorderCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDupMeshBorder"; }
  const wchar_t* LocalCommandName() const { return L"SampleDupMeshBorder"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDupMeshBorder object
static class CCommandSampleDupMeshBorder theSampleDupMeshBorderCommand;

CRhinoCommand::result CCommandSampleDupMeshBorder::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::open_mesh);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Mesh* mesh = go.Object(0).Mesh();
  if (0 == mesh)
    return CRhinoCommand::failure;

  if (mesh->IsClosed())
    return CRhinoCommand::nothing;

  const ON_MeshTopology& mesh_top = mesh->Topology();
  for (int i = 0; i < mesh_top.m_tope.Count(); i++)
  {
    const ON_MeshTopologyEdge& mesh_edge = mesh_top.m_tope[i];
    if (mesh_edge.m_topf_count == 1)
    {
      int vtopi0 = mesh_edge.m_topvi[0];
      ON_3dPoint p0 = mesh->Vertex(mesh_top.m_topv[vtopi0].m_vi[0]);

      int vtopi1 = mesh_edge.m_topvi[1];
      ON_3dPoint p1 = mesh->Vertex(mesh_top.m_topv[vtopi1].m_vi[0]);

      if (p0.IsValid() && p1.IsValid())
      {
        ON_Line line(p0, p1);
        if (line.IsValid())
        {
          CRhinoCurveObject* line_obj = context.m_doc.AddCurveObject(line);
          if (line_obj)
            line_obj->Select(true);
        }
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDupMeshBorder command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
