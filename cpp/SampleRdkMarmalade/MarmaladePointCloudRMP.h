
#pragma once

#include "MarmaladePlugIn.h"

class CMarmaladePointCloudRMP : public IRhRdkCustomRenderMeshProvider
{
public:
	CMarmaladePointCloudRMP(void);
	virtual ~CMarmaladePointCloudRMP(void);

public:
	virtual void DeleteThis(void) { delete this; }
	virtual UUID PlugInId(void) const;
	virtual UUID ProviderId(void) const;
	virtual ON_wString Name(void) const;
	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const UUID& uuidRequestingPlugIn, IRhRdkCustomRenderMeshManager::eMeshType type) const;
	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, IRhRdkCustomRenderMeshes& mesh, IRhRdkCustomRenderMeshManager::eMeshType type) const;
	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const UUID& uuidRequestingPlugIn, IRhRdkCustomRenderMeshManager::eMeshType type) const;
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) { return NULL; }
};
