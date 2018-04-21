
#pragma once

#include "MarmaladePlugIn.h"

class CMarmaladePointCloudRMP : public CRhRdkCustomRenderMeshProvider
{
public:
	CMarmaladePointCloudRMP(void);
	virtual ~CMarmaladePointCloudRMP(void);

public:
	virtual UUID PlugInId(void) const override;
	virtual UUID ProviderId(void) const override;
	virtual ON_wString Name(void) const override;

	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                 const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                   const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
	                               IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes,
	                               bool bWillBuildCustomMeshCheck = true) const override;
};
