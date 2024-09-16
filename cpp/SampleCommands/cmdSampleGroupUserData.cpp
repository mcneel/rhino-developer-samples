#include "stdafx.h"
#include "SampleCommandsPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN CSampleGroupUserData
//

class CSampleGroupUserData : public ON_UserData
{
  ON_OBJECT_DECLARE(CSampleGroupUserData);

public:
  static ON_UUID Id();

  CSampleGroupUserData();
  ~CSampleGroupUserData() = default;
  CSampleGroupUserData(const CSampleGroupUserData& src);
  CSampleGroupUserData& operator=(const CSampleGroupUserData& src);

  bool GetDescription(ON_wString& description) override;
  bool Archive() const override;
  bool Write(ON_BinaryArchive& binary_archive) const override;
  bool Read(ON_BinaryArchive& binary_archive) override;

public:
  // Helper functions
  static bool HasUserData(const CRhinoGroup* group);
  static bool SetUserData(CRhinoDoc& doc, const CRhinoGroup* group);
  static bool GetUserData(const CRhinoGroup* group, ON_wString& string);
  static bool RemoveUserData(CRhinoDoc& doc, const CRhinoGroup* pRhinoGroup);

public:
  ON_wString m_string;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

ON_OBJECT_IMPLEMENT(CSampleGroupUserData, ON_UserData, "820A89D9-8AA5-48F2-8DAE-1A440F3A93CB");

ON_UUID CSampleGroupUserData::Id()
{
  return ON_CLASS_ID(CSampleGroupUserData);
}

CSampleGroupUserData::CSampleGroupUserData()
{
  m_userdata_uuid = CSampleGroupUserData::Id();
  m_application_uuid = SampleCommandsPlugIn().PlugInID();
  m_userdata_copycount = 1; // enable copying
  m_string = L"Hello Rhino Group!";
}

CSampleGroupUserData::CSampleGroupUserData(const CSampleGroupUserData& src)
  : ON_UserData(src)
{
  m_userdata_uuid = CSampleGroupUserData::Id();
  m_application_uuid = SampleCommandsPlugIn().PlugInID();
  m_string = src.m_string;
}

CSampleGroupUserData& CSampleGroupUserData::operator=(const CSampleGroupUserData& src)
{
  if (this != &src)
  {
    ON_UserData::operator=(src);
    m_string = src.m_string;
  }
  return *this;
}

bool CSampleGroupUserData::GetDescription(ON_wString& description)
{
  description = L"Sample Group User Data";
  return true;
}

bool CSampleGroupUserData::Archive() const
{
  return true;
}

bool CSampleGroupUserData::Write(ON_BinaryArchive& binary_archive) const
{
  int minor_version = 0;
  bool rc = binary_archive.BeginWrite3dmChunk(TCODE_ANONYMOUS_CHUNK, 1, minor_version);
  if (!rc)
    return false;

  for (;;)
  {
    // version 1.0 fields
    rc = binary_archive.WriteString(m_string);
    if (!rc) break;

    break;
  }

  if (!binary_archive.EndWrite3dmChunk())
    rc = false;

  return rc;
}

bool CSampleGroupUserData::Read(ON_BinaryArchive& binary_archive)
{
  int major_version = 0;
  int minor_version = 0;
  bool rc = binary_archive.BeginRead3dmChunk(TCODE_ANONYMOUS_CHUNK, &major_version, &minor_version);
  if (!rc)
    return false;

  for (;;)
  {
    rc = (1 == major_version);
    if (!rc) break;

    // version 1.0 fields
    rc = binary_archive.ReadString(m_string);
    if (!rc) break;

    break;
  }

  if (!binary_archive.EndRead3dmChunk())
    rc = false;

  return rc;
}

bool CSampleGroupUserData::HasUserData(const CRhinoGroup* group)
{
  if (nullptr != group)
  {
    CSampleGroupUserData* data = CSampleGroupUserData::Cast(group->GetUserData(CSampleGroupUserData::Id()));
    return (nullptr != data);
  }
  return false;
}

bool CSampleGroupUserData::SetUserData(CRhinoDoc& doc, const CRhinoGroup* group)
{
  if (nullptr == group)
    return false;

  if (CSampleGroupUserData::HasUserData(group))
    return true;

  ON_Group new_group(*group);

  CSampleGroupUserData* data = new CSampleGroupUserData();
  if (!new_group.AttachUserData(data))
  {
    delete data;
    return false;
  }

  return doc.m_group_table.ModifyGroup(new_group, group->Index(), false);
}

bool CSampleGroupUserData::GetUserData(const CRhinoGroup* group, ON_wString& string)
{
  if (nullptr != group)
  {
    CSampleGroupUserData* data = CSampleGroupUserData::Cast(group->GetUserData(CSampleGroupUserData::Id()));
    if (nullptr != data)
    {
      string = data->m_string;
      return true;
    }
  }
  return false;
}

bool CSampleGroupUserData::RemoveUserData(CRhinoDoc& doc, const CRhinoGroup* group)
{
  if (nullptr == group)
    return false;

  if (!CSampleGroupUserData::HasUserData(group))
    return false;

  ON_Group new_group(*group);

  CSampleGroupUserData* data = CSampleGroupUserData::Cast(new_group.GetUserData(CSampleGroupUserData::Id()));
  if (nullptr != data)
  {
    new_group.DetachUserData(data);
    delete data;
    data = nullptr;
    return doc.m_group_table.ModifyGroup(new_group, group->Index(), false);
  }

  return false;
}

//
// END CSampleGroupUserData
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSetGroupUserData command
//

#pragma region SampleSetGroupUserData command

class CCommandSampleSetGroupUserData : public CRhinoCommand
{
public:
  CCommandSampleSetGroupUserData() = default;
  UUID CommandUUID() override
  {
    // {9A8AE3FD-6413-4397-8303-EAD11954232E}
    static const GUID SampleSetGroupUserDataCommand_UUID =
    { 0x9a8ae3fd, 0x6413, 0x4397, { 0x83, 0x3, 0xea, 0xd1, 0x19, 0x54, 0x23, 0x2e } };
    return SampleSetGroupUserDataCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSetGroupUserData"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSetGroupUserData object
static class CCommandSampleSetGroupUserData theSampleSetGroupUserDataCommand;

CRhinoCommand::result CCommandSampleSetGroupUserData::RunCommand(const CRhinoCommandContext& context)
{
  const CRhinoGroup* group = context.m_doc.m_group_table[0];
  if (nullptr == group)
  {
    RhinoApp().Print(L"The document's group table is empty.\n");
    return CRhinoCommand::nothing;
  }

  bool rc = CSampleGroupUserData::SetUserData(context.m_doc, group);
  if (rc)
    RhinoApp().Print(L"Group user data added successfully.\n");
  else
    RhinoApp().Print(L"Unable to add group user data.\n");

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSetGroupUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetGroupUserData command
//

#pragma region SampleGetGroupUserData command

class CCommandSampleGetGroupUserData : public CRhinoCommand
{
public:
  CCommandSampleGetGroupUserData() = default;
  UUID CommandUUID() override
  {
    // {15C1B793-F9EB-4B9F-8591-CC467A360ED0}
    static const GUID SampleGetGroupUserDataCommand_UUID =
    { 0x15c1b793, 0xf9eb, 0x4b9f, { 0x85, 0x91, 0xcc, 0x46, 0x7a, 0x36, 0xe, 0xd0 } };
    return SampleGetGroupUserDataCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleGetGroupUserData"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleGetGroupUserData object
static class CCommandSampleGetGroupUserData theSampleGetGroupUserDataCommand;

CRhinoCommand::result CCommandSampleGetGroupUserData::RunCommand(const CRhinoCommandContext& context)
{
  const CRhinoGroup* group = context.m_doc.m_group_table[0];
  if (nullptr == group)
  {
    RhinoApp().Print(L"The document's group table is empty.\n");
    return CRhinoCommand::nothing;
  }

  ON_wString string;
  bool rc = CSampleGroupUserData::GetUserData(group, string);
  if (rc)
    RhinoApp().Print(L"Group has user data: %ls\n", static_cast<const wchar_t*>(string));
  else
    RhinoApp().Print(L"No group user data found.\n");

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGetGroupUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
