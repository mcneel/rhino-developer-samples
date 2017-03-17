#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomMeshObjects command
//

class CCustomMeshObjectsUI : public CRhinoMeshObjectsUI
{
public:
  CCustomMeshObjectsUI() {}
  virtual ~CCustomMeshObjectsUI() {}

  // Override of pure virtual CRhinoMeshObjectsUI
  CRhinoCommand::result GetMeshParameters(const ON_SimpleArray<const CRhinoObject*>& objects, ON_MeshParameters& mp)
  {
    UNREFERENCED_PARAMETER(objects);
    mp = m_mp;
    return CRhinoCommand::success;
  }

public:
  ON_MeshParameters m_mp;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleCustomMeshObjects : public CRhinoCommand
{
public:
  CCommandSampleCustomMeshObjects() {}
  ~CCommandSampleCustomMeshObjects() {}
  UUID CommandUUID()
  {
    // {E391201E-D2E8-4CCF-8883-D278D1ED8A68}
    static const GUID SampleCustomMeshObjectsCommand_UUID =
    { 0xE391201E, 0xD2E8, 0x4CCF, { 0x88, 0x83, 0xD2, 0x78, 0xD1, 0xED, 0x8A, 0x68 } };
    return SampleCustomMeshObjectsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCustomMeshObjects"; }
  const wchar_t* LocalCommandName() { return L"SampleCustomMeshObjects"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCustomMeshObjects object
static class CCommandSampleCustomMeshObjects theSampleCustomMeshObjectsCommand;

CRhinoCommand::result CCommandSampleCustomMeshObjects::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface to mesh");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  int i, object_count = go.ObjectCount();
  ON_SimpleArray<const CRhinoObject*> objects(object_count);
  for (i = 0; i < object_count; i++)
  {
    const CRhinoObject* obj = go.Object(i).Object();
    if (obj)
      objects.Append(obj);
  }

  if (0 == objects.Count())
    return CRhinoCommand::failure;

  CCustomMeshObjectsUI ui;
  ui.m_mp.SetRelativeTolerance(0.1);    // Density
  ui.m_mp.SetRefineAngleRadians(0.0);   // Maximum angle (radians)
  ui.m_mp.SetGridAspectRatio(0.0);      // Maximum aspect radio
  ui.m_mp.SetMinimumEdgeLength(0.0001); // Minimum edge length
  ui.m_mp.SetMaximumEdgeLength(0.3);    // Maximum edge length
  ui.m_mp.SetTolerance(0.0);            // Maximum distance, edge to surface
  ui.m_mp.SetGridMinCount(0);           // Minimum initial grid quads
  ui.m_mp.SetRefine(true);              // Refine mesh
  ui.m_mp.SetTextureRange(2);           // Pack textures

  ON_ClassArray<CRhinoObjectMesh> meshes;
  CRhinoCommand::result rc = RhinoMeshObjects(objects, ui.m_mp, ui, meshes);

  if (rc == CRhinoCommand::success)
  {
    for (i = 0; i < meshes.Count(); i++)
    {
      CRhinoObjectMesh& mesh = meshes[i];
      if (mesh.m_mesh)
      {
        mesh.m_mesh->ConvertQuadsToTriangles();

        if (mesh.m_mesh_attributes.GroupCount() > 0)
          mesh.m_mesh_attributes.RemoveFromAllGroups();

        CRhinoMeshObject* mesh_obj = new CRhinoMeshObject(mesh.m_mesh_attributes);
        mesh_obj->SetMesh(mesh.m_mesh);
        mesh.m_mesh = 0;

        context.m_doc.AddObject(mesh_obj);
      }
    }
    context.m_doc.Redraw();
  }

  return rc;
}

//
// END SampleCustomMeshObjects command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
