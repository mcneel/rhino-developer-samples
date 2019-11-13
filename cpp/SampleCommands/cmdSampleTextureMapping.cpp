#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTextureMapping command
//

#pragma region SampleTextureMapping command

class CCommandSampleTextureMapping : public CRhinoCommand
{
public:
  CCommandSampleTextureMapping() = default;
  virtual ~CCommandSampleTextureMapping() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x614743C0, 0xA387, 0x45FD,{ 0x8E, 0x43, 0x1C, 0x20, 0x7E, 0xFD, 0xBB, 0x08 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleTextureMapping"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleTextureMapping object
static class CCommandSampleTextureMapping theSampleTextureMappingCommand;

CRhinoCommand::result CCommandSampleTextureMapping::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  // Get the texture mapping from an object
  CRhinoGetObject selection;
  selection.EnablePreSelect(false);
  selection.SetCommandPrompt(L"Select object");
  selection.GetObjects(1, 1);
  if (selection.Result() != CRhinoGet::object)
    return cancel;

  const auto* pObject = selection.Object(0).Object();
  if (nullptr == pObject)
    return cancel;

  CRhinoGetInteger get;
  get.SetCommandPrompt(L"Mapping channel");
  get.SetDefaultInteger(0);
  if (CRhinoGet::number != get.GetNumber())
    return cancel;

  // This is the mapping channel on the object we're going to look for.
  const auto iMappingChannel = get.Number();
  const auto uuidCurrentRenderingPlugIn = RhinoApp().GetDefaultRenderApp();

  ON_TextureMapping mappingNew;
  mappingNew.SetSurfaceParameterMapping();

  ON_Xform localXform(1);
  int iIndex = -1;

  auto* pDoc = pObject->Document();
  if (nullptr == pDoc)
    return cancel;

  auto& table = pDoc->m_texture_mapping_table;
  const auto* pMR = pObject->Attributes().m_rendering_attributes.MappingRef(uuidCurrentRenderingPlugIn);

  // If there's no mapping ref, we can assume that there's no custom mapping so use surface param mapping
  if (nullptr != pMR)
  {
    // There are iCount mapping channels on this object.  Iterate through them looking for the channel.
    const int iCount = pMR->m_mapping_channels.Count();
    for (int i = 0; i < iCount; i++)
    {
      const auto& mc = pMR->m_mapping_channels[i];
      if (iMappingChannel == mc.m_mapping_channel_id)
      {
        // OK - this is the guy.
        iIndex = mc.m_mapping_index;

        // The mapping for an object is modified per object by its local transform.
        localXform = mc.m_object_xform;

        // Now look up the mapping in the table.
        const CRhinoTextureMapping& mapping = table[iIndex];
        
        // OK - that's how to get the one in the table, now to modify it a bit.
        mappingNew = mapping;
      }
    }
  }

  // Modify the texture mapping

  // Change to sphere mapping
  mappingNew.SetSphereMapping(ON_Sphere(ON_origin, 10.0));

  // Add some tiling
  ON_Xform uvw_scale;
  uvw_scale.DiagonalTransformation(0.1, 0.1, 0.1);
  mappingNew.m_uvw = uvw_scale;

  // Set the texture mapping on an object

  // OK - putting it back is a bit of a pain.  But here goes.

  // Copy the attributes
  if (iIndex >= 0)
  {
    // This does everything.
    table.ModifyTextureMapping(mappingNew, iIndex);
  }
  else
  {
    // There's no entry in the table.  We have to add one.
    iIndex = table.AddTextureMapping(mappingNew);

    // In this case, we're going to have to build new attributes for the object
    // because there's no existing custom texture mapping.
    auto attrNew = pObject->Attributes();
    auto* pMRNew = const_cast<ON_MappingRef*>(attrNew.m_rendering_attributes.MappingRef(uuidCurrentRenderingPlugIn));
    if (nullptr == pMRNew)
    {
      pMRNew = attrNew.m_rendering_attributes.AddMappingRef(uuidCurrentRenderingPlugIn);
    }

    ASSERT(pMRNew);

    bool bFound = false;
    if (nullptr != pMRNew)
    {
      for (int i = 0; i < pMRNew->m_mapping_channels.Count(); i++)
      {
        auto& mc = const_cast<ON_MappingChannel&>(pMRNew->m_mapping_channels[i]);
        if (mc.m_mapping_channel_id == iMappingChannel)
        {
          // We found one - we can just modify it.
          mc.m_mapping_index = iIndex;
          mc.m_object_xform = localXform;
          bFound = true;
        }
      }
    }

    if (!bFound)
    {
      // Couldn't modify - have to add.
      pMRNew->AddMappingChannel(iMappingChannel, table[iIndex].Id());
    }

    // Now just modify the attributes.
    pDoc->ModifyObjectAttributes(CRhinoObjRef(pObject), attrNew);

    // And finally, regen the display to make sure everything updates.
    pDoc->DeferredRedraw();
  }

  // Use the texture mapping to set up TCs on a mesh.

  // Now - let's use the thing on another mesh...just some fun:
  auto* pMesh = ::RhinoMeshSphere(ON_Sphere(ON_origin, 10.0), 20, 20);
  if (nullptr != pMesh)
  {
    // This is how to set the texture coordinates on any mesh from the texture mapping on channel 0 of an object.
    pMesh->SetTextureCoordinates(mappingNew, &localXform);

    delete pMesh;
    pMesh = nullptr;
  }

  // Get the texture tiling on the material of an object using the old school ON_Material SDK
  const auto& existing_mat = pObject->ObjectMaterial(uuidCurrentRenderingPlugIn);
  for (int i = 0; i < existing_mat.m_textures.Count(); i++)
  {
    const auto& tex = existing_mat.m_textures[i];
    if (tex.m_type == ON_Texture::TYPE::bitmap_texture)
    {
      //Texture tiling for the diffuse texture ("bitmap") is in tex.m_uvw;
      //Remember to set tex.m_bApply_uvw
    }
  }

  // Get the texture tiling on the material of an object using the RDK.
  const auto* pMaterial = pObject->ObjectRdkMaterial(ON_COMPONENT_INDEX());
  if (nullptr != pMaterial)
  {
    // There's a material - let's see if there's a texture.
    const auto* pTexture = dynamic_cast<const CRhRdkTexture*>(pMaterial->GetTextureForUsage(CRhRdkMaterial::ChildSlotUsage::Diffuse));
    if (nullptr != pTexture)
    {
      //There's a texture in the diffuse slot.  Set the repeat value.

      //We need to open the texture for writing using BeginChange().
      auto& tex = static_cast<CRhRdkTexture&>(pTexture->BeginChange(RhRdkChangeContext::Program));
      tex.SetRepeat(ON_3dVector(0.1, 0.1, 0.1));
      tex.EndChange(); // ...and close it again.
    }
  }

  return success;
}

#pragma endregion

//
// END SampleTextureMapping command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
