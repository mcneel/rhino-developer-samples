#pragma once

class CSampleCommandsEventWatcher : public CRhinoEventWatcher
{
public:
  CSampleCommandsEventWatcher() {}
  ~CSampleCommandsEventWatcher() {}

  // WARNING: 
  // Never modify the Rhino document in an OnCloseDocument() override.
  // If you need to change the Rhino document or post update messages
  // to controls, then your OnCloseDocument() override
  // should record what happened in an efficient way and then make
  // any changes that are required in a CRhinoIsIdle.Notify() override.
  void OnCloseDocument(CRhinoDoc& doc);
};
