/////////////////////////////////////////////////////////////////////////////
// SampleSerializePlugIn.h
//

#pragma once

#include "SampleSerializeData.h"
#include "SampleSerializeWatcher.h"

class CSampleSerializePlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleSerializePlugIn();
  ~CSampleSerializePlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Document user data overrides
  BOOL CallWriteDocument(const CRhinoFileWriteOptions& options);
  BOOL WriteDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileWriteOptions& options);
  BOOL ReadDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileReadOptions& options);

  CSampleSerializeData& Data();

private:
  ON_wString m_plugin_version;
  CSampleSerializeData m_plugin_data;
  CSampleSerializeWatcher m_plugin_watcher;
};

CSampleSerializePlugIn& SampleSerializePlugIn();
