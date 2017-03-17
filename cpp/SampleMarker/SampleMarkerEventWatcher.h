#pragma once

class CSampleMarkerEventWatcher : public CRhinoEventWatcher
{
public:
  // Virtual CRhinoEventWatcher::OnEndOpenDocument() override
  void OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference);
};