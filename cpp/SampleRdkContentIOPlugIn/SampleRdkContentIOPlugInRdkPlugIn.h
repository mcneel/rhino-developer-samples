
#pragma once

class CSampleRdkContentIOPlugInRdkPlugIn : public CRhRdkRenderPlugIn
{
public:
	static UUID RhinoPlugInId(void);
	static UUID RdkPlugInId(void);
	static UUID RenderEngineId(void);

	virtual bool Initialize(void) override;
	virtual void Uninitialize(void) override;

	virtual void AbortRender(void) override;

	virtual void UiContentTypes(OUT ON_SimpleArray<UUID>& aTypes) const override;

	//virtual void RenderSettingsSections(OUT ON_SimpleArray<UUID>& aSections) const override;

	CRhinoPlugIn& RhinoPlugIn(void) const override;

	virtual void RegisterExtensions(void) const override;

	//virtual bool AllowChooseContent(const CRhRdkContent& content) const override;

	virtual bool Icon(CRhinoDib& dibOut) const override;
	virtual UUID PlugInId(void) const override { return RdkPlugInId(); }

	//virtual bool DisableAutoUpdatePreview(void) const override;

	//virtual bool PreferBasicContent(void) const override { return false; }

	//virtual const IRhRdkDecalSupport* DecalSupport(void) const override;

	virtual bool CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, OUT CRhinoDib& dib) override;
	//virtual bool CreatePreview(const CSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut) override;

	virtual bool SupportsFeature(const UUID& uuidFeature) const override;

	//virtual bool CreateQuickPreview(const CSize& size, const IRhRdkPreviewSceneServer& ss, QuickPreviewReason reason, CRhinoDib& dibOut) override;

	//virtual void AddCustomRenderSettingsSections(RhRdkModality m, ON_SimpleArray<IRhinoUiSection*>& aSections) const override;
	//virtual void AddCustomSunSections(RhRdkModality m, ON_SimpleArray<IRhinoUiSection*>& aSections) const override;

	//virtual void GetCustomRenderSaveFileTypes(ON_ClassArray<CRhRdkFileType>& aFileType) const override;
	//virtual bool SaveCustomRenderFile(const wchar_t* wszFilename, CRhRdkFileType& type, const UUID& uuidRenderSession, bool bSaveAlpha) const override;
};
