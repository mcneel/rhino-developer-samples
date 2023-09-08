#include "stdafx.h"
#include "SampleCustomRenderMeshEventWatcher.h"
#include "SampleCustomRenderMeshUserData.h"

void CSampleCustomRenderMeshEventWatcher::OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object)
{
  // Use either old or new object depending on which one has valid id
  const CRhinoObject& object = new_object.Attributes().m_uuid != ON_nil_uuid ? new_object : old_object;

  // If object has sample custom render mesh user data then tell custom render mesh manager that it's CRM's need to be updated
  if (object.Attributes().GetUserData(SampleCustomRenderMeshUserDataUuid) != nullptr)
  {
    // TODO: ANDY -- this is not supposed to be called by clients so why is it here?
    // RhRdkCustomRenderMeshManager2 does not have this function.
    RhRdkCustomRenderMeshManager().OnRhinoObjectChanged(doc, &object);
  }
}
