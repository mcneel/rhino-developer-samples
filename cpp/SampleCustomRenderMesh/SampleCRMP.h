
#pragma once

#include <IRhRdkCustomRenderMeshManager.h>

using namespace RhRdk;
using namespace CustomRenderMeshes;

class CSampleCRMP : public RhRdk::CustomRenderMeshes::IRenderMeshProvider, public CRhRdkExtension
{
public:
	CSampleCRMP(void);
	virtual ~CSampleCRMP(void);

public:
	virtual UUID PlugInId(void) const override;
	virtual UUID ProviderId(void) const override;
	virtual UUID InterfaceId(void) const override;
	virtual ON_wString Name(void) const override;

	virtual CRhRdkVariant GetParameter(const CRhinoDoc& doc, const ObjectId& object, const wchar_t* wszParamName) const override;
	virtual void SetParameter(const CRhinoDoc& doc, const ObjectId& object, const wchar_t* wszParamName, const CRhRdkVariant& value) override;

	virtual std::vector<ObjectId> NonObjectIds(void) const override;

	virtual bool HasCustomRenderMeshes(
	             ON::mesh_type mt,
	             const ON_Viewport& vp,
	             const CRhinoDoc& doc,
	             const ObjectId& objectId,
	             ON__UINT32& flags,
	             const CRhinoPlugIn* requestingPlugIn,
	             const CDisplayPipelineAttributes* pAttributes) const override;

	virtual std::shared_ptr<const IRenderMeshes> RenderMeshes(
	        ON::mesh_type mt, 
	        const ON_Viewport& vp,
	        const CRhinoDoc& doc,
	        const ObjectId& objectId,
	        const CRhRdkObjectAncestry& ancestry_only_for_by_parent_attributes,
	        ON__UINT32& flags,
	        std::shared_ptr<const IRenderMeshes> crmIn,
	        const CRhinoPlugIn* requestingPlugIn,
	        const CDisplayPipelineAttributes* pAttributes) const override;

	virtual std::unique_ptr<IProgress> Progress(const CRhinoDoc&, const std::vector<ObjectId>* pOptionalObjects) const override;

	virtual void* EVF(const wchar_t*, void*) override;

private:
	mutable IRenderMeshes::ProviderTracking m_tracking;
};
