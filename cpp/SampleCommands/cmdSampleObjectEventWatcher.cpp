#include "StdAfx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectEventWatcher command
//

/*
NOTE: the class is named an event WATCHER for a reason; it should only watch and never change
the objects that are in the rhino database.This will mess up things like the undo stack.
*/
class CSampleObjectEventWatcher : public CRhinoEventWatcher
{
public:
  CSampleObjectEventWatcher() = default;
  ~CSampleObjectEventWatcher() = default;

  // Called if a new object is added to the document.
  virtual void OnAddObject(CRhinoDoc& doc, CRhinoObject& object) override;

  // Called if an object is deleted.  
  // At some later point the object can be un-deleted.
  virtual void OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;

  // Called if an object is about to be replaced.
  //
  // If both CRhinoDoc::UndoActive() or CRhinoDoc::RedoActive() are false,
  // then immediatedly after OnReplaceObject(doc, old_object, new_object)
  // is called there will be a call to OnDeleteObject(doc, old_object)
  // and then a call to OnAddObject(doc, new_object).
  //
  //  If either CRhinoDoc::UndoActive() and CRhinoDoc::RedoActive() are
  //  true, then immediatedly after OnReplaceObject(doc, old_object, new_object)
  //  is called there will be a call to OnDeleteObject(doc, old_object)
  //  and then a call to OnUnDeleteObject(doc, new_object).
  virtual void OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object) override;

  // Called if an object is un-deleted.
  virtual void OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;

  // Called if an object's attributes change.
  virtual void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes) override;
};

void CSampleObjectEventWatcher::OnAddObject(CRhinoDoc& doc, CRhinoObject& object)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(object);
  RhinoApp().Print(L"Event watcher caught OnAddObject\n");
  // This is a good place perform any logic that you see necessary in your plug-in
}

void CSampleObjectEventWatcher::OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(object);
  RhinoApp().Print(L"Event watcher caught OnDeleteObject\n");
  // This is a good place perform any logic that you see necessary in your plug-in
}

void CSampleObjectEventWatcher::OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(old_object);
  UNREFERENCED_PARAMETER(new_object);
  RhinoApp().Print(L"Event watcher caught OnReplaceObject\n");
  // This is a good place perform any logic that you see necessary in your plug-in
}

void CSampleObjectEventWatcher::OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(object);
  RhinoApp().Print(L"Event watcher caught OnUnDeleteObject\n");
  // This is a good place perform any logic that you see necessary in your plug-in
}

void CSampleObjectEventWatcher::OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(object);
  UNREFERENCED_PARAMETER(old_attributes);
  RhinoApp().Print(L"Event watcher caught OnModifyObjectAttributes\n");
  // This is a good place perform any logic that you see necessary in your plug-in
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleObjectEventWatcher : public CRhinoCommand
{
public:
  CCommandSampleObjectEventWatcher();
  ~CCommandSampleObjectEventWatcher() {}
  UUID CommandUUID()
  {
    // {910C6743-4D07-4C12-9B01-34D23A64A510}
    static const GUID SampleObjectEventWatcherCommand_UUID =
    { 0x910C6743, 0x4D07, 0x4C12, { 0x9B, 0x1, 0x34, 0xD2, 0x3A, 0x64, 0xA5, 0x10 } };
    return SampleObjectEventWatcherCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleObjectEventWatcher"; }
  const wchar_t* LocalCommandName() const { return L"SampleObjectEventWatcher"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  // Declare event watcher as member variable
  CSampleObjectEventWatcher m_watcher;
  bool m_bRegistered;
};

// The one and only CCommandSampleObjectEventWatcher object.  
static class CCommandSampleObjectEventWatcher theSampleObjectEventWatcherCommand;

CCommandSampleObjectEventWatcher::CCommandSampleObjectEventWatcher()
{
  m_bRegistered = false;
}

CRhinoCommand::result CCommandSampleObjectEventWatcher::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  // Register the event watcher
  if (IDYES == RhinoMessageBox(L"Watch object events?", L"Object Watcher", MB_YESNO | MB_ICONQUESTION))
  {
    if (!m_bRegistered)
    {
      m_watcher.Register();
      m_bRegistered = true;
    }
    // Enable the event watcher
    m_watcher.Enable(true);
  }
  else
  {
    // Disable the event watcher
    m_watcher.Enable(false);
  }

  return CRhinoCommand::success;
}

//
// END SampleObjectEventWatcher command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
