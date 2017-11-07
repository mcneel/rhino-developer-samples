#pragma once

class CSampleRenderer;

// CSampleRealtimeRendererPlugIn plug-in
class CSampleRealtimeRendererPlugIn : public CRhinoUtilityPlugIn
{
public:
	CSampleRealtimeRendererPlugIn();
	~CSampleRealtimeRendererPlugIn();

	// Required overrides
	const wchar_t* PlugInName() const;
	const wchar_t* PlugInVersion() const;
	GUID PlugInID() const;
	BOOL OnLoadPlugIn();
	void OnUnloadPlugIn();

	// Load time override
	CRhinoPlugIn::plugin_load_time PlugInLoadTime();

private:
	ON_wString m_plugin_version;
};

CSampleRealtimeRendererPlugIn& SampleRealtimeRendererPlugIn();
