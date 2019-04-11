
#pragma once

class CSampleRdkMaterialWithChildrenRdkPlugIn : public CRhRdkRenderPlugIn
{
public:
	static UUID RhinoPlugInId(void);
	static UUID RdkPlugInId(void);
	static UUID RenderEngineId(void);

	virtual bool Initialize(void) override;
	virtual void Uninitialize(void) override;
	virtual void AbortRender(void) override;
	virtual void UiContentTypes(OUT ON_SimpleArray<UUID>& aTypes) const override;
	virtual CRhinoPlugIn& RhinoPlugIn(void) const override;
	virtual void RegisterExtensions(void) const override;
	virtual bool Icon(CRhinoDib& dibOut) const override;
	virtual UUID PlugInId(void) const override { return RdkPlugInId(); }
	virtual bool CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, OUT CRhinoDib& dib) override;
	virtual bool SupportsFeature(const UUID& uuidFeature) const override;
};
