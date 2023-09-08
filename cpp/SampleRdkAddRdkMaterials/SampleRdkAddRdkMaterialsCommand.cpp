
#include "stdafx.h"

static class CSampleRdkAddRdkMaterialsCmd : public CRhinoCommand
{
protected:
  virtual UUID CommandUUID() override { static const UUID uuid = { 0x43991fff, 0x967a, 0x44d8, { 0x86, 0x60, 0x2f, 0x66, 0xc7, 0x40, 0xf2, 0x12 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleRdkAddRdkMaterials"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
}
theSampleRdkAddRdkMaterialsCmd;

static void AttachTestBitmapTextureToMaterial(const CRhinoDoc* pDocAssoc, CRhRdkMaterial& material, const wchar_t* wszChildSlotName)
{
    // Add a bitmap texture to the bitmap slot of the material.
    CRhRdkSimulatedTexture tex(pDocAssoc);
    tex.SetFilename(L"C:\\example_image.jpg", pDocAssoc, false); // TODO: Set the full path to the file.
    auto* pBitmap = ::RhRdkNewBitmapTexture(tex, pDocAssoc);
    if (nullptr != pBitmap)
    {
        material.SetChild(pBitmap, wszChildSlotName);
        material.SetChildSlotOn(wszChildSlotName, true);
    }
}

static void AttachTestWoodTextureToMaterial(const CRhinoDoc* pDocAssoc, CRhRdkMaterial& material, const wchar_t* wszChildSlotName)
{
    auto* pWood = ::RhRdkContentFactoriesEx().NewContentFromTypeEx(uuidWoodTextureType, pDocAssoc);
    if (nullptr != pWood)
    {
        material.SetChild(pWood, wszChildSlotName);
        material.SetChildSlotOn(wszChildSlotName, true);
    }
}

static void AttachTestRuntimeDibTextureToMaterial(const CRhinoDoc* pDocAssoc, CRhRdkMaterial& material, const wchar_t* wszChildSlotName)
{
    const int width = 320, height = 240, depth = 32;
    auto dib = std::make_shared<CRhinoDib>(width, height, depth);

    auto func = [](CRhinoDib::Pixel& pixel, void*)
    {
        // TODO: Fill the dib from your memory pixels.
        pixel.Set(rand() & 255, rand() & 255, rand() & 255, 255);
        return true;
    };

    dib->ProcessPixels(func);

    // Create a dib texture from the dib and set it as a child of the Basic Material in the bump texture slot.
    auto* pTexture = ::RhRdkNewDibTexture(dib, pDocAssoc);
    if (nullptr != pTexture)
    {
        material.SetChild(pTexture, wszChildSlotName);
        material.SetChildSlotOn(wszChildSlotName, true);
    }
}

CRhinoCommand::result CSampleRdkAddRdkMaterialsCmd::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_SimpleArray<CRhRdkMaterial*> materials;

  //METHOD 1:
  //Create a custom material from an ON_Material
  {
      // Create an ON_Material.
      ON_Material mat;
      ON_Color col(255, 0, 0); // Initial color.
      mat.SetDiffuse(col);

      // Create a Basic (Custom) Material using the ON_Material.
      auto* pCustom = CRhRdkMaterial::FromOnMaterial(mat, doc);
      if (nullptr == pCustom)
        return CRhinoCommand::failure;

      pCustom->SetInstanceName(L"#1: Sample Custom");

      AttachTestBitmapTextureToMaterial(doc, *pCustom, CS_MAT_BITMAP_TEXTURE);
      AttachTestWoodTextureToMaterial(doc, *pCustom, CS_MAT_TRANSPARENCY_TEXTURE);
      AttachTestRuntimeDibTextureToMaterial(doc, *pCustom, CS_MAT_BUMP_TEXTURE);

      materials.Append(pCustom);
  }

  //METHOD 2:
  //Create a PBR material from an ON_Material set into PBR mode.
  {
      //Now do the same for a PBR material.
      // Create an ON_Material as a PBR.
      ON_Material mat;

      mat.ToPhysicallyBased();
      ASSERT(mat.IsPhysicallyBased());
      auto pbr = mat.PhysicallyBased();

      ON_4fColor col;
      col.SetRGBA(1.f, 0.f, 0.f, 1.f);
      pbr->SetBaseColor(col);

      auto* pPBR = CRhRdkMaterial::FromOnMaterial(mat, doc);

      pPBR->SetInstanceName(L"#2: Sample PBR");

      AttachTestBitmapTextureToMaterial(doc, *pPBR, CS_MAT_PBR_BASE_COLOR);
      AttachTestWoodTextureToMaterial(doc, *pPBR, CS_MAT_PBR_OPACITY);
      AttachTestRuntimeDibTextureToMaterial(doc, *pPBR, CS_MAT_PBR_METALLIC);

      materials.Append(pPBR);
  }

  //METHOD 3:
  //Create a PBR material from scratch and fiddle with the parameters directly.
  {
      auto* pPBR = static_cast<CRhRdkMaterial*>(::RhRdkContentFactoriesEx().NewContentFromTypeEx(uuidPhysicallyBasedMaterialType, doc));

      ON_4fColor col;
      col.SetRGBA(0.f, 0.f, 1.f, 1.f);

      pPBR->SetParameter(FS_MAT_PBR_BASE_COLOR, col);
      pPBR->SetParameter(FS_MAT_PBR_METALLIC, true);

      pPBR->SetInstanceName(L"#3: Sample PBR Set Param");

      AttachTestBitmapTextureToMaterial(doc, *pPBR, CS_MAT_PBR_BASE_COLOR);
      AttachTestWoodTextureToMaterial(doc, *pPBR, CS_MAT_PBR_ROUGHNESS);
      AttachTestRuntimeDibTextureToMaterial(doc, *pPBR, CS_MAT_PBR_CLEARCOAT);

      materials.Append(pPBR);
  }

  // Attach the Basic Material and PBR to the document.
  auto& contents = doc->Contents().BeginChange(RhRdkChangeContext::Program);
  for (int i=0;i<materials.Count();i++)
  {
      contents.Attach(*materials[i]);
  }
  contents.EndChange();

  // Create a sphere object and assign the custom material to it.
  const ON_Sphere sphere(ON_3dPoint::Origin, 8);
  ON_NurbsSurface ns;
  sphere.GetNurbForm(ns);
  ON_3dmObjectAttributes attr;
  auto* pObject = doc->AddSurfaceObject(ns, &attr);
  if (nullptr != pObject)
  {
    CRhRdkObjectDataAccess da(pObject);
    da.SetMaterialInstanceId(materials[0]->InstanceId());
  }

  // Do the same for a layer with the PBR material
  const ON_Layer l;
  const int index = doc->m_layer_table.AddLayer(l);
  if (index >= 0)
  {
    const auto& layer = doc->m_layer_table[index];
    CRhRdkObjectDataAccess da(&layer);
    da.SetMaterialInstanceId(materials[1]->InstanceId());
  }

  doc->DeferredRedraw();

  return success;
}
