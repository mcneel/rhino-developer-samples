/////////////////////////////////////////////////////////////////////////////
// CustomObjectEventWatcher.h
//

#include "StdAfx.h"

/*
The CRhinoEventWatcher class has virtual functions that you can override in a subclass
to be notified when events occur in Rhino.

NOTE: the class is named an event WATCHER for a reason; it should only watch and never change
the objects that are in the rhino database. This will mess up things like the undo stack.
*/
class CCustomObjectEventWatcher : public CRhinoEventWatcher
{
public:
  CCustomObjectEventWatcher() {}
  ~CCustomObjectEventWatcher() {}
 
  // Called if a new object is added to the document.
  void OnAddObject(
		CRhinoDoc& doc,
		CRhinoObject& object
		);

  // Called if an object is deleted.  
  // At some later point the object can be un-deleted.
  void OnDeleteObject(
		CRhinoDoc& doc,
		CRhinoObject& object
		);

  // Called if an object is about to be replaced.
  void OnReplaceObject(
		CRhinoDoc& doc,
		CRhinoObject& old_object,
		CRhinoObject& new_object
		);

  // Called if an object is un-deleted.
  void OnUnDeleteObject(
		CRhinoDoc& doc,
		CRhinoObject& object
		);

  // Called if an object's attributes change.
  void OnModifyObjectAttributes(
    CRhinoDoc& doc, 
    CRhinoObject& object, 
    const CRhinoObjectAttributes& old_attributes
    );
};