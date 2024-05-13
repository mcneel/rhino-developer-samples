#include "stdafx.h"

static ON_wString TextureTypeString(const ON_Texture::TYPE type)
{
  switch (type)
  {
    case ON_Texture::TYPE::no_texture_type: return L"no_texture_type";
    case ON_Texture::TYPE::bump_texture: return L"bump_texture";
    case ON_Texture::TYPE::transparency_texture: return L"transparency_texture";
    case ON_Texture::TYPE::pbr_base_color_texture: return L"pbr_base_color_texture";
    case ON_Texture::TYPE::pbr_subsurface_texture: return L"pbr_subsurface_texture";
    case ON_Texture::TYPE::pbr_subsurface_scattering_color_texture: return L"pbr_subsurface_scattering_color_texture";
    case ON_Texture::TYPE::pbr_subsurface_scattering_radius_texture: return L"pbr_subsurface_scattering_radius_texture";
    case ON_Texture::TYPE::pbr_metallic_texture: return L"pbr_metallic_texture";
    case ON_Texture::TYPE::pbr_specular_texture: return L"pbr_specular_texture";
    case ON_Texture::TYPE::pbr_specular_tint_texture: return L"pbr_specular_tint_texture";
    case ON_Texture::TYPE::pbr_roughness_texture: return L"pbr_roughness_texture";
    case ON_Texture::TYPE::pbr_anisotropic_texture: return L"pbr_anisotropic_texture";
    case ON_Texture::TYPE::pbr_anisotropic_rotation_texture: return L"pbr_anisotropic_rotation_texture";
    case ON_Texture::TYPE::pbr_sheen_texture: return L"pbr_sheen_texture";
    case ON_Texture::TYPE::pbr_sheen_tint_texture: return L"pbr_sheen_tint_texture";
    case ON_Texture::TYPE::pbr_clearcoat_texture: return L"pbr_clearcoat_texture";
    case ON_Texture::TYPE::pbr_clearcoat_roughness_texture: return L"pbr_clearcoat_roughness_texture";
    case ON_Texture::TYPE::pbr_opacity_ior_texture: return L"pbr_opacity_ior_texture";
    case ON_Texture::TYPE::pbr_opacity_roughness_texture: return L"pbr_opacity_roughness_texture";
    case ON_Texture::TYPE::pbr_emission_texture: return L"pbr_emission_texture";
    case ON_Texture::TYPE::pbr_ambient_occlusion_texture: return L"pbr_ambient_occlusion_texture";
    case ON_Texture::TYPE::pbr_displacement_texture: return L"pbr_displacement_texture";
    case ON_Texture::TYPE::pbr_clearcoat_bump_texture: return L"pbr_clearcoat_bump_texture";
    case ON_Texture::TYPE::pbr_alpha_texture: return L"pbr_alpha_texture";
    case ON_Texture::TYPE::emap_texture: return L"";
    default: return L"unknown_type";
  }
}



