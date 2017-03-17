#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBrepBoxWithMesh command
//

#pragma region SampleBrepBoxWithMesh command

class CCommandSampleBrepBoxWithMesh : public CRhinoCommand
{
public:
  CCommandSampleBrepBoxWithMesh() {}
  ~CCommandSampleBrepBoxWithMesh() {}
  UUID CommandUUID()
  {
    // {5D3C6B55-1551-4DFF-88AE-FC64A2E8949A}
    static const GUID SampleBrepBoxWithMeshCommand_UUID =
    { 0x5D3C6B55, 0x1551, 0x4DFF, { 0x88, 0xAE, 0xFC, 0x64, 0xA2, 0xE8, 0x94, 0x9A } };
    return SampleBrepBoxWithMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleBrepBoxWithMesh"; }
  const wchar_t* LocalCommandName() const { return L"SampleBrepBoxWithMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleBrepBoxWithMesh object
static class CCommandSampleBrepBoxWithMesh theSampleBrepBoxWithMeshCommand;

CRhinoCommand::result CCommandSampleBrepBoxWithMesh::RunCommand(const CRhinoCommandContext& context)
{
  // Get the corners of the box
  CArgsRhinoGetBox args;
  ON_3dPoint corners[8];
  CRhinoCommand::result rc = RhinoGetBox(args, corners);
  if (rc != CRhinoCommand::success)
    return rc;

  // Create a brep from the corners.
  // Note, we are responsible for this memory.
  ON_Brep* brep = ON_BrepBox(corners);
  if (0 == brep)
  {
    ON_Plane plane(corners[0], corners[1], corners[3]);
    if (ON_ZERO_TOLERANCE > plane.plane_equation.ValueAt(corners[7]))
      RhinoApp().Print(L"Box must have a height.\n");
    return CRhinoCommand::failure;
  }

  // Meshing parameters to use when creating a render mesh.
  const ON_MeshParameters& mp = context.m_doc.Properties().RenderMeshParameters();

  // Calculates polygon mesh approximation of the brep.
  // Note, we are responsible for this memory.
  ON_SimpleArray<ON_Mesh*> meshes(brep->m_F.Count());
  int mesh_count = brep->CreateMesh(mp, meshes);
  if (mesh_count == brep->m_F.Count())
  {
    // Remove an previous values of this type of mesh.
    // Note, not necessary with new ON_Brep objects.
    brep->DestroyMesh(ON::render_mesh, true);

    // Transfer meshes from meshes[] array to brep->m_F[].
    // Note, brep will delete meshes.
    for (int i = 0; i < mesh_count; i++)
    {
      if (meshes[i])
      {
        ON_BrepFace& face = brep->m_F[i];
        // Again, not necessary with new ON_Brep objects
        face.DestroyMesh(ON::render_mesh, true);
        face.SetMesh(ON::render_mesh, meshes[i]);
      }
    }

    // Create the runtime brep object
    CRhinoBrepObject* brep_obj = new CRhinoBrepObject();

    // Attach the brep.
    // Note, object will delete brep.
    brep_obj->SetBrep(brep);

    // Add object to the doucment
    if (!context.m_doc.AddObject(brep_obj))
    {
      delete brep_obj; // Don't leak...
      return CRhinoCommand::failure;
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleBrepBoxWithMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
