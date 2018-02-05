
#pragma once

class CSampleRdkAsyncRendererRdkPlugIn : public CRhRdkRenderPlugIn
{
public:
	CSampleRdkAsyncRendererRdkPlugIn() = default;

	virtual bool Initialize(void) override;
	virtual void Uninitialize(void) override;
	virtual UUID PlugInId(void) const override;
	virtual void AbortRender(void) override;
	virtual CRhinoPlugIn& RhinoPlugIn(void) const override;

protected:
	virtual void RegisterExtensions(void) const override;

	// Preview renderers
	virtual bool CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) override;
	virtual bool CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut) override;

	virtual bool SupportsFeature(const UUID& uuidFeature) const override;
};
