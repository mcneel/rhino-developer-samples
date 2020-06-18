#include "StdAfx.h"
#include "SampleDocumentUserData.h"
#include "SampleCommandsPlugIn.h"

// Opennurbs classes that are saved in .3dm files require
// an ON_OBJECT_IMPLEMENT() call in their implmentation.
// Use Microsoft's guidgen to create the id.
ON_OBJECT_IMPLEMENT(CSampleDocumentUserData, ON_UserData, "31BDF571-8C94-4A49-8CAE-51D2AA12ECF2");

ON_UUID CSampleDocumentUserData::Id()
{
  return ON_CLASS_ID(CSampleDocumentUserData);
}

ON_UUID CSampleDocumentUserData::PlugInId()
{
  return SampleCommandsPlugIn().PlugInID();
}

CSampleDocumentUserData::CSampleDocumentUserData()
{
  m_userdata_uuid = CSampleDocumentUserData::Id();

  // CRITICAL:
  // m_application_uuid must be assigned the uuid of the plug-in
  // that will be responsible for reading and writing our user data.
  // In this example, we'll use CSampleDocumentUserData as our primary plug-in.
  m_application_uuid = CSampleDocumentUserData::PlugInId();

  // enable copying
  m_userdata_copycount = 1; 

  // initialize your data members here
  m_strings.SetCapacity(16);
}

CSampleDocumentUserData::CSampleDocumentUserData(const CSampleDocumentUserData& src) 
  : ON_UserData(src)
{
  m_userdata_uuid = CSampleDocumentUserData::Id();

  // CRITICAL:
  // m_application_uuid must be assigned the uuid of the plug-in
  // that will be responsible for reading and writing our user data.
  // In this example, we'll use CSampleDocumentUserData as our primary plug-in.
  m_application_uuid = CSampleDocumentUserData::PlugInId();

  // All other ON_UserData fields are correctly initialized by 
  // the ON_UserData(src) call. In fact, if src is 
  // a CSampleObjectUserData class, then ALL ON_UserData fields are 
  // correctly initialized by ON_UserData(src).  
  // The next lines are here to handle the rare case 
  // when a class derived from CSampleObjectUserData is passed as src.

  m_strings = src.m_strings;
}

CSampleDocumentUserData& CSampleDocumentUserData::operator=(const CSampleDocumentUserData& src)
{
  // NEVER set m_userdata_uuid or m_application_uuid
  // in an operator=.  These values are set when the
  // class is constructed and should not be changed.
  if (this != &src)
  {
    // Use the base class to copy all appropriate ON_UserData fields.
    ON_UserData::operator=(src);

    // Explictly copy ONLY members you added to the CSampleDocumentUserData class.
    m_strings = src.m_strings;
  }
  return *this;
}

bool CSampleDocumentUserData::GetDescription(ON_wString& description)
{
  description = L"SampleDocumentUserData";
  return true;
}

bool CSampleDocumentUserData::Archive() const
{
  // If false is returned, nothing will be saved in 3dm archives.
  return true;
}

bool CSampleDocumentUserData::Write(ON_BinaryArchive& binary_archive) const
{
  bool rc = binary_archive.BeginWrite3dmChunk(TCODE_ANONYMOUS_CHUNK, m_major_version, m_minor_version);
  if (!rc)
    return false;

  // Write class members like this
  for (;;)
  {
    // version 1.0 fields
    rc = binary_archive.WriteArray(m_strings);
    if (!rc) break;

    // If you add data members to CSampleDocumentUserData after you 
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

  // If BeginWrite3dmChunk() returns true then EndWrite3dmChunk() must be called, 
  // even if a write operation failed.
  if (!binary_archive.EndWrite3dmChunk())
    rc = false;

  return rc;
}

bool CSampleDocumentUserData::Read(ON_BinaryArchive& binary_archive)
{
  int major_version = 0;
  int minor_version = 0;
  bool rc = binary_archive.BeginRead3dmChunk(TCODE_ANONYMOUS_CHUNK, &major_version, &minor_version);
  if (!rc)
    return false;

  // Read class members like this
  for (;;)
  {
    rc = (m_major_version == major_version);
    if (!rc) break;

    // version 1.0 fields
    rc = binary_archive.ReadArray(m_strings);
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

  // If BeginRead3dmChunk() returns true then EndRead3dmChunk() must be called, 
  // even if a read operation failed.
  if (!binary_archive.EndRead3dmChunk())
    rc = false;

  return rc;
}

int CSampleDocumentUserData::Count() const
{
  return m_strings.Count();
}

ON_wString CSampleDocumentUserData::GetAt(int index) const
{
  ON_wString str;
  if (index >= 0 && index < m_strings.Count())
    str = m_strings[index];
  return str;
}

int CSampleDocumentUserData::Find(const wchar_t* str) const
{
  if (nullptr == str || 0 == str[0])
    return -1;

  for (int i = 0; i < m_strings.Count(); i++)
  {
    const ON_wString& item = m_strings[i];
    if (0 == item.CompareOrdinal(str, true))
      return i;
  }

  return -1;
}

int CSampleDocumentUserData::Add(const wchar_t* str)
{
  if (nullptr == str || 0 == str[0])
    return -1;

  int index = Find(str);
  if (index < 0)
  {
    m_strings.Append(str);
    return Count() - 1;
  }

  return -1;
}

bool CSampleDocumentUserData::Delete(int index)
{
  if (index > 0 && index < Count())
  {
    m_strings.Remove(index);
    return true;
  }

  return false;
}

bool CSampleDocumentUserData::Delete(const wchar_t* str)
{
  if (nullptr == str || 0 == str[0])
    return false;

  int index = Find(str);
  if (index > 0 && index < Count())
  {
    m_strings.Remove(index);
    return true;
  }

  return false;
}

void CSampleDocumentUserData::Clear()
{
  m_strings.Destroy();
}
