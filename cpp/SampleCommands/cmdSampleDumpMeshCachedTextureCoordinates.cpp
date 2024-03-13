#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDumpMeshCachedTextureCoordinates command
//

#pragma region SampleDumpMeshCachedTextureCoordinates command

class CCommandSampleDumpMeshCachedTextureCoordinates : public CRhinoCommand
{
public:
  CCommandSampleDumpMeshCachedTextureCoordinates() = default;
  UUID CommandUUID() override
  {
    // {941C3313-24F5-4771-91BB-F4823F511CE5}
    static const GUID SampleDumpMeshCachedTextureCoordinatesCommand_UUID =
    { 0x941c3313, 0x24f5, 0x4771, { 0x91, 0xbb, 0xf4, 0x82, 0x3f, 0x51, 0x1c, 0xe5 } };

    return SampleDumpMeshCachedTextureCoordinatesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDumpMeshCachedTextureCoordinates"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDumpMeshCachedTextureCoordinates object
static class CCommandSampleDumpMeshCachedTextureCoordinates theSampleDumpMeshCachedTextureCoordinatesCommand;

CRhinoCommand::result CCommandSampleDumpMeshCachedTextureCoordinates::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  bool rc = gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_3dm_file_dialog, nullptr, RhinoApp().MainWnd());
  if (!rc)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"File not found\n");
    return CRhinoCommand::failure;
  }

  ONX_Model model;
  rc = model.Read(filename);
  if (!rc)
  {
    RhinoApp().Print(L"Error reading file\n");
    return CRhinoCommand::failure;
  }

  ONX_ModelComponentIterator it(model, ON_ModelComponent::Type::ModelGeometry);
  const ON_ModelComponent* model_component = nullptr;
  for (model_component = it.FirstComponent(); nullptr != model_component; model_component = it.NextComponent())
  {
    const ON_ModelGeometryComponent* model_geometry = ON_ModelGeometryComponent::Cast(model_component);
    if (nullptr != model_geometry)
    {
      const ON_Mesh* mesh = ON_Mesh::Cast(model_geometry->Geometry(nullptr));
      const ON_3dmObjectAttributes* attributes = model_geometry->Attributes(nullptr);
      if (nullptr != mesh && nullptr != attributes)
      {
        const ON_ModelComponentReference& render_material_ref = model.MaterialFromAttributes(*attributes);
        const ON_Material* material = ON_Material::FromModelComponentRef(render_material_ref, nullptr);
        if (nullptr != material)
        {
          auto& mapping_refs = attributes->m_rendering_attributes.m_mappings;
          const ON_MappingRef* mapping_ref = mapping_refs.Count() > 0 ? &mapping_refs[0] : nullptr;

          mesh->SetCachedTextureCoordinatesFromMaterial(model, *material, mapping_ref);

          for (int i = 0; i < mesh->m_TC.Count(); i++)
          {
            const ON_TextureCoordinates& tc = mesh->m_TC[i];

            if (tc.m_T.Count() > 0)
            {
              RhinoApp().Print(L"ON_Mesh::m_TC[%d]: (%f, %f)", i, tc.m_T[0].x, tc.m_T[0].y);
              if (tc.m_T.Count() > 1)
              {
                auto last = tc.m_T.Last();
                RhinoApp().Print(L" ... (%f, %f)", (*last).x, (*last).y);
              }

              RhinoApp().Print(L"\n");
            }
          }

          if (material->m_textures.Count() > 0)
            RhinoApp().Print(L"Cached texture coordinates by texture:\n");

          for (int i = 0; i < material->m_textures.Count(); i++)
          {
            RhinoApp().Print(L" Texture (type %d):\n", material->m_textures[i].m_type);

            const ON_TextureCoordinates* pTCs = mesh->GetCachedTextureCoordinates(model, material->m_textures[i], mapping_ref);
            if (nullptr != pTCs)
            {
              RhinoApp().Print(L"  (%f, %f)", pTCs->m_T[0].x, pTCs->m_T[0].y);
              if (pTCs->m_T.Count() > 1)
              {
                auto last = pTCs->m_T.Last();
                RhinoApp().Print(L" ... (%f, %f)", (*last).x, (*last).y);
              }

              RhinoApp().Print(L"\n");
            }
            else
            {
              RhinoApp().Print(L"  None");
            }
          }
        }
      }
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDump3dmMaterialTextures command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
