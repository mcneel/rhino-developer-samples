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
  CCommandSampleTextureMapping() {}
  ~CCommandSampleTextureMapping() {}
  UUID CommandUUID()
  {
    // {614743C0-A387-45FD-8E43-1C207EFDBB08}
    static const GUID SampleTextureMappingCommand_UUID =
    { 0x614743C0, 0xA387, 0x45FD,{ 0x8E, 0x43, 0x1C, 0x20, 0x7E, 0xFD, 0xBB, 0x08 } };
    return SampleTextureMappingCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleTextureMapping"; }
  const wchar_t* LocalCommandName() const { return L"SampleTextureMapping"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
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

  const CRhinoObject* pObject = selection.Object(0).Object();
  if (NULL == pObject)
    return cancel;

  CRhinoGetInteger get;
  get.SetCommandPrompt(L"Mapping channel");
  get.SetDefaultInteger(0);
  if (CRhinoGet::number != get.GetNumber())
    return cancel;

  // This is the mapping channel on the object we're going to look for.
  const int iMappingChannel = get.Number();
  const UUID uuidCurrentRenderingPlugIn = RhinoApp().GetDefaultRenderApp();

  ON_TextureMapping mappingNew;
  mappingNew.SetSurfaceParameterMapping();

  ON_Xform localXform(1);
  int iIndex = -1;

  CRhinoDoc* pDoc = pObject->Document();
  if (NULL == pDoc)
    return cancel;

  CRhinoTextureMappingTable& table = pDoc->m_texture_mapping_table;
  const ON_MappingRef* pMR = pObject->Attributes().m_rendering_attributes.MappingRef(uuidCurrentRenderingPlugIn);

  // If there's no mapping ref, we can assume that there's no custom mapping so use surface param mapping
  if (pMR)
  {
    // There are iCount mapping channels on this object.  Iterate through them looking for the channel
    const int iCount = pMR->m_mapping_channels.Count();
    for (int i = 0; i < iCount; i++)
    {
      const ON_MappingChannel& mc = pMR->m_mapping_channels[i];
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
  uvw_scale.Scale(0.1, 0.1, 0.1);
  mappingNew.m_uvw = uvw_scale;

  // Set the texture mapping on an object

  // OK - putting it back is a bit of a pain.  But here goes.

  // Copy the attributes
  if (-1 != iIndex)
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
    ON_3dmObjectAttributes attrNew = pObject->Attributes();
    ON_MappingRef* pMRNew = const_cast<ON_MappingRef*>(attrNew.m_rendering_attributes.MappingRef(uuidCurrentRenderingPlugIn));
    if (NULL == pMRNew)
    {
      pMRNew = attrNew.m_rendering_attributes.AddMappingRef(uuidCurrentRenderingPlugIn);
    }

    ASSERT(pMR);

    bool bFound = false;
    for (int i = 0; i < pMR->m_mapping_channels.Count(); i++)
    {
      ON_MappingChannel& mc = const_cast<ON_MappingChannel&>(pMRNew->m_mapping_channels[i]);
      if (mc.m_mapping_channel_id == iMappingChannel)
      {
        // We found one - we can just modify it.
        mc.m_mapping_index = iIndex;
        mc.m_object_xform = localXform;
        bFound = true;
      }
    }

    if (!bFound)
    {
      // Couldn't modify - have to add.
      pMRNew->AddMappingChannel(iMappingChannel, table[iIndex].Id());
    }

    // Now just modify the attributes
    pDoc->ModifyObjectAttributes(CRhinoObjRef(pObject), attrNew);

    // And finally, regen the display to make sure everything updates.
    pDoc->DeferredRedraw();
  }

  // Use the texture mapping to set up TCs on a mesh

  // Now - let's use the thing on another mesh...just some fun:
  ON_Mesh* pMesh = ::RhinoMeshSphere(ON_Sphere(ON_origin, 10.0), 20, 20);

  // This is how to set the texture coordinates on any mesh from the texture mapping on channel 0 of an object.
  pMesh->SetTextureCoordinates(mappingNew, &localXform);

  delete pMesh;
  pMesh = NULL;

  // Get the texture tiling on the material of an object using the old school ON_Material SDK
  const CRhinoMaterial& existing_mat = pObject->ObjectMaterial(uuidCurrentRenderingPlugIn);
  for (int i = 0; i < existing_mat.m_textures.Count(); i++)
  {
    const ON_Texture& tex = existing_mat.m_textures[i];
    if (tex.m_type == ON_Texture::TYPE::bitmap_texture)
    {
      //Texture tiling for the diffuse texture ("bitmap") is in tex.m_uvw;
      //Remember to set tex.m_bApply_uvw
    }
  }

  // Get the texture tiling on the material of an object using the RDK

  const CRhRdkMaterial* pMaterial = pObject->ObjectRdkMaterial(ON_COMPONENT_INDEX());
  if (pMaterial)
  {
    // There's a material - let's see if there's a texture.
    const CRhRdkTexture* pTexture = dynamic_cast<const CRhRdkTexture*>(pMaterial->GetTextureForUsage(CRhRdkMaterial::ChildSlotUsage::kDiffuse));
    if (pTexture)
    {
      //There's a texture in the diffuse slot.  Set the repeat value.

      //Open the texture for writing using "BeginChange"
      CRhRdkTexture& non_const_texture = dynamic_cast<CRhRdkTexture&>(pTexture->BeginChange(RhRdkChangeContext::Program));
      non_const_texture.SetRepeat(ON_3dVector(0.1, 0.1, 0.1));
      non_const_texture.Changed();
      non_const_texture.EndChange();
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleTextureMapping command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

