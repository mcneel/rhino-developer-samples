/////////////////////////////////////////////////////////////////////////////
// CustomObjectEventWatcher.cpp
//

#include "StdAfx.h"
#include "CustomObjectEventWatcher.h"

void CCustomObjectEventWatcher::OnAddObject(
	CRhinoDoc& doc,
	CRhinoObject& object
	)
{
	RhinoApp().Print( L"Event watcher caught OnAddObject\n" );
	// This is a good place perform any logic that you see necessary in your plug-in
}

void CCustomObjectEventWatcher::OnDeleteObject(
	CRhinoDoc& doc,
	CRhinoObject& object
	)
{
	RhinoApp().Print( L"Event watcher caught OnDeleteObject\n" );
	// This is a good place perform any logic that you see necessary in your plug-in
}

void CCustomObjectEventWatcher::OnReplaceObject(
	CRhinoDoc& doc,
	CRhinoObject& old_object,
	CRhinoObject& new_object
	)
{
	RhinoApp().Print( L"Event watcher caught OnReplaceObject\n" );
	// This is a good place perform any logic that you see necessary in your plug-in
}

void CCustomObjectEventWatcher::OnUnDeleteObject(
	CRhinoDoc& doc,
	CRhinoObject& object
	)
{
	RhinoApp().Print( L"Event watcher caught OnUnDeleteObject\n" );
	// This is a good place perform any logic that you see necessary in your plug-in
}

void CCustomObjectEventWatcher::OnModifyObjectAttributes(
    CRhinoDoc& doc, 
    CRhinoObject& object, 
    const CRhinoObjectAttributes& old_attributes
    )
{
	RhinoApp().Print( L"Event watcher caught OnModifyObjectAttributes\n" );
	// This is a good place perform any logic that you see necessary in your plug-in
}
