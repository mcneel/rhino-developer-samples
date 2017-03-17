#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleReduceMesh command
//

#pragma region SampleReduceMesh command

#include <omp.h>

class CCommandSampleReduceMesh : public CRhinoCommand
{
public:
  CCommandSampleReduceMesh() {}
  ~CCommandSampleReduceMesh() {}
  UUID CommandUUID()
  {
    // {7A88F21C-32C-46C5-B3EC-09DC82786B0D}
    static const GUID SampleReduceMeshCommand_UUID =
    { 0x7A88F21C, 0x32C, 0x46C5, { 0xB3, 0xEC, 0x09, 0xDC, 0x82, 0x78, 0x6B, 0x0D } };
    return SampleReduceMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleReduceMesh"; }
  const wchar_t* LocalCommandName() const { return L"SampleReduceMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleReduceMesh object
static class CCommandSampleReduceMesh theSampleReduceMeshCommand;

CRhinoCommand::result CCommandSampleReduceMesh::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select meshes to reduce");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  ON_SimpleArray<ON_Mesh*> meshes(object_count);
  for (int i = 0; i < object_count; i++)
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    meshes.Append((0 == mesh) ? 0 : mesh->Duplicate());
  }

  const int max_num_threads = 8;
  int num_threads = 0;
  omp_set_num_threads(max_num_threads);

#pragma omp parallel shared(meshes, num_threads)
  {
#pragma omp master
    num_threads = omp_get_num_threads();

#pragma omp for
    for (int i = 0; i < object_count; i++)
    {
      if (0 != meshes[i])
      {
        ON_Mesh& mesh = *(meshes[i]);
        mesh.CullDegenerateFaces();
        if (mesh.ConvertQuadsToTriangles())
        {
          bool rc = RhinoReduceMesh(mesh, (int)(mesh.m_F.Count() * 0.5), true, 10, true);
          if (rc)
            mesh.ConvertTrianglesToQuads(5.0 * ON_PI / 180.0, 2.0);
        }
      }
    }
  }

  for (int i = 0; i < object_count; i++)
  {
    if (0 != meshes[i])
    {
      CRhinoMeshObject* new_obj = new CRhinoMeshObject();
      new_obj->SetMesh(meshes[i]);
      meshes[i] = 0;
      context.m_doc.ReplaceObject(go.Object(i), new_obj);
    }
  }

  if (num_threads == max_num_threads)
    RhinoApp().Print(L"%d OpenMP threads were used.\n", max_num_threads);
  else
    RhinoApp().Print(L"Expected %d OpenMP threads, but %d were used.\n", max_num_threads, num_threads);

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleReduceMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
