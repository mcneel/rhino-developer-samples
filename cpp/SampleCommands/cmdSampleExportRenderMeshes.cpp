
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
  CCommandSampleExportRenderMeshes() = default;
  ~CCommandSampleExportRenderMeshes() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x5DDA7654, 0x1B65, 0x4664, { 0xBF, 0xAB, 0xA0, 0xE3, 0xBD, 0x03, 0xF6, 0x2D } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleExportRenderMeshes"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;

private:
  bool DoExport(CRhinoDoc& doc, const CRhinoObjRef& obj_ref, const ON_Mesh* mesh, const CRhinoMaterial* front_material, const CRhinoMaterial* back_material);
};

// The one and only CCommandSampleExportRenderMeshes object
static class CCommandSampleExportRenderMeshes theSampleExportRenderMeshesCommand;

CRhinoCommand::result CCommandSampleExportRenderMeshes::RunCommand(const CRhinoCommandContext& context)
{
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  ON_SimpleArray<const CRhinoObject*> objects;
  CRhinoObjectIterator it(*pDoc, CRhinoObjectIterator::normal_or_locked_objects, CRhinoObjectIterator::active_and_reference_objects);
  for (const CRhinoObject* obj = obj = it.First(); nullptr != obj; obj = it.Next())
  {
    if (obj->IsMeshable(ON::render_mesh))
      objects.Append(obj);
  }

  if (0 == objects.Count())
    return nothing;

  ON_ClassArray<CRhinoObjRef> render_meshes(objects.Count());

  ::RhinoGetRenderMeshes(objects, render_meshes, true, true);
  if (0 == render_meshes.Count())
    return failure;

  const auto render_plugin_id = RhinoApp().GetDefaultRenderApp();
  ON_SimpleArray<CRhinoObjRef::CNestedIRef> nested_iref;

  ON_Xform xform(1); // unused

  for (int i = 0; i < render_meshes.Count(); i++)
  {
    const auto& obj_ref = render_meshes[i];
    const auto* obj = obj_ref.Object();
    if (nullptr == obj)
      continue;

    const auto* attributes = &(obj->Attributes());
    const auto* material_ref = attributes->m_rendering_attributes.MaterialRef(render_plugin_id);

    auto material_source = material_ref ? material_ref->MaterialSource() : attributes->MaterialSource();
    if (ON::material_from_parent == material_source)
    {
      nested_iref.SetCount(0);

      const auto* top_iref = obj_ref.GetInstanceTransformation(xform, &nested_iref);
      if (nullptr != top_iref)
      {
        for (int j = nested_iref.Count(); j >= 0; j--)
        {
          if (j)
          {
            const auto* parent_iref = nested_iref[j - 1].m_iref;
            if (nullptr == parent_iref)
              break;

            attributes = &parent_iref->Attributes();
            material_ref = attributes->m_rendering_attributes.MaterialRef(render_plugin_id);
            material_source = material_ref ? material_ref->MaterialSource() : attributes->MaterialSource();
            if (ON::material_from_parent != material_source)
              break;
          }
          else
          {
            attributes = &top_iref->Attributes();
          }
        }
      }
    }

    const CRhinoMaterial* front_material = nullptr;
    const CRhinoMaterial* back_material  = nullptr;
    pDoc->m_material_table.GetMaterials(render_plugin_id, *attributes, front_material, back_material);

    if ((nullptr == front_material) || (nullptr == back_material))
    {
      const auto& default_material = pDoc->m_material_table[-1];

      if (nullptr == front_material)
        front_material = &default_material;

      if (nullptr == back_material)
        back_material = &default_material;
    }

    DoExport(*pDoc, obj_ref, obj_ref.Mesh(), front_material, back_material);
  }

  return success;
}

bool CCommandSampleExportRenderMeshes::DoExport(CRhinoDoc& doc, const CRhinoObjRef& obj_ref, const ON_Mesh* mesh, const CRhinoMaterial* front_material, const CRhinoMaterial* back_material)
{
  // TODO: Add export code here.

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
