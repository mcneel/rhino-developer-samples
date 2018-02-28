
#pragma once

class CSampleRdkMaterialPreviewRdkPlugIn : public CRhRdkRenderPlugIn
{
public:
	CSampleRdkMaterialPreviewRdkPlugIn() = default;

	virtual bool Initialize() override;
	virtual void Uninitialize() override;

	virtual UUID PlugInId() const override;

	virtual void AbortRender() override;

	virtual CRhinoPlugIn& RhinoPlugIn() const override;

	virtual PreviewRenderTypes PreviewRenderType(void) const { return PreviewRenderTypes::Progressive; }

protected:
	virtual void RegisterExtensions() const override;

	// Preview renderers
	virtual bool CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) override;
	virtual bool CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut) override;

	virtual bool SupportsFeature(const UUID& uuidFeature) const override;

private:
	bool m_bCancelPreview = false;
};
