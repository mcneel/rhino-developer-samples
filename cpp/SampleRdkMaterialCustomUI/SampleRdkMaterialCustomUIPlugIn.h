
#pragma once

#include "SampleRdkMaterialCustomUIEventWatcher.h"

class CSampleRdkMaterialCustomUIRdkPlugIn;

class CSampleRdkMaterialCustomUIPlugIn : public CRhinoRenderPlugIn
{
public:
	CSampleRdkMaterialCustomUIPlugIn();
	virtual ~CSampleRdkMaterialCustomUIPlugIn() = default;

	// Required overrides
	const wchar_t* PlugInName() const override;
	const wchar_t* PlugInVersion() const override;
	GUID PlugInID() const override;

	BOOL OnLoadPlugIn() override;
	void OnUnloadPlugIn() override;

	// Render overrides
	CRhinoCommand::result Render(const CRhinoCommandContext& context, bool bPreview) override;

private:
	ON_wString m_plugin_version;
	CSampleRdkMaterialCustomUIEventWatcher m_event_watcher;
	CSampleRdkMaterialCustomUIRdkPlugIn* m_pRdkPlugIn;

	// TODO: Add additional class information here
};

// Return a reference to the one and only CSampleRdkMaterialCustomUIPlugIn object
CSampleRdkMaterialCustomUIPlugIn& SampleRdkMaterialCustomUIPlugIn();
