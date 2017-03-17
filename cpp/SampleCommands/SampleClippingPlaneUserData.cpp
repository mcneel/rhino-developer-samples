#include "StdAfx.h"
#include "SampleClippingPlaneUserData.h"
#include "SampleCommandsPlugIn.h"

ON_OBJECT_IMPLEMENT(CSampleClippingPlaneUserData, ON_UserData, "82A75FC9-ECFC-44E1-BECB-991A15A6AD94");

ON_UUID CSampleClippingPlaneUserData::Id()
{
  return CSampleClippingPlaneUserData::m_CSampleClippingPlaneUserData_class_rtti.Uuid();
}

ON_UUID CSampleClippingPlaneUserData::PlugInId()
{
  // Return the plug-in's uuid
  return SampleCommandsPlugIn().PlugInID();
}

CSampleClippingPlaneUserData::CSampleClippingPlaneUserData()
{
  m_userdata_uuid = CSampleClippingPlaneUserData::Id();
  m_application_uuid = CSampleClippingPlaneUserData::PlugInId();
  m_userdata_copycount = 1; // enable copying
}

CSampleClippingPlaneUserData::~CSampleClippingPlaneUserData()
{
}

CSampleClippingPlaneUserData::CSampleClippingPlaneUserData(const CSampleClippingPlaneUserData& src)
  : ON_UserData(src)
{
  m_userdata_uuid = CSampleClippingPlaneUserData::Id();
  m_application_uuid = CSampleClippingPlaneUserData::PlugInId();
  m_viewport_ids = src.m_viewport_ids;
}

CSampleClippingPlaneUserData& CSampleClippingPlaneUserData::operator=(const CSampleClippingPlaneUserData& src)
{
  if (this != &src)
  {
    ON_UserData::operator=(src);
    m_viewport_ids = src.m_viewport_ids;
  }
  return *this;
}

BOOL CSampleClippingPlaneUserData::GetDescription(ON_wString& description)
{
  description = L"Clipping Plane User Data";
  return true;
}

BOOL CSampleClippingPlaneUserData::Archive() const
{
  return true;
}

BOOL CSampleClippingPlaneUserData::Write(ON_BinaryArchive& binary_archive) const
{
  int minor_version = 0;
  bool rc = binary_archive.BeginWrite3dmChunk(TCODE_ANONYMOUS_CHUNK, 1, minor_version);
  if (!rc)
    return false;

  // Write class members like this
  for (;;)
  {
    // version 1.0 fields
    rc = binary_archive.WriteArray(m_viewport_ids);
    if (!rc) break;

    // TODO...
    break;
  }

  // If BeginWrite3dmChunk() returns true,
  // then EndWrite3dmChunk() must be called, 
  // even if a write operation failed.
  if (!binary_archive.EndWrite3dmChunk())
    rc = false;

  return rc;
}

BOOL CSampleClippingPlaneUserData::Read(ON_BinaryArchive& binary_archive)
{
  int major_version = 0;
  int minor_version = 0;
  bool rc = binary_archive.BeginRead3dmChunk(TCODE_ANONYMOUS_CHUNK, &major_version, &minor_version);
  if (!rc)
    return false;

  // Read class members like this
  for (;;)
  {
    rc = (1 == major_version);
    if (!rc) break;

    // version 1.0 fields
    rc = binary_archive.ReadArray(m_viewport_ids);
    if (!rc) break;

    // TODO...
    break;
  }

  // If BeginRead3dmChunk() returns true,
  // then EndRead3dmChunk() must be called, 
  // even if a read operation failed.
  if (!binary_archive.EndRead3dmChunk())
    rc = false;

  return rc;
}

/////////////////////////////////////////////////////////////////////////////

bool CSampleClippingPlaneUserData::HasUserData(const CRhinoObject* object)
{
  // Validate the object
  if (0 == object)
    return false;

  // Validate the geometry
  const ON_Geometry* geometry = object->Geometry();
  if (0 == geometry)
    return false;

  // Get our user data
  CSampleClippingPlaneUserData* user_data = CSampleClippingPlaneUserData::Cast(geometry->GetUserData(CSampleClippingPlaneUserData::Id()));
  if (0 == user_data)
    return false;

  return true;
}

bool CSampleClippingPlaneUserData::AddUserData(CRhinoDoc& doc, const CRhinoObject* object, const ON_SimpleArray<ON_UUID>& viewport_ids)
{
  // Validate the object
  if (0 == object)
    return false;

  // Validate the geometry
  const ON_Geometry* geometry = object->Geometry();
  if (0 == geometry)
    return false;

  // Get our user data
  CSampleClippingPlaneUserData* user_data = CSampleClippingPlaneUserData::Cast(geometry->GetUserData(CSampleClippingPlaneUserData::Id()));
  if (user_data)
    return SetUserData(doc, object, viewport_ids);

  // Duplicate the object
  CRhinoObject* dupe_object = object->DuplicateRhinoObject();
  if (0 == dupe_object)
    return false;

  // Get clipping plane object
  CRhinoClippingPlaneObject* dupe_clip = CRhinoClippingPlaneObject::Cast(dupe_object);
  if (0 == dupe_clip)
  {
    delete dupe_object;
    return false;
  }

  // Remove clip viewports
  for (int i = 0; i < viewport_ids.Count(); i++)
    dupe_clip->RemoveClipViewport(viewport_ids[i]);

  // Get the duplicated geometry
  ON_Geometry* dupe_geometry = const_cast<ON_Geometry*>(dupe_object->Geometry());
  if (0 == dupe_geometry)
  {
    delete dupe_object;
    return false;
  }

  // New user data
  user_data = new CSampleClippingPlaneUserData();
  if (0 == user_data)
  {
    delete dupe_object;
    return false;
  }

  // Set member variables
  user_data->m_viewport_ids = viewport_ids;

  // Attach user data
  if (0 == dupe_geometry->AttachUserData(user_data))
  {
    delete user_data;
    delete dupe_object;
    return false;
  }

  // Replace the object
  if (!doc.ReplaceObject(CRhinoObjRef(object), dupe_object, true))
  {
    // user_data will be deleted by dupe_object
    delete dupe_object;
    return false;
  }

  return true;
}

