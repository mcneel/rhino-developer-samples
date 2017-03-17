/////////////////////////////////////////////////////////////////////////////
// BlockWalkPlugIn.h

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CBlockWalkPlugIn
// See BlockWalkPlugIn.cpp for the implementation of this class

class CBlockWalkPlugIn : public CRhinoUtilityPlugIn
{
public:
  CBlockWalkPlugIn();
  ~CBlockWalkPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

CBlockWalkPlugIn& BlockWalkPlugIn();



