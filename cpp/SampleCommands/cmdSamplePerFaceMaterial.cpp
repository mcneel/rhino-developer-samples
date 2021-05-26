
#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePerFaceMaterial command
//

#pragma region SamplePerFaceMaterial command

class CCommandSamplePerFaceMaterial : public CRhinoCommand
{
public:
  CCommandSamplePerFaceMaterial() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x5B02BE51, 0x25C9, 0x4C83, { 0x95, 0xC6, 0xE5, 0x56, 0x2E, 0x59, 0x9C, 0x31 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SamplePerFaceMaterial"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSamplePerFaceMaterial object
static class CCommandSamplePerFaceMaterial theSamplePerFaceMaterialCommand;

CRhinoCommand::result CCommandSamplePerFaceMaterial::RunCommand(const CRhinoCommandContext& context)
{
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  const auto render_plugin_id = RhinoApp().GetDefaultRenderApp();

  ON_UuidIndex idi;
  int count = 0;
  while (true)
  {
    idi.m_i = -1;
    idi.m_id = ON_nil_uuid;

    CRhinoGetObject go;
    go.SetCommandPrompt(L"Select object with the rendering material you want to assign");
    if (count > 0)
    {
      go.EnablePreSelect(false);
      go.EnablePostSelect(true);
    }

    go.GetObjects(1, 1);
    if (CRhinoCommand::success != go.CommandResult())
      return go.CommandResult();

    const auto* obj = go.Object(0).Object();
    if (nullptr == obj)
      return failure;

    const auto& att = obj->Attributes();
    if ((idi.m_i = att.m_material_index) >= 0)
    {
      idi.m_id = pDoc->m_material_table[idi.m_i].Id();
    }
    else
    {
      const auto* mat_ref = att.m_rendering_attributes.MaterialRef(render_plugin_id);
      if (nullptr == mat_ref)
      {
        mat_ref = att.m_rendering_attributes.MaterialRef(ON_nil_uuid);
      }

      if (nullptr != mat_ref)
      {
        idi.m_id = mat_ref->m_material_id;
        idi.m_i = pDoc->m_material_table.FindMaterial(idi.m_id);
      }
    }

    if ((idi.m_i >= 0) && ON_UuidIsNotNil(idi.m_id))
      break;

    RhinoApp().Print(L"The selected object does not have a rendering material.\n");
    count++;
  }

  UUID uuidInstance = ON_nil_uuid;

  while (true)
  {
    CRhinoGetObject go;
    go.SetGeometryFilter(ON::surface_object);
    go.SetCommandPrompt(L"Select surface you want to use the rendering material");
    go.AcceptNothing();
    go.EnablePreSelect(false);
    go.EnablePostSelect(true);
    go.GetObjects(1, 1);
    if (CRhinoCommand::success != go.CommandResult())
      return go.CommandResult();

    if (CRhinoGet::nothing == go.Result())
      break;

    const auto& obj_ref = go.Object(0);

    const auto* brep_object = CRhinoBrepObject::Cast(obj_ref.Object());
    if (nullptr == brep_object)
    {
      RhinoApp().Print(L"The selected item is not a brep object\n");
      continue;
    }

    const auto* brep = brep_object->Brep();
    if (nullptr == brep)
      continue;

    const auto* brep_face = ON_BrepFace::Cast(obj_ref.Surface());
    if (nullptr == brep_face)
      continue;

    const CRhinoMaterial* front_mat = nullptr;
    const CRhinoMaterial* back_mat = nullptr;
    const auto& object_attributes = brep_object->Attributes();
    pDoc->m_material_table.GetMaterials(render_plugin_id, object_attributes, front_mat, back_mat);
    if (nullptr == front_mat)
      continue;

    ON_Material mat = *front_mat;
    int mc = -1;
    if (mat.Id() != idi.m_id)
    {
      for (mc = 0; mc < mat.m_material_channel.Count(); mc++)
      {
        if (idi.m_id == mat.m_material_channel[mc].m_id)
          break;
      }

      if (mc >= mat.m_material_channel.Count())
      {
        mc = mat.m_material_channel.Count();
        mat.m_material_channel.Append(idi);
        if (mat.Index() >= 0)
        {
          pDoc->m_material_table.ModifyMaterial(mat, mat.Index());
        }
        else
        {
          if (ON_UuidIsNil(uuidInstance))
          {
            // The main object material is not yet in the document material table. Add it.
            auto* pBM = ::RhRdkNewBasicMaterial(mat, pDoc);
            if (nullptr != pBM)
            {
              auto& contents = pDoc->Contents().BeginChange(RhRdkChangeContext::Program);
              contents.Attach(*pBM);
              contents.EndChange();

              uuidInstance = pBM->InstanceId();
            }
          }

          mat.SetRdkMaterialInstanceId(uuidInstance);
          mat.SetMaterialPlugInId(uuidUniversalRenderEngine);

          const auto newMatIndex = pDoc->m_material_table.AddMaterial(mat);
          mat.SetIndex(newMatIndex);
        }
      }
    }

    const int face_index = brep_face->m_face_index;
    if (face_index >= brep->m_F.Count())
      return failure;

    auto* new_brep = ON_Brep::New(*brep);
    if (nullptr == new_brep)
      return failure;

    new_brep->m_F[face_index].m_face_material_channel = mc + 1;

    auto* new_brep_object = new CRhinoBrepObject(object_attributes);
    new_brep_object->SetBrep(new_brep);

    if (pDoc->ReplaceObject(CRhinoObjRef(brep_object), new_brep_object) && front_mat->Index() != mat.Index())
    {
      // modify attributes to use new material
      auto new_attributes = new_brep_object->Attributes();
      new_attributes.m_material_index = mat.Index();
      new_attributes.SetMaterialSource(ON::material_from_object);
      new_brep_object->ModifyAttributes(new_attributes);
    }

    pDoc->Redraw();
  }

  return success;
}

#pragma endregion

//
// END SamplePerFaceMaterial command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
