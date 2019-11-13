
#include "stdafx.h"

class CCommandSampleTexture : public CRhinoCommand
{
public:
  CCommandSampleTexture() = default;
  virtual ~CCommandSampleTexture() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x50B79DB6, 0xD2B7, 0x4696, { 0x89, 0xD7, 0x0B, 0xA7, 0x11, 0xD5, 0xE8, 0xDE } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleTexture"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleTexture object
static class CCommandSampleTexture theSampleTextureCommand;

CRhinoCommand::result CCommandSampleTexture::RunCommand(const CRhinoCommandContext& context)
{
  const auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

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

  const auto& object_ref = go.Object(0);
  const auto* object = object_ref.Object();
  if (nullptr == object)
    return CRhinoCommand::failure;

  if (object->Attributes().m_material_index >= 0)
  {
    RhinoApp().Print(L"Object already has a material assigned.\n");
    return nothing;
  }

  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  const auto bResult = gf.DisplayFileDialog(CRhinoGetFileDialog::open_bitmap_dialog);
  if (!bResult)
    return cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return nothing;

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"The specified file cannot be found.\n");
    return nothing;
  }

  CRhinoDib dib;
  if (!dib.ReadFromFile(pDoc->RuntimeSerialNumber(), filename))
  {
    RhinoApp().Print(L"The specified file cannot be identifed as a supported type.\n");
    return nothing;
  }

  // Create a material and add the texture to it.
  ON_Material mat;
  mat.AddTexture(filename, ON_Texture::TYPE::bitmap_texture);

  // Create an RDK material from the material.
  auto* pMaterial = ::RhRdkNewBasicMaterial(mat, pDoc);
  if (nullptr == pMaterial)
    return nothing;

  // Set the name of the material from the file name.
  ON_wString sName;
  ON_FileSystemPath::SplitPath(filename, nullptr, nullptr, &sName, nullptr);
  pMaterial->SetInstanceName(sName);

  // Attach the material to the document's render content collection.
  auto& contents = pDoc->Contents().BeginChange(RhRdkChangeContext::Program);
  contents.Attach(*pMaterial);
  contents.EndChange();

  // Assign the material to the object.
  CRhRdkObjectDataAccess da(object);
  da.SetMaterialInstanceId(pMaterial);

  return success;
}
