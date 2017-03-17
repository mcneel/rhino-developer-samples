/////////////////////////////////////////////////////////////////////////////
// SampleDigitizerPlugIn.h : main header file for the SampleDigitizer plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleDigitizerPlugIn
// See SampleDigitizerPlugIn.cpp for the implementation of this class
//

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

  // Digitizer overrides
  bool EnableDigitizer( bool bEnable );
  ON::unit_system UnitSystem() const;
  double PointTolerance() const;

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
	HANDLE m_hThread;
	DWORD m_dwThreadID;
	bool m_bDigitizerConnected;
};

CSampleDigitizerPlugIn& SampleDigitizerPlugIn();



