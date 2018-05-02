
#pragma once

#include "MarmaladePlugIn.h"

class CMarmaladeRMP;

class CMarmaladeRdkPlugIn : public CRhRdkRenderPlugIn
{
public:
	CMarmaladeRdkPlugIn(void);
	virtual ~CMarmaladeRdkPlugIn(void);

	// Initialization.
	virtual bool Initialize(void) override;
	virtual void Uninitialize(void) override;

	virtual UUID PlugInId(void) const override;

	/// deprecated void PlugInRdkVersion(int& iMajor, int& iMinor, int& iBeta) const { iMajor = RDK_MAJOR_VERSION; iMinor = RDK_MINOR_VERSION, iBeta = RDK_BETA_RELEASE; }
	CRhinoPlugIn& RhinoPlugIn(void) const override { return ::MarmaladePlugIn(); }

protected:
	virtual bool AllowChooseContent(const CRhRdkContent& content) const override;

	virtual void RegisterExtensions(void) const override;

	virtual void AbortRender(void) override;

	virtual CRhRdkVariant GetParameter(const wchar_t* wszParam) const override;

	// Preview renderers.
	virtual bool CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality,
	                           const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) override;

	virtual bool CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut) override;

	virtual bool SupportsFeature(const UUID& uuidFeature) const override;

	virtual void AddCustomRenderSettingsSections(RhRdkUiModalities m, ON_SimpleArray<IRhinoUiSection*>& aSections) const override;

	// Custom commands.
	virtual void AddCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const override;
	virtual void UpdateCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const override;
	virtual void AddCustomEditorMenu(IRhRdkMenu& menu, const IRhRdkContentEditor& editor) const override;

	bool IsMarmaladeCurrentRenderer(void) const;
};
