/////////////////////////////////////////////////////////////////////////////
// SampleSerializeWatcher.h

#pragma once

class CSampleSerializeWatcher : public CRhinoEventWatcher
{
public:
  void OnCloseDocument(CRhinoDoc& doc);
};
