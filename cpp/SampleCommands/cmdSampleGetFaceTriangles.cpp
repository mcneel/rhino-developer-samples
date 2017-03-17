#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetFaceTriangles command
//

#pragma region SampleGetFaceTriangles command

/*
Description:
  Returns an array of meshes, one for each face in the brep.
Parameters:
  brep   - [in] The brep from which to create the meshes.
  meshes - [out] The output meshes.
Returns:
  The number of meshes added to the output array
Remarks:
  Memory for the meshes is allocated and becomes the responsibility
  of the caller.
*/
static int RhGetBrepFaceTriangles(const ON_Brep* brep,
  ON_SimpleArray<ON_Mesh*>& meshes)
{
  const int meshes_Count = meshes.Count();
  if (brep && brep->IsValid())
  {
    for (int i = 0; i < brep->m_F.Count(); i++)
    {
      ON_Mesh* mesh = new ON_Mesh();
      if (brep->m_F[i].CreateMesh(ON_MeshParameters::FastRenderMesh, mesh))
      {
        mesh->ConvertQuadsToTriangles();
        meshes.Append(mesh);
      }
      else
      {
        delete mesh;
      }
    }
  }

  return meshes.Count() - meshes_Count;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleGetFaceTriangles : public CRhinoCommand
{
public:
  CCommandSampleGetFaceTriangles() {}
  ~CCommandSampleGetFaceTriangles() {}
  UUID CommandUUID()
  {
    // {D1036400-5F8D-4BAE-96E7-AC8666BF0844}
    static const GUID SampleGetFaceTrianglesCommand_UUID =
    { 0xD1036400, 0x5F8D, 0x4BAE, { 0x96, 0xE7, 0xAC, 0x86, 0x66, 0xBF, 0x08, 0x44 } };
    return SampleGetFaceTrianglesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGetFaceTriangles"; }
  const wchar_t* LocalCommandName() const { return L"SampleGetFaceTriangles"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleGetFaceTriangles object
static class CCommandSampleGetFaceTriangles theSampleGetFaceTrianglesCommand;

CRhinoCommand::result CCommandSampleGetFaceTriangles::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select polysurface to mesh");
  go.SetGeometryFilter(CRhinoGetObject::polysrf_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Brep* brep = go.Object(0).Brep();
  if (0 == brep)
    return CRhinoCommand::failure;

  ON_SimpleArray<ON_Mesh*> meshes;
  int mesh_count = RhGetBrepFaceTriangles(brep, meshes);

  for (int i = 0; i < mesh_count; i++)
  {
    ON_Mesh* mesh = meshes[i];
    if (mesh)
    {
      CRhinoMeshObject* mesh_obj = new CRhinoMeshObject();
      // ~CRhinoMeshObject() will delete this mesh
      mesh_obj->SetMesh(mesh);
      context.m_doc.AddObject(mesh_obj);
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGetFaceTriangles command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
