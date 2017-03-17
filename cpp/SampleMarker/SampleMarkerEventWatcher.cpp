#include "stdafx.h"
#include "SampleMarkerEventWatcher.h"
#include "SampleMarkerObject.h"
#include "SampleMarkerUserData.h"

// Promotes a CRhinoPointObject to a CSampleMarkerObject
static void PromoteRhinoPointObjectToMarkerObject(const CRhinoPointObject* point_obj)
{
  static void* CSampleMarkerObject__vtable_ptr = 0;
  static void* CRhinoPointObject__vtable_ptr = 0;

  if (0 == point_obj)
    return;

  if (0 == CSampleMarkerObject__vtable_ptr)
  {
    // The first time we call this function, the statics are initialized
    CSampleMarkerObject a;
    CSampleMarkerObject__vtable_ptr = ON_ClassVtable((void*)(&a));
    CRhinoPointObject b;
    CRhinoPointObject__vtable_ptr = ON_ClassVtable((void*)(&b));
  }

  if (CRhinoPointObject__vtable_ptr == *((void**)point_obj))
  {
    // Change from CRhinoPointObject vtable to CSampleMarkerObject vtable
    *((void**)point_obj) = CSampleMarkerObject__vtable_ptr;
  }
}

void CSampleMarkerEventWatcher::OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference)
{
  UNREFERENCED_PARAMETER(filename);
  UNREFERENCED_PARAMETER(bMerge);
  UNREFERENCED_PARAMETER(bReference);

  CRhinoObjectIterator it(doc, CRhinoObjectIterator::undeleted_objects, CRhinoObjectIterator::active_objects);
  it.SetObjectFilter(ON::point_object);

  const CRhinoObject* obj = 0;
  for (obj = it.First(); obj; obj = it.Next())
  {
    const CRhinoPointObject* point_obj = CRhinoPointObject::Cast(obj);
    if (point_obj && CSampleMarkerUserData::HasMarkerUserData(point_obj))
    {
      PromoteRhinoPointObjectToMarkerObject(point_obj);
    }
  }
}
