/////////////////////////////////////////////////////////////////////////////
// SampleCursorPlugIn.h : main header file for the SampleCursor plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleCursorPlugIn
// See SampleCursorPlugIn.cpp for the implementation of this class
//

class CSampleCursorPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleCursorPlugIn();
  ~CSampleCursorPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  HCURSOR Cursor() const;

private:
  ON_wString m_plugin_version;
  HCURSOR m_hCursor;
};

CSampleCursorPlugIn& SampleCursorPlugIn();



