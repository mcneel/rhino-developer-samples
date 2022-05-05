#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtractRenderMesh command
//

#pragma region SampleExtractRenderMesh command

class CCommandSampleExtractRenderMesh : public CRhinoCommand
{
public:
  CCommandSampleExtractRenderMesh() = default;
  UUID CommandUUID() override
  {
    // {12166A45-335D-4C6D-BF1D-2BEBD23518E2}
    static const GUID SampleExtractRenderMeshCommand_UUID =
    { 0x12166A45, 0x335D, 0x4C6D, { 0xBF, 0x1D, 0x2B, 0xEB, 0xD2, 0x35, 0x18, 0xE2 } };
    return SampleExtractRenderMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleExtractRenderMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleExtractRenderMesh object
static class CCommandSampleExtractRenderMesh theSampleExtractRenderMeshCommand;

CRhinoCommand::result CCommandSampleExtractRenderMesh::RunCommand(const CRhinoCommandContext& context)
{
  const ON::mesh_type mt = ON::render_mesh;
  const bool bOkToCreate = true;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surfaces, polysurfaces, meshes, extrusions and block instances");
  go.EnableGroupSelect();
  go.EnableSubObjectSelect(false);

  unsigned int gf = 0;
  gf |= CRhinoObject::surface_object;
  gf |= CRhinoObject::polysrf_object;
  gf |= CRhinoObject::mesh_object;
  gf |= CRhinoObject::instance_reference;
  gf |= CRhinoObject::extrusion_object;
  go.SetGeometryFilter(gf);

  go.GetObjects(1, 0);
  if (go.Result() != CRhinoGet::object)
    return CRhinoCommand::cancel;

  ON_SimpleArray<const CRhinoObject*> object_list;
  for (int i = 0; i < go.ObjectCount(); i++)
    object_list.Append(go.Object(i).Object());

  ON_ClassArray<CRhinoObjRef> render_meshes;
  if (ON::analysis_mesh == mt)
    RhinoGetAnalysisMeshes(object_list, render_meshes, bOkToCreate);
  else
    RhinoGetRenderMeshes(object_list, render_meshes, bOkToCreate);

  for (int i = 0; i < render_meshes.Count(); i++)
  {
    const CRhinoObjRef& objref = render_meshes[i];
    const ON_Mesh* mesh = objref.Mesh();
    if (nullptr != mesh && mesh->m_V.Count() >= 3 || mesh->m_F.Count() > 0)
    {
      const CRhinoObject* rhino_object = objref.Object();
      if (nullptr != rhino_object)
      {
        ON_Xform xform(ON_Xform::IdentityTransformation);
        const CRhinoInstanceObject* iref = objref.GetInstanceTransformation(xform);
        bool bApplyInstanceXform = (nullptr != iref && xform.IsValid() && false == xform.IsIdentity());

        ON_3dmObjectAttributes attribs = rhino_object->Attributes();
        attribs.m_uuid = ON_nil_uuid;
        attribs.RemoveFromAllGroups();
        if (bApplyInstanceXform)
          attribs.Transform(xform);

        ON_Mesh* new_mesh = new ON_Mesh(*mesh);
        if (!new_mesh->HasVertexNormals())
          new_mesh->ComputeVertexNormals();

        CRhinoMeshObject* mesh_object = new CRhinoMeshObject(attribs);
        mesh_object->SetMesh(new_mesh);
        context.m_doc.AddObject(mesh_object);
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleExtractRenderMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
