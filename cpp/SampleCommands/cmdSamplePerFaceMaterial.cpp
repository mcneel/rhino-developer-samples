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
  UUID CommandUUID() override
  {
    // {5B02BE51-25C9-4C83-95C6-E5562E599C31}
    static const GUID SamplePerFaceMaterialCommand_UUID =
    { 0x5B02BE51, 0x25C9, 0x4C83, { 0x95, 0xC6, 0xE5, 0x56, 0x2E, 0x59, 0x9C, 0x31 } };
    return SamplePerFaceMaterialCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePerFaceMaterial"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSamplePerFaceMaterial object
static class CCommandSamplePerFaceMaterial theSamplePerFaceMaterialCommand;

CRhinoCommand::result CCommandSamplePerFaceMaterial::RunCommand(const CRhinoCommandContext& context)
{
  ON_UUID render_plugin_id = RhinoApp().GetDefaultRenderApp();
  ON_UuidIndex idi;

  for (int count = 0; count < 1000; count++)
  {
    memset(&idi, 0, sizeof(idi));
    idi.m_i = -1;

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
    
    const CRhinoObject* obj = go.Object(0).Object();
    if (nullptr == obj)
      return CRhinoCommand::failure;

    const ON_3dmObjectAttributes& att = obj->Attributes();
    if ((idi.m_i = att.m_material_index) >= 0)
    {
      idi.m_id = context.m_doc.m_material_table[idi.m_i].Id();
    }
    else
    {
      const ON_MaterialRef* mat_ref = att.m_rendering_attributes.MaterialRef(render_plugin_id);
      if (nullptr == mat_ref)
        mat_ref = att.m_rendering_attributes.MaterialRef(ON_nil_uuid);
      if (nullptr != mat_ref)
      {
        idi.m_id = mat_ref->m_material_id;
        idi.m_i = context.m_doc.m_material_table.FindMaterial(idi.m_id);
      }
    }
    
    if (idi.m_i >= 0 && !ON_UuidIsNil(idi.m_id))
      break;

    RhinoApp().Print(L"This object does not have a rendering material.\n");
  }

  if (idi.m_i >= 0) for (int count = 0; count < 1000; count++)
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

    const CRhinoBrepObject* brep_object = CRhinoBrepObject::Cast(go.Object(0).Object());
    if (nullptr == brep_object)
      continue;

    const ON_Brep* brep = brep_object->Brep();
    if (nullptr == brep)
      continue;

    const ON_BrepFace* brep_face = ON_BrepFace::Cast(go.Object(0).Surface());
    if (0 == brep_face)
      continue;

    int face_index = brep_face->m_face_index;

    const CRhinoMaterial* front_mat = nullptr;
    const CRhinoMaterial* back_mat = nullptr;
    const ON_3dmObjectAttributes& object_attributes = brep_object->Attributes();
    context.m_doc.m_material_table.GetMaterials(render_plugin_id, object_attributes, front_mat, back_mat);
    if (nullptr == front_mat)
      continue;

    ON_Material mat = *front_mat;
    int mc = -1;
    if (front_mat->Id() != idi.m_id)
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
          context.m_doc.m_material_table.ModifyMaterial(mat, mat.Index());
        }
        else
        {
          mat.SetIndex(context.m_doc.m_material_table.AddMaterial(mat));
        }
      }
    }

    ON_Brep* new_brep = ON_Brep::New(*brep);
    if (new_brep->m_F.Count() > face_index)
    {
      new_brep->m_F[face_index].m_face_material_channel = mc + 1;
      CRhinoBrepObject* new_brep_object = new CRhinoBrepObject(object_attributes);
      new_brep_object->SetBrep(new_brep);
      if (context.m_doc.ReplaceObject(CRhinoObjRef(brep_object), new_brep_object) && front_mat->Index() != mat.Index())
      {
        // modify attributes to use new material
        ON_3dmObjectAttributes new_attributes = new_brep_object->Attributes();
        new_attributes.m_material_index = mat.Index();
        new_attributes.SetMaterialSource(ON::material_from_object);
        new_brep_object->ModifyAttributes(new_attributes);
      }
    }
    else
    {
      delete new_brep;
    }
    context.m_doc.Redraw();
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePerFaceMaterial command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
