#include "stdafx.h"
#include "SampleMarkerPlugIn.h"
#include "SampleMarkerUserData.h"

ON_OBJECT_IMPLEMENT(CSampleMarkerUserData, ON_UserData, "6504DAED-D3F3-4246-A1D9-902957658400");

ON_UUID CSampleMarkerUserData::Id()
{
  return CSampleMarkerUserData::m_CSampleMarkerUserData_class_rtti.Uuid();
}

ON_UUID CSampleMarkerUserData::PlugInId()
{
  return SampleMarkerPlugIn().PlugInID();
}

CSampleMarkerUserData::CSampleMarkerUserData()
{
  m_userdata_uuid = CSampleMarkerUserData::Id();

  /*
  CRITICAL:
    m_application_uuid must be assigned the uuid of the plug-in
    that will be responsible for reading and writing our user data.
    In this example, we'll use PlugIn1 as our primary plug-in.
  */
  m_application_uuid = CSampleMarkerUserData::PlugInId();

  m_userdata_copycount = 1; // enable copying

  // initialize your data members here
  m_bEnabled = false;
}

CSampleMarkerUserData::~CSampleMarkerUserData()
{
}

CSampleMarkerUserData::CSampleMarkerUserData(const CSampleMarkerUserData& src)
  : ON_UserData(src)
{
  m_userdata_uuid = CSampleMarkerUserData::Id();

  /*
  CRITICAL:
    m_application_uuid must be assigned the uuid of the plug-in
    that will be responsible for reading and writing our user data.
    In this example, we'll use PlugIn1 as our primary plug-in.
  */
  m_application_uuid = CSampleMarkerUserData::PlugInId();

  // All other ON_UserData fields are correctly initialized by 
  // the ON_UserData(src) call. In fact, if src is 
  // a CSampleMarkerUserData class, then ALL ON_UserData fields are 
  // correctly initialized by ON_UserData(src).  
  // The next lines are here to handle the rare case 
  // when a class derived from CSampleMarkerUserData is passed as src.

  // copy your data members here.
  m_bEnabled = src.m_bEnabled;
}

CSampleMarkerUserData& CSampleMarkerUserData::operator=(const CSampleMarkerUserData& src)
{
  // NEVER set m_userdata_uuid or m_application_uuid
  // in an operator=.  These values are set when the
  // class is constructed and should not be changed.
  if (this != &src)
  {
    // Use the base class to copy all appropriate 
    // ON_UserData fields.
    ON_UserData::operator=(src);

    // Explictly copy ONLY members you added to
    // the CSampleMarkerUserData class.
    m_bEnabled = src.m_bEnabled;
  }
  return *this;
}

BOOL CSampleMarkerUserData::GetDescription(ON_wString& description)
{
  description = L"Sample Marker User Data";
  return true;
}

BOOL CSampleMarkerUserData::Archive() const
{
  return TRUE;
}

BOOL CSampleMarkerUserData::Write(ON_BinaryArchive& binary_archive) const
{
  int minor_version = 0;
  bool rc = binary_archive.BeginWrite3dmChunk(TCODE_ANONYMOUS_CHUNK, 1, minor_version);
  if (!rc)
    return false;

  // Write class members like this
  for (;;)
  {
    // version 1.0 fields
    rc = binary_archive.WriteBool(m_bEnabled);
    if (!rc) break;

    // If you add data members to CSampleMarkerUserData after you 
    // have released a product, simply increment minor_version
    // by one, and write the new information.  This way you
    // can enhance your user data over several versions of
    // your product without breaking file IO.  It is CRITICAL
    // that once you write something to a customer's .3dm
    // file, you continue to write it the same way for all
    // future versions.

    // version 1.1 fields added DD MMM YYYY
    // rc = binary_archive.WriteSomethingNew( ... );
    // if (!rc) break;
    // rc = binary_archive.WriteSomethingElseNew( ... );
    // if (!rc) break;

    // version 1.2 fields added DD MMM YYYY
    // rc = binary_archive.WriteAnotherSomethingNew( ... );
    // if (!rc) break;
    // rc = binary_archive.WriteAnotherSomethingElseNew( ... );
    // if (!rc) break;

    break;
  }

  // If BeginWrite3dmChunk() returns true,
  // then EndWrite3dmChunk() must be called, 
  // even if a write operation failed.
  if (!binary_archive.EndWrite3dmChunk())
    rc = false;

  return rc;
}

BOOL CSampleMarkerUserData::Read(ON_BinaryArchive& binary_archive)
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
    rc = binary_archive.ReadBool(&m_bEnabled);
    if (!rc) break;

    // The code in the comment below demonstrates how to
    // correctly read information added in later releases
    // of your product.
    //if ( minor_version >= 1 )
    //{
    //  // version 1.1 fields added DD MMM YYYY
    //  rc = binary_archive.ReadSomethingNew( ... );
    //  if (!rc) break;
    //  rc = binary_archive.ReadSomethingElseNew( ... );
    //  if (!rc) break;
    //  
    //  if ( minor_version >= 2 )
    //  {
    //    // version 1.2 fields added DD MMM YYYY
    //    rc = binary_archive.ReadAnotherSomethingNew( ... );
    //    if (!rc) break;
    //    rc = binary_archive.ReadAnotherSomethingElseNew( ... );
    //    if (!rc) break;
    //  }
    //}

    break;
  }

  // If BeginRead3dmChunk() returns true,
  // then EndRead3dmChunk() must be called, 
  // even if a read operation failed.
  if (!binary_archive.EndRead3dmChunk())
    rc = false;

  return rc;
}

bool CSampleMarkerUserData::HasMarkerUserData(const CRhinoObject* object)
{
  if (0 == object)
    return false;

  CSampleMarkerUserData* pUserData = CSampleMarkerUserData::Cast(object->GetGeometryUserData(CSampleMarkerUserData::Id()));
  if (0 == pUserData)
    return false;

  return true;
}

bool CSampleMarkerUserData::AddMarkerUserData(const CRhinoObject* object)
{
  if (0 == object)
    return false;

  CSampleMarkerUserData* pUserData = CSampleMarkerUserData::Cast(object->GetGeometryUserData(CSampleMarkerUserData::Id()));
  if (pUserData)
    return true;

  pUserData = new CSampleMarkerUserData();
  if (0 == pUserData)
    return false;

  bool rc = const_cast<CRhinoObject*>(object)->AttachGeometryUserData(pUserData);
  if (!rc)
  {
    delete pUserData;
    return false;
  }

  return true;
}