class CCommandSampleCachedTextureCoordinates : public CRhinoCommand
{
public:
  CCommandSampleCachedTextureCoordinates() = default;
  ~CCommandSampleCachedTextureCoordinates() = default;
  UUID CommandUUID() override
  {
    // {E7C7DF80-FCC8-468B-BC73-63EBE6081941}
    static const GUID SampleCachedTextureCoordinatesCommand_UUID =
    { 0xe7c7df80, 0xfcc8, 0x468b, { 0xbc, 0x73, 0x63, 0xeb, 0xe6, 0x8, 0x19, 0x41 } };
    return SampleCachedTextureCoordinatesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCachedTextureCoordinates"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDrawString object
static class CCommandSampleCachedTextureCoordinates theSampleCachedTextureCoordinatesCommand;

CRhinoCommand::result CCommandSampleCachedTextureCoordinates::RunCommand(const CRhinoCommandContext& context)
{
  const CRhinoDoc* pDoc = context.Document();
  if (nullptr == pDoc)
    return CRhinoCommand::failure;  

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects");
  CRhinoGet::result res = go.GetObjects(1, 0);
  if (res != CRhinoGet::result::object)
    return go.CommandResult();

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& rhinoObjectRef = go.Object(i);
    const CRhinoObject* pRhinoObject = rhinoObjectRef.Object();
    if (nullptr == pRhinoObject)
      continue;

    // Get the render meshes from the object
    ON_SimpleArray<const ON_Mesh*> meshes;
    pRhinoObject->GetMeshes(ON::mesh_type::render_mesh, meshes);

    // If there are no render meshes, create them and get them again
    if (meshes.Count() == 0)
    {
      ON_MeshParameters mp;
      pRhinoObject->GetRenderMeshParameters(mp);
      const_cast<CRhinoObject*>(pRhinoObject)->CreateMeshes(ON::mesh_type::render_mesh, mp, false);
      meshes = pRhinoObject->GetMeshes(ON::mesh_type::render_mesh, meshes);
      if (meshes.Count() == 0)
      {
        continue;
      }
    }

    ON_wString sObjectId;
    ON_UuidToString(pRhinoObject->Id(), sObjectId);

    RhinoApp().Print(L"Object %s with %u meshes\n", (const wchar_t*)sObjectId, meshes.UnsignedCount());

    // Get the mapping ref for the object. Mapping ref is needed to access object's texture mappings.
    // Mapping ref may be null if there are no mappings but cached texture coordinates methods will handle this case.
    // To get a mapping ref for a specific render plug-in pass in its id.
    const ON_MappingRef* pMappingRef = pRhinoObject->Attributes().m_rendering_attributes.MappingRef(ON_nil_uuid);

    // Iterate through the meshes, each polysurface face has a corresponding mesh
    for (int mi = 0; mi < meshes.Count(); mi++)
    {
      const ON_Mesh* pMesh = meshes[mi];
      if (nullptr == pMesh)
        continue;

      // Figure out what is the component type of this object's subobjects
      ON_COMPONENT_INDEX::TYPE subMaterialComponentType = ON_COMPONENT_INDEX::TYPE::invalid_type;
      if (pRhinoObject->ObjectType() == ON::object_type::subd_object)
        subMaterialComponentType = ON_COMPONENT_INDEX::TYPE::subd_face;
      else if (pRhinoObject->ObjectType() == ON::object_type::brep_object)
        subMaterialComponentType = ON_COMPONENT_INDEX::TYPE::brep_face;
      else if (pRhinoObject->ObjectType() == ON::object_type::extrusion_object)
        subMaterialComponentType = ON_COMPONENT_INDEX::TYPE::brep_face;

      // Get object material. If the current subobject uses a subobject material then it is returned.
      ON_COMPONENT_INDEX ci(subMaterialComponentType, mi);
      const CRhinoMaterial& meshMaterial = pRhinoObject->ObjectMaterial(ci);

      ON_wString sMaterialId;
      ON_UuidToString(meshMaterial.Id(), sMaterialId);
      RhinoApp().Print(L"  Mesh %u material %s\n", mi, (const wchar_t*)sMaterialId);

      // Set up cached texture coordinates based on the material texture channels. 
      pMesh->SetCachedTextureCoordinatesFromMaterial(*pDoc, meshMaterial, pMappingRef);

      // Get all the textures used by the material
      for (int ti = 0; ti < meshMaterial.m_textures.Count(); ti++)
      {
        const ON_Texture& texture = meshMaterial.m_textures[ti];

        // Get the cached texture coordinates for this texture
        const ON_TextureCoordinates* pCachedTCs = pMesh->GetCachedTextureCoordinates(*pDoc, texture, pMappingRef);
        if (nullptr != pCachedTCs  && pCachedTCs->m_T.Count() == pMesh->VertexCount())
        {
          ON_wString sTextureType = TextureTypeString(texture.m_type);
          RhinoApp().Print(L"    Cached texture coordinates for %s texture. ", (const wchar_t*)sTextureType);
          if (texture.IsWcsProjected())
            RhinoApp().Print(L"Wcs projection used.");
          else if (texture.IsWcsBoxProjected())
            RhinoApp().Print(L"Wcs box projection used.");
          else if (texture.m_mapping_channel_id > 0)
            RhinoApp().Print(L"Mapping channel %d used.", texture.m_mapping_channel_id);
          RhinoApp().Print(L"\n");
        } 
      }
    }
  }
  return CRhinoCommand::success;
}