bool CSampleClippingPlaneUserData::SetUserData(CRhinoDoc& doc, const CRhinoObject* object, const ON_SimpleArray<ON_UUID>& viewport_ids)
{
  // Validate the object
  if (0 == object)
    return false;

  // Validate the geometry
  const ON_Geometry* geometry = object->Geometry();
  if (0 == geometry)
    return false;

  // Get our user data
  CSampleClippingPlaneUserData* user_data = CSampleClippingPlaneUserData::Cast(geometry->GetUserData(CSampleClippingPlaneUserData::Id()));
  if (0 == user_data)
    return false;

  // Reset pointer
  user_data = 0;

  // Duplicate the object
  CRhinoObject* dupe_object = object->DuplicateRhinoObject();
  if (0 == dupe_object)
    return false;

  // Get clipping plane object
  CRhinoClippingPlaneObject* dupe_clip = CRhinoClippingPlaneObject::Cast(dupe_object);
  if (0 == dupe_clip)
  {
    delete dupe_object;
    return false;
  }

  // Remove clip viewports
  for (int i = 0; i < viewport_ids.Count(); i++)
    dupe_clip->RemoveClipViewport(viewport_ids[i]);

  // Get the duplicated geometry
  ON_Geometry* dupe_geometry = const_cast<ON_Geometry*>(dupe_object->Geometry());
  if (0 == dupe_geometry)
  {
    delete dupe_object;
    return false;
  }

  // Get the duplicated user data
  user_data = CSampleClippingPlaneUserData::Cast(dupe_geometry->GetUserData(CSampleClippingPlaneUserData::Id()));
  if (0 == user_data)
  {
    delete dupe_object;
    return false;
  }

  // Set member variables
  user_data->m_viewport_ids = viewport_ids;

  // Replace the object
  if (!doc.ReplaceObject(CRhinoObjRef(object), dupe_object, true))
  {
    delete dupe_object;
    return false;
  }

  return true;
}

bool CSampleClippingPlaneUserData::GetUserData(const CRhinoObject* object, ON_SimpleArray<ON_UUID>& viewport_ids)
{
  // Validate the document and the object
  if (0 == object)
    return false;

  // Validate the geometry
  const ON_Geometry* geometry = object->Geometry();
  if (0 == geometry)
    return false;

  // Query the geometry
  CSampleClippingPlaneUserData* user_data = CSampleClippingPlaneUserData::Cast(geometry->GetUserData(CSampleClippingPlaneUserData::Id()));
  if (0 == user_data)
    return false;

  viewport_ids = user_data->m_viewport_ids;

  return true;
}

bool CSampleClippingPlaneUserData::RemoveUserData(CRhinoDoc& doc, const CRhinoObject* object)
{
  // Validate the document and the object
  if (0 == object)
    return false;

  // Validate the geometry
  const ON_Geometry* geometry = object->Geometry();
  if (0 == geometry)
    return false;

  // Query the geometry
  CSampleClippingPlaneUserData* user_data = CSampleClippingPlaneUserData::Cast(geometry->GetUserData(CSampleClippingPlaneUserData::Id()));
  if (0 == user_data)
    return false;

  // Reset pointer
  user_data = 0;

  // Duplicate the object
  CRhinoObject* dupe_object = object->DuplicateRhinoObject();
  if (0 == dupe_object)
    return false;

  // Get the duplicated geometry
  ON_Geometry* dupe_geometry = const_cast<ON_Geometry*>(dupe_object->Geometry());
  if (0 == dupe_geometry)
  {
    delete dupe_object;
    return false;
  }

  // Get the duplicated user data
  user_data = CSampleClippingPlaneUserData::Cast(dupe_geometry->GetUserData(CSampleClippingPlaneUserData::Id()));
  if (0 == user_data)
  {
    delete dupe_object;
    return false;
  }

  // Get clipping plane object
  CRhinoClippingPlaneObject* dupe_clip = CRhinoClippingPlaneObject::Cast(dupe_object);
  if (0 == dupe_clip)
  {
    delete dupe_object;
    return false;
  }

  // Add clip viewports
  for (int i = 0; i < user_data->m_viewport_ids.Count(); i++)
    dupe_clip->AddClipViewport(user_data->m_viewport_ids[i]);

  // Detach and delete the user data
  dupe_geometry->DetachUserData(user_data);
  delete user_data;
  user_data = 0;

  // Replace the object
  if (!doc.ReplaceObject(CRhinoObjRef(object), dupe_object, true))
  {
    delete dupe_object;
    return false;
  }

  return true;
}

int CSampleClippingPlaneUserData::StandardViewportIds(CRhinoDoc& doc, ON_SimpleArray<UUID>& viewport_ids)
{
  ON_SimpleArray<CRhinoView*> view_list;
  const int view_count = doc.GetViewList(view_list, true, false);
  for (int i = 0; i < view_count; i++)
  {
    CRhinoView* view = view_list[i];
    if (view)
      viewport_ids.Append(view->ActiveViewportID());
  }
  return viewport_ids.Count();
}
