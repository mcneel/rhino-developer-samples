#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshBoundary command
//

class CCommandSampleMeshBoundary : public CRhinoCommand
{
public:
  CCommandSampleMeshBoundary() = default;
  UUID CommandUUID() override
  {
    // {2B83C913-6E82-49EC-AB0E-CBFFC1841797}
    static const GUID SampleMeshBoundaryCommand_UUID =
    { 0x2B83C913, 0x6E82, 0x49EC, { 0xAB, 0x0E, 0xCB, 0xFF, 0xC1, 0x84, 0x17, 0x97 } };
    return SampleMeshBoundaryCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMeshBoundary"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMeshBoundary object
static class CCommandSampleMeshBoundary theSampleMeshBoundaryCommand;

CRhinoCommand::result CCommandSampleMeshBoundary::RunCommand(const CRhinoCommandContext& context)
{
  // Select an open mesh object
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select open mesh");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::open_mesh);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Validate the selection
  const CRhinoObjRef& ref = go.Object(0);
  const ON_Mesh* mesh = ref.Mesh();
  if (nullptr == mesh)
    return CRhinoCommand::failure;

  // Get the mesh's topology
  const ON_MeshTopology& mesh_top = mesh->Topology();
  ON_SimpleArray<const ON_Curve*> lines;

  // Find all of the mesh edges that have only a single mesh face
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
        ON_LineCurve* line = new ON_LineCurve(p0, p1);
        if (nullptr != line && line->IsValid())
          lines.Append(line);
        else
          delete line; // Don't leak...
      }
    }
  }

  // Join all of the line segments
  ON_SimpleArray<ON_Curve*> output;
  if (RhinoMergeCurves(lines, output, ON_ZERO_TOLERANCE))
  {
    for (int i = 0; i < output.Count(); i++)
    {
      CRhinoCurveObject* obj = new CRhinoCurveObject();
      obj->SetCurve(output[i]);
      if (context.m_doc.AddObject(obj))
        obj->Select();
      else
        delete obj; // Don't leak...
    }
    context.m_doc.Redraw();
  }

  // Clean up
  for (int i = 0; i < lines.Count(); i++)
    delete lines[i]; // Don't leak...

  return CRhinoCommand::success;
}

//
// END SampleMeshBoundary command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
