
#pragma once

#include "MarmaladePlugIn.h"

class CMarmaladeRMP : public CRhRdkCustomRenderMeshProvider
{
public:
	CMarmaladeRMP(void);
	virtual ~CMarmaladeRMP(void);

	virtual UUID PlugInId(void) const override;
	virtual UUID ProviderId(void) const override;
	virtual ON_wString Name(void) const override;
	virtual bool IsViewDependent(void) const override;
	virtual bool IsRequestingPlugInDependent(void) const override;
	virtual bool IsPreviewAndStandardSameMesh(void) const override;
	virtual CRhRdkVariant GetParameter(const CRhinoObject& object, const wchar_t* wszParamName) const override;
	virtual void SetParameter(const CRhinoObject& object, const wchar_t* wszParamName, const CRhRdkVariant& value) override;

	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                 const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                   const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
	                               IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes,
	                               bool bWillBuildCustomMeshCheck) const override;
};
