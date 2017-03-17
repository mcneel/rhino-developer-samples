#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTexture command
//

class CCommandSampleTexture : public CRhinoCommand
{
public:
  CCommandSampleTexture() {}
  ~CCommandSampleTexture() {}
  UUID CommandUUID()
  {
    // {50B79DB6-D2B7-4696-89D7-0BA711D5E8DE}
    static const GUID SampleTextureCommand_UUID =
    { 0x50B79DB6, 0xD2B7, 0x4696, { 0x89, 0xD7, 0x0B, 0xA7, 0x11, 0xD5, 0xE8, 0xDE } };
    return SampleTextureCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleTexture"; }
  const wchar_t* LocalCommandName() { return L"SampleTexture"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleTexture object
static class CCommandSampleTexture theSampleTextureCommand;

CRhinoCommand::result CCommandSampleTexture::RunCommand(const CRhinoCommandContext& context)
{
  unsigned int geometry_filter =
    CRhinoGetObject::surface_object |
    CRhinoGetObject::polysrf_object |
    CRhinoGetObject::mesh_object;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface, polysurface, or mesh to add texture");
  go.SetGeometryFilter(geometry_filter);
  go.EnablePreSelect(TRUE);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& object_ref = go.Object(0);
  const CRhinoObject* object = object_ref.Object();
  if (0 == object)
    return CRhinoCommand::failure;

  int material_index = object->Attributes().m_material_index;
  if (material_index >= 0)
  {
    RhinoApp().Print(L"Object already has a material assigned.\n");
    return CRhinoCommand::nothing;
  }

  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL bResult = gf.DisplayFileDialog(CRhinoGetFileDialog::open_bitmap_dialog);
  if (!bResult)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"The specified file cannot be found.\n");
    return CRhinoCommand::nothing;
  }

  CRhinoDib dib;
  if (!dib.ReadFromFile(filename))
  {
    RhinoApp().Print(L"The specified file cannot be identifed as a supported type.\n");
    return CRhinoCommand::nothing;
  }

  ON_Material material;
  material.AddTexture(filename, ON_Texture::TYPE::bitmap_texture);

  material_index = context.m_doc.m_material_table.AddMaterial(material);
  if (material_index < 0)
  {
    RhinoApp().Print(L"Failed to add material to material table.\n");
    return CRhinoCommand::nothing;
  }

  CRhinoObjectAttributes attributes(object->Attributes());
  attributes.SetMaterialSource(ON::material_from_object);
  attributes.m_material_index = material_index;

  context.m_doc.ModifyObjectAttributes(object_ref, attributes);
  context.m_doc.Regen();

  return CRhinoCommand::success;
}

//
// END SampleTexture command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
