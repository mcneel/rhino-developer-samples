
#pragma once

#include "MarmaladePlugIn.h"

class CMarmaladeRMP;

class CMarmaladeRdkPlugIn : public CRhRdkRenderPlugIn
{
public:
	CMarmaladeRdkPlugIn(void);
	virtual ~CMarmaladeRdkPlugIn(void);

	// Initialization.
	virtual bool Initialize(void);
	virtual void Uninitialize(void);

	virtual UUID PlugInId(void) const;

	void PlugInRdkVersion(int& iMajor, int& iMinor, int& iBeta) const { iMajor = RDK_MAJOR_VERSION; iMinor = RDK_MINOR_VERSION, iBeta = RDK_BETA_RELEASE; }
	CRhinoPlugIn& RhinoPlugIn(void) const { return ::MarmaladePlugIn(); }

protected:
	virtual void EnableNonModalWindows(bool bEnable) const;

	virtual bool AllowChooseContent(const CRhRdkContent& content) const;
	
	virtual void RegisterContent(IRhRdkContentFactories& factories) const;
	virtual void AbortRender(void);

	virtual CRhRdkVariant GetParameter(const wchar_t* wszParam) const;

	// Preview renderers.
	virtual HBITMAP CreatePreview(const CSize& sizeImage, eRhRdkRenderQuality quality, const IRhRdkPreviewSceneServer* pSceneServer);
	virtual HBITMAP CreatePreview(const CSize& sizeImage, const CRhRdkTexture& texture);

	virtual bool SupportsFeature(const UUID& uuidFeature) const;

	// Custom commands.
	virtual void AddCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const;
	virtual void UpdateCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const;
	virtual void AddCustomEditorMenu(IRhRdkMenu& menu, const IRhRdkContentEditor& editor) const;

	// View dock bar
	virtual void RegisterCustomPlugIns(void) const;

	// Custom render mesh providers.
	virtual void RegisterCustomRenderMeshProviders(IRhRdkCustomRenderMeshManager& crmm) const;

	// Custom content I/O plug-ins.
	virtual void RegisterContentIOPlugIns(IRhRdkContentIOPlugIns& ciop) const;

	bool IsMarmaladeCurrentRenderer(void) const;
};
