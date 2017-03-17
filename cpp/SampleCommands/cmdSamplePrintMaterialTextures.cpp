#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePrintMaterialTextures command
//

#pragma region SamplePrintMaterialTextures command

class CCommandSamplePrintMaterialTextures : public CRhinoCommand
{
public:
  CCommandSamplePrintMaterialTextures() {}
  ~CCommandSamplePrintMaterialTextures() {}
  UUID CommandUUID()
  {
    // {515DCFC6-A4F9-42D1-95A0-16FD1923E339}
    static const GUID SamplePrintMaterialTexturesCommand_UUID =
    { 0x515DCFC6, 0xA4F9, 0x42D1, { 0x95, 0xA0, 0x16, 0xFD, 0x19, 0x23, 0xE3, 0x39 } };
    return SamplePrintMaterialTexturesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePrintMaterialTextures"; }
  const wchar_t* LocalCommandName() const { return L"SamplePrintMaterialTextures"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePrintMaterialTextures object
static class CCommandSamplePrintMaterialTextures theSamplePrintMaterialTexturesCommand;

CRhinoCommand::result CCommandSamplePrintMaterialTextures::RunCommand(const CRhinoCommandContext& context)
{
  const int material_count = context.m_doc.m_material_table.MaterialCount();
  for (int mi = 0; mi < material_count; mi++)
  {
    const CRhinoMaterial& material = context.m_doc.m_material_table[mi];
    int ti = material.FindTexture(0, ON_Texture::TYPE::bitmap_texture);
    if (ti >= 0)
    {
      if (material.m_textures[ti].m_image_file_reference.IsSet())
      {
        ON_wString filename = material.m_textures[ti].m_image_file_reference.FullPath();
        RhinoApp().Print(L"Material[%d] = %s\n", mi, filename);
      }
    }
  }
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePrintMaterialTextures command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
