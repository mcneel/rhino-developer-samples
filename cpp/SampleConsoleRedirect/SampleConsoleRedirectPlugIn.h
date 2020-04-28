#pragma once

class CSampleConsoleRedirectPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleConsoleRedirectPlugIn();
  ~CSampleConsoleRedirectPlugIn() = default;

  // Required overrides
  const wchar_t* PlugInName() const override;
  const wchar_t* PlugInVersion() const override;
  GUID PlugInID() const override;
  
  // Additional overrides
  BOOL OnLoadPlugIn() override;
  void OnUnloadPlugIn() override;

private:
  bool CreateOutputConsole();

private:
  ON_wString m_plugin_version;
};

// Return a reference to the one and only CSampleConsoleRedirectPlugIn object
CSampleConsoleRedirectPlugIn& SampleConsoleRedirectPlugIn();
