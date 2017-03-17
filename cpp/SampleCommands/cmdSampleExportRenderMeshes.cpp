#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExportRenderMeshes command
//

#pragma region SampleExportRenderMeshes command

class CCommandSampleExportRenderMeshes : public CRhinoCommand
{
public:
  CCommandSampleExportRenderMeshes() {}
  ~CCommandSampleExportRenderMeshes() {}
  UUID CommandUUID()
  {
    // {5DDA7654-1B65-4664-BFAB-A0E3BD03F62D}
    static const GUID SampleExportRenderMeshesCommand_UUID =
    { 0x5DDA7654, 0x1B65, 0x4664, { 0xBF, 0xAB, 0xA0, 0xE3, 0xBD, 0x03, 0xF6, 0x2D } };
    return SampleExportRenderMeshesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExportRenderMeshes"; }
  const wchar_t* LocalCommandName() const { return L"SampleExportRenderMeshes"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool DoExport(CRhinoDoc& doc, const CRhinoObjRef& obj_ref, const ON_Mesh* mesh, const CRhinoMaterial* front_material, const CRhinoMaterial* back_material);
};

// The one and only CCommandSampleExportRenderMeshes object
static class CCommandSampleExportRenderMeshes theSampleExportRenderMeshesCommand;

CRhinoCommand::result CCommandSampleExportRenderMeshes::RunCommand(const CRhinoCommandContext& context)
{
  ON_SimpleArray<const CRhinoObject*> objects(100);
  CRhinoObjectIterator it(context.m_doc, CRhinoObjectIterator::normal_or_locked_objects, CRhinoObjectIterator::active_and_reference_objects);

  const CRhinoObject* obj = 0;
  for (obj = it.First(); obj; obj = it.Next())
  {
    if (obj->IsMeshable(ON::render_mesh))
      objects.Append(obj);
  }

  if (0 == objects.Count())
    return CRhinoCommand::nothing;

  ON_ClassArray<CRhinoObjRef> render_meshes(objects.Count());
  int render_mesh_count = RhinoGetRenderMeshes(objects, render_meshes, true, true);
  if (0 == render_mesh_count || 0 == render_meshes.Count())
    return CRhinoCommand::failure;

  ON_UUID render_plugin_id = RhinoApp().GetDefaultRenderApp();
  ON_SimpleArray<CRhinoObjRef::CNestedIRef> nested_iref;
  ON_Xform xform(1); // unused

  for (int i = 0; i < render_mesh_count; i++)
  {
    const CRhinoObjRef& obj_ref = render_meshes[i];
    obj = obj_ref.Object();
    if (0 == obj)
      continue;

    const CRhinoObjectAttributes* attributes = &(obj->Attributes());
    ON::object_material_source material_source = ON::material_from_object;
    const ON_MaterialRef* material_ref = attributes->m_rendering_attributes.MaterialRef(render_plugin_id);
    material_source = material_ref ? material_ref->MaterialSource() : attributes->MaterialSource();
    if (ON::material_from_parent == material_source)
    {
      nested_iref.SetCount(0);
      const CRhinoInstanceObject* top_iref = obj_ref.GetInstanceTransformation(xform, &nested_iref);
      if (top_iref)
      {
        for (int j = nested_iref.Count(); j >= 0; j--)
        {
          if (j)
          {
            const CRhinoInstanceObject* parent_iref = nested_iref[j - 1].m_iref;
            if (0 == parent_iref)
              break;

            attributes = &parent_iref->Attributes();
            material_ref = attributes->m_rendering_attributes.MaterialRef(render_plugin_id);
            material_source = material_ref ? material_ref->MaterialSource() : attributes->MaterialSource();
            if (ON::material_from_parent != material_source)
              break;
          }
          else
            attributes = &top_iref->Attributes();
        }
      }
    }

    const CRhinoMaterial* front_material = 0;
    const CRhinoMaterial* back_material = 0;
    context.m_doc.m_material_table.GetMaterials(render_plugin_id, *attributes, front_material, back_material);

    if (0 == front_material)
      front_material = &(context.m_doc.m_material_table[-1]); // default material
    if (0 == back_material)
      back_material = &(context.m_doc.m_material_table[-1]); // default material

    DoExport(context.m_doc, obj_ref, obj_ref.Mesh(), front_material, back_material);
  }

  return CRhinoCommand::success;
}

bool CCommandSampleExportRenderMeshes::DoExport(CRhinoDoc& doc, const CRhinoObjRef& obj_ref, const ON_Mesh* mesh, const CRhinoMaterial* front_material, const CRhinoMaterial* back_material)
{
  // TODO.: Add export code here.

  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(obj_ref);
  UNREFERENCED_PARAMETER(mesh);
  UNREFERENCED_PARAMETER(front_material);
  UNREFERENCED_PARAMETER(back_material);

  return false;
}

#pragma endregion

//
// END SampleExportRenderMeshes command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
