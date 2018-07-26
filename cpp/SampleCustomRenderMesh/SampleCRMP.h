#pragma once
#include <IRhRdkCustomRenderMeshManager.h>

class CSampleCRMP : public CRhRdkCustomRenderMeshProvider
{
public:
  CSampleCRMP(void);
	virtual ~CSampleCRMP(void);

public:
	virtual UUID PlugInId(void) const override;
	virtual UUID ProviderId(void) const override;
	virtual ON_wString Name(void) const override;

  virtual bool IsViewDependent(void) const override;
  virtual bool IsRequestingPlugInDependent(void) const override;
  virtual bool IsPreviewAndStandardSameMesh(void) const override;
  virtual CRhRdkVariant GetParameter(const CRhinoObject&, const wchar_t*) const override;
  virtual void SetParameter(const CRhinoObject&, const wchar_t*, const CRhRdkVariant&) override;

	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                 const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                   const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
	                               IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes,
	                               bool bWillBuildCustomMeshCheck = true) const override;
};
