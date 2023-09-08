#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMappingUVWTransform command
//

class CCommandSampleMappingUVWTransform : public CRhinoTestCommand
{
public:
  CCommandSampleMappingUVWTransform() {}
  ~CCommandSampleMappingUVWTransform() {}
  UUID CommandUUID()
  {
    // {0E8A61CC-CFB0-44D4-A90A-DB75C0F2F6C9}
    static const GUID SampleMappingUVWTransformCommand_UUID =
    { 0xe8a61cc, 0xcfb0, 0x44d4, { 0xa9, 0xa, 0xdb, 0x75, 0xc0, 0xf2, 0xf6, 0xc9 } };
    return SampleMappingUVWTransformCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMappingUVWTransform"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context);
};

// The one and only CCommandSampleMappingUVWTransform object
static class CCommandSampleMappingUVWTransform theSampleMappingUVWTransformCommand;

CRhinoCommand::result CCommandSampleMappingUVWTransform::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  const ON_UUID pluginId = RhinoApp().GetDefaultRenderApp();

  // Select brep object
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select brep");
  go.SetGeometryFilter(CRhinoGetObject::polysrf_object);
  const CRhinoGet::result res = go.GetObjects(0, 1);
  if (res != CRhinoGet::result::object)
    return go.CommandResult();
  if (1 != go.ObjectCount())
    return CRhinoCommand::failure;
  CRhinoObjRef objRef = go.Object(0);
  const CRhinoBrepObject* pBrepObject = CRhinoBrepObject::Cast(objRef.Object());
  if (nullptr == pBrepObject)
    return CRhinoCommand::failure;

  // Make sure object has mapping ref
  const ON_MappingRef* pMR = pBrepObject->Attributes().m_rendering_attributes.MappingRef(pluginId);
  if (nullptr == pMR)
  {
    CRhinoObjectAttributes newAttrs(pBrepObject->Attributes());
    newAttrs.m_rendering_attributes.AddMappingRef(pluginId);
    if (!doc->ModifyObjectAttributes(objRef, newAttrs))
      return CRhinoCommand::failure;
  }
  pMR = pBrepObject->Attributes().m_rendering_attributes.MappingRef(pluginId);
  if (nullptr == pMR)
    return CRhinoCommand::failure;

  // Make sure object has at least the default surface parameter mapping
  if (pMR->m_mapping_channels.Count() == 0)
  {
    ON_TextureMapping srfpMapping;
    srfpMapping.SetSurfaceParameterMapping();
    const int mappingTableIndex = doc->m_texture_mapping_table.AddTextureMapping(srfpMapping);
    if (mappingTableIndex < 0)
      return CRhinoCommand::failure;
    CRhinoObjectAttributes newAttrs(pBrepObject->Attributes());
    newAttrs.m_rendering_attributes.AddMappingChannel(pluginId, 1, doc->m_texture_mapping_table[mappingTableIndex].Id());
    if (!doc->ModifyObjectAttributes(objRef, newAttrs))
      return CRhinoCommand::failure;
  }
  pMR = pBrepObject->Attributes().m_rendering_attributes.MappingRef(pluginId);
  if (nullptr == pMR)
    return CRhinoCommand::failure;

  // Define new domain for u, v and w
  ON_Interval uDomain(0.0, 0.5);
  ON_Interval vDomain(0.25, 0.75);
  ON_Interval wDomain(0.0, 1.0);
  ON_Xform uChange, vChange, wChange;
  uChange.IntervalChange(0, ON_Interval(0.0, 1.0), uDomain);
  vChange.IntervalChange(1, ON_Interval(0.0, 1.0), vDomain);
  wChange.IntervalChange(2, ON_Interval(0.0, 1.0), wDomain);

  // Change each mapping to use new domain
  for (int imc = 0; imc < pMR->m_mapping_channels.Count(); imc++)
  {
    const ON_MappingChannel& mc = pMR->m_mapping_channels[imc];
    const int mappingTableIndex = doc->m_texture_mapping_table.FindTextureMapping(&mc);
    if (mappingTableIndex >= 0)
    {
      ON_TextureMapping newMapping(doc->m_texture_mapping_table[mappingTableIndex]);
      newMapping.m_uvw = uChange * vChange * wChange;
      if (!doc->m_texture_mapping_table.ModifyTextureMapping(newMapping, mappingTableIndex))
        return CRhinoCommand::failure;
    }
  }

  return CRhinoCommand::success;
}
