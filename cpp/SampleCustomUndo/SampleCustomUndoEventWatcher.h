#pragma once

class CSampleCustomUndoEventWatcher : public CRhinoEventWatcher
{
public:
  /*
  Description:
    Used to monitor Rhino's built in Undo/Redo support.
  Parameters:
    undo_event    - [in] The type of undo event.
    undo_event_sn - [in] Runtime serial number Rhino assigned to the event.
    cmd           - [in] Command being undone. Can be NULL.
  Remarks:
   WARNING: Never modify the Rhino document in a UndoEvent() override.
     If you need to change the Rhino document or post update messages
     to controls, then your UndoEvent() override should record what happened
     in an efficient way and then make any changes that are required in a
     CRhinoIsIdle.Notify() override.
  */
  void UndoEvent(CRhinoEventWatcher::undo_event undo_event, unsigned int undo_event_sn, const CRhinoCommand* cmd);
};