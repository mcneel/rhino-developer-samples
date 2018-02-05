// SampleRdkMaterialCustomUIRdkPlugIn.h
//

#pragma once

// CSampleRdkMaterialCustomUIRdkRenderPlugIn
// See SampleRdkMaterialCustomUIRdkPlugIn.cpp for the implementation of this class.
//

class CSampleRdkMaterialCustomUIRdkPlugIn : public CRhRdkRenderPlugIn
{
public:
	CSampleRdkMaterialCustomUIRdkPlugIn() = default;

	bool Initialize() override;
	void Uninitialize() override;

	 UUID PlugInId() const override;

	void AbortRender() override;

	CRhinoPlugIn& RhinoPlugIn() const override;

protected:
  void RegisterExtensions() const override;

	// Preview renderers
	bool CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) override;
	bool CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut) override;

	bool SupportsFeature(const UUID& uuidFeature) const override;
};
