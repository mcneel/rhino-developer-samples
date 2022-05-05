#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDump3dmMaterialTextures command
//

#pragma region SampleDump3dmMaterialTextures command

class CCommandSampleDump3dmMaterialTextures : public CRhinoCommand
{
public:
  CCommandSampleDump3dmMaterialTextures() = default;
  UUID CommandUUID() override
  {
    // {69E96465-4BF4-471A-9DBD-8DEB640C511D}
    static const GUID SampleDump3dmMaterialTexturesCommand_UUID =
    { 0x69E96465, 0x4BF4, 0x471A, { 0x9D, 0xBD, 0x8D, 0xEB, 0x64, 0x0C, 0x51, 0x1D } };
    return SampleDump3dmMaterialTexturesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDump3dmMaterialTextures"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDump3dmMaterialTextures object
static class CCommandSampleDump3dmMaterialTextures theSampleDump3dmMaterialTexturesCommand;

CRhinoCommand::result CCommandSampleDump3dmMaterialTextures::RunCommand(const CRhinoCommandContext& context)
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
      const ON_3dmObjectAttributes* attributes = model_geometry->Attributes(nullptr);
      if (nullptr != attributes)
      {
        const ON_ModelComponentReference& render_material_ref = model.RenderMaterialFromAttributes(*attributes);
        const ON_Material* material = ON_Material::FromModelComponentRef(render_material_ref, nullptr);
        if (nullptr != material)
        {
          const int texture_index = material->FindTexture(nullptr, ON_Texture::TYPE::bitmap_texture, ON_UNSET_INT_INDEX);
          if (texture_index >= 0)
          {
            ON_wString texture_path = material->m_textures[texture_index].m_image_file_reference.FullPath();
            RhinoApp().Print(L"%s\n", static_cast<const wchar_t*>(texture_path));
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
