#include "stdafx.h"
#include "SampleCustomUndoEventWatcher.h"

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
void CSampleCustomUndoEventWatcher::UndoEvent(CRhinoEventWatcher::undo_event undo_event, unsigned int undo_event_sn, const CRhinoCommand* cmd)
{
  const wchar_t* cmd_name = cmd ? cmd->LocalCommandName() : 0;
  if (0 == cmd_name)
    cmd_name = L"<none>";

  switch (undo_event)
  {
  case CRhinoEventWatcher::begin_recording:
    RhinoApp().Print(L"> UNDO EVENT %5d: Begin recording %s changes\n", undo_event_sn, cmd_name);
    break;
  case CRhinoEventWatcher::end_recording:
    RhinoApp().Print(L"> UNDO EVENT %5d: End recording %s changes\n", undo_event_sn, cmd_name);
    break;
  case CRhinoEventWatcher::begin_undo:
    RhinoApp().Print(L"> UNDO EVENT %5d: Begin undoing %s changes\n", undo_event_sn, cmd_name);
    break;
  case CRhinoEventWatcher::end_undo:
    RhinoApp().Print(L"> UNDO EVENT %5d: End undoing %s changes\n", undo_event_sn, cmd_name);
    break;
  case CRhinoEventWatcher::begin_redo:
    RhinoApp().Print(L"> UNDO EVENT %5d: Begin redoing %s changes\n", undo_event_sn, cmd_name);
    break;
  case CRhinoEventWatcher::end_redo:
    RhinoApp().Print(L"> UNDO EVENT %5d: End redoing %s changes\n", undo_event_sn, cmd_name);
    break;
  case CRhinoEventWatcher::purge_record:
    RhinoApp().Print(L"> UNDO EVENT %5d: Purging %s changes\n", undo_event_sn, cmd_name);
    break;
  }
}