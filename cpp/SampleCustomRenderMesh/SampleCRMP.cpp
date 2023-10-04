#include "stdafx.h"
#include "SampleCRMP.h"
#include "SampleCustomRenderMeshPlugIn.h"
#include "SampleCustomRenderMeshUserData.h"

CSampleCRMP::CSampleCRMP(void)
{
}

CSampleCRMP::~CSampleCRMP(void)
{
}

UUID CSampleCRMP::PlugInId(void) const
{
	return SampleCustomRenderMeshPlugIn().PlugInID();
}

UUID CSampleCRMP::ProviderId(void) const
{
  // {5F3B9BE4-4663-450C-86DE-098695A0101E}
  static const GUID uuidSampleCRMP =
  { 0x5f3b9be4, 0x4663, 0x450c,{ 0x86, 0xde, 0x9, 0x86, 0x95, 0xa0, 0x10, 0x1e } };

	return uuidSampleCRMP;
}

ON_wString CSampleCRMP::Name(void) const
{
  const static ON_wString wszName = L"SampleCRMP";
  return wszName;
}

bool CSampleCRMP::IsViewDependent(void) const
{
  // Custom render meshes are not dependent on the view
  return false;
}

bool CSampleCRMP::IsRequestingPlugInDependent(void) const
{
  // Custom render meshes are same for all requesting plug-ins
  return false;
}

bool CSampleCRMP::IsPreviewAndStandardSameMesh(void) const
{
  // Preivew and standard meshes are same
  return true;
}

CRhRdkVariant CSampleCRMP::GetParameter(const CRhinoObject&, const wchar_t*) const
{
  return 0;
}

void CSampleCRMP::SetParameter(const CRhinoObject&, const wchar_t*, const CRhRdkVariant&)
{
}

bool CSampleCRMP::WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc&,
                                      const UUID&, const CDisplayPipelineAttributes*) const
{
  // No custom render mesh for document
  if (nullptr == pObject)
    return false;

  // Read sample custom render mesh user data from the object attributes
  const CSampleCustomRenderMeshUserData* pUserData = CSampleCustomRenderMeshUserData::Cast(pObject->Attributes().GetUserData(SampleCustomRenderMeshUserDataUuid));

  // If object doesn't have this type of user data then it doesn't custom render mesh
  if (pUserData == nullptr)
    return false;

  return true;
}

ON_BoundingBox CSampleCRMP::BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
                                        const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const
{
  // No custom render mesh for document. Therefore no need to provide bounding box either.
  if (nullptr == pObject)
    return ON_BoundingBox::EmptyBoundingBox;

  // Read sample custom render mesh user data from the object attributes
  const CSampleCustomRenderMeshUserData* pUserData = CSampleCustomRenderMeshUserData::Cast(pObject->Attributes().GetUserData(SampleCustomRenderMeshUserDataUuid));

  // If object doesn't have this type of user data then it doesn't custom render mesh. So no bounding box needed either.
  if (pUserData == nullptr)
    return ON_BoundingBox::EmptyBoundingBox;

  // Calculate bounding box for the custom render mesh. This does not need to be a tight bounding box.
  ON_BoundingBox bb = pObject->BoundingBox();
  ON_Xform translationXform = ON_Xform::TranslationTransformation(0.0, 0.0, pUserData->Amount());
  bb.Transform(translationXform);
  return bb;
}

bool CSampleCRMP::BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
                                    IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes,
                                    bool bWillBuildCustomMeshCheck) const
{
  const CRhinoObject* pObject = crmInOut.Object();

  // No custom render mesh for document
  if (nullptr == pObject)
    return false;

  // Call WillBuildCustomMesh if requested
  if (bWillBuildCustomMeshCheck)
  {
    if (!WillBuildCustomMesh(vp, pObject, doc, uuidRequestingPlugIn, pAttributes))
    {
      return false;
    }
  }

  // Read sample custom render mesh user data from the object attributes
  const CSampleCustomRenderMeshUserData* pUserData = CSampleCustomRenderMeshUserData::Cast(pObject->Attributes().GetUserData(SampleCustomRenderMeshUserDataUuid));

  // If object doesn't have this type of user data then it doesn't custom render mesh
  if (pUserData == nullptr)
    return false;

Look at thickening in Displacement plug-in
Remember to put the 'Tracker' in the header.

  ON_Xform translationXform = ON_Xform::TranslationTransformation(0.0, 0.0, pUserData->Amount());

  ON_SimpleArray<const ON_Mesh*> renderMeshes;
  pObject->GetMeshes(ON::render_mesh, renderMeshes);
  for (int mi = 0; mi < renderMeshes.Count(); mi++)
  {
    const ON_Mesh* pRenderMesh = renderMeshes[mi];
    if (pRenderMesh != nullptr)
    {
      ON_Mesh* pCustomMesh = new ON_Mesh(*pRenderMesh);
      if (pCustomMesh != nullptr)
      {
        pCustomMesh->Transform(translationXform);
        crmInOut.SetUseObjectsMappingChannels(true);
        crmInOut.SetInstanceTransform(ON_Xform::IdentityTransformation);
        crmInOut.SetAutoDeleteMeshesOn();
        crmInOut.SetAutoDeleteMaterialsOn();
        crmInOut.Add(pCustomMesh, nullptr);
      }
    }
  }

  return true;
}
