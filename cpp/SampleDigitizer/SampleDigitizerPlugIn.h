#pragma once

class CSampleDigitizerPlugIn : public CRhinoDigitizerPlugIn
{
public:
  CSampleDigitizerPlugIn();
  ~CSampleDigitizerPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  // Digitizer overrides
  bool EnableDigitizer(bool bEnable);
  ON::LengthUnitSystem UnitSystem() const;
  double PointTolerance() const;

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  HANDLE m_hThread;
  DWORD m_dwThreadID;
  bool m_bDigitizerConnected;
};

CSampleDigitizerPlugIn& SampleDigitizerPlugIn();



