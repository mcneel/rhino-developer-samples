#include "stdafx.h"

class CSampleBloat : public ON_UserData
{
  ON_OBJECT_DECLARE(CSampleBloat);

public:
  CSampleBloat();

  // virtual
  bool Write(ON_BinaryArchive& binary_archive) const;

  // virtual
  bool Read(ON_BinaryArchive& binary_archive);

  // virtual 
  bool GetDescription(ON_wString& description);

  // virtual 
  bool Archive() const;

  // number of extra bytes to read/write as user data
  size_t m_sizeof_bloat;

  static ON__UINT64 AddBloat(const ON_Object*, size_t);
  static bool m_bEnableBloat;
};

ON_OBJECT_IMPLEMENT(CSampleBloat, ON_UserData, "3BA3D3D2-A674-4B29-84E4-CB06660EF590");

bool CSampleBloat::m_bEnableBloat = true;

ON__UINT64 CSampleBloat::AddBloat(const ON_Object* obj, size_t mb)
{
  if (0 == obj)
    return 0;

  CSampleBloat* bloat = CSampleBloat::Cast(obj->GetUserData(ON_CLASS_ID(CSampleBloat)));

  if (bloat)
  {
    if (mb <= 0)
    {
      if (const_cast<ON_Object*>(obj)->DetachUserData(bloat))
        delete bloat;
      bloat = 0;
    }
  }
  else if (mb > 0)
  {
    bloat = new CSampleBloat();
    if (!const_cast<ON_Object*>(obj)->AttachUserData(bloat))
    {
      delete bloat;
      bloat = 0;
      return 0;
    }
  }
  if (bloat)
    bloat->m_sizeof_bloat = mb * 1024 * 1024;

  return bloat ? bloat->m_sizeof_bloat : 0;
}


CSampleBloat::CSampleBloat()
  : m_sizeof_bloat(1024 * 1024 * 256)
{
  m_userdata_uuid = m_userdata_uuid = ON_CLASS_ID(CSampleBloat);
  m_application_uuid = ON_rhino5_id;
  m_userdata_copycount = CSampleBloat::m_bEnableBloat ? 1 : 0;
}

// virtual
bool CSampleBloat::Write(
  ON_BinaryArchive& binary_archive
) const
{
  bool rc = binary_archive.BeginWrite3dmChunk(TCODE_ANONYMOUS_CHUNK, 1, 0);
  if (!rc)
    return false;

  for (;;)
  {
    size_t sz = m_sizeof_bloat;
    rc = binary_archive.WriteBigSize(sz);
    if (!rc)
      break;

    // put bloat in a TCODE_USER chunk with no CRC
    rc = binary_archive.BeginWrite3dmBigChunk(TCODE_USER, 0);
    if (!rc)
      break;
    {
      char buffer[2048];
      const size_t sizeof_buffer = sizeof(buffer);
      memset(buffer, 0, sizeof_buffer);
      while (rc && sz > 0)
      {
        const size_t count = (sz > sizeof_buffer) ? sizeof_buffer : sz;
        rc = binary_archive.WriteByte(count, buffer);
        sz -= count;
      }
    }
    if (!binary_archive.EndWrite3dmChunk())
      rc = false;
    if (!rc)
      break;

    break;
  }

  if (!binary_archive.EndWrite3dmChunk())
    rc = false;

  return rc;
}

// virtual
bool CSampleBloat::Read(
  ON_BinaryArchive& binary_archive
)
{
  m_sizeof_bloat = 0;

  int major_version = 0;
  int minor_version = 0;
  bool rc = binary_archive.BeginRead3dmChunk(TCODE_ANONYMOUS_CHUNK, &major_version, &minor_version);
  if (!rc)
    return false;

  for (;;)
  {
    rc = (1 == major_version);
    if (!rc)
      break;

    size_t sz = 0;
    rc = binary_archive.ReadBigSize(&sz);
    if (!rc)
      break;

    // fseek over the bloat but verfy the chunk tcode and length
    ON__UINT32 chunk_tcode = 0;
    ON__INT64 chunk_length = 0;
    rc = binary_archive.BeginRead3dmBigChunk(&chunk_tcode, &chunk_length);
    if (rc && !binary_archive.EndRead3dmChunk())
    {
      rc = false;
      break;
    }
    if (sz != (size_t)chunk_length)
    {
      ON_ERROR("CSampleBloat::Read - wrong amount of bloat in file");
      rc = false;
      break;
    }
    if (TCODE_USER != chunk_tcode)
    {
      ON_ERROR("CSampleBloat::Read - wrong chunk tcode around file bloat");
      rc = false;
      break;
    }

    if (!rc)
      break;

    break;
  }

  if (!binary_archive.EndRead3dmChunk())
  {
    ON_ERROR("CSampleBloat::Read - EndWrite3dmChunk() failed");
    rc = false;
  }

  return rc;
}

// virtual 
bool CSampleBloat::GetDescription(ON_wString& description)
{
  const double mb = ((double)m_sizeof_bloat) / (1024.0*1024.0);
  description.Format(L"Bloat: %g MB (%Lu bytes)", mb, m_sizeof_bloat);
  return true;
}

// virtual 
bool CSampleBloat::Archive() const
{
  return (CSampleBloat::m_bEnableBloat && m_sizeof_bloat > 0);
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleWriteBigModel command
//

#pragma region SampleWriteBigModel command

class CCommandSampleWriteBigModel : public CRhinoCommand
{
public:
  CCommandSampleWriteBigModel() = default;
  UUID CommandUUID() override
  {
    // {63B9FDFC-C629-42F0-B947-209E90B7569A}
    static const GUID SampleWriteBigModelCommand_UUID =
    { 0x63B9FDFC, 0xC629, 0x42F0,{ 0xB9, 0x47, 0x20, 0x9E, 0x90, 0xB7, 0x56, 0x9A } };
    return SampleWriteBigModelCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleWriteBigModel"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleWriteBigModel object
static class CCommandSampleWriteBigModel theSampleWriteBigModelCommand;

CRhinoCommand::result CCommandSampleWriteBigModel::RunCommand(const CRhinoCommandContext& context)
{
  // This sample command will write a ~14 GB 3dm file.

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Brep* brep = go.Object(0).Brep();
  if (NULL == brep)
    return CRhinoCommand::failure;

  CRhinoDump text_log;

  ONX_ErrorCounter error_counter;
  error_counter.ClearLibraryErrors();

  ON_Object* bloated_500_megabyte_brep = brep->Duplicate();
  CSampleBloat::AddBloat(bloated_500_megabyte_brep, 500);

  const int count = 32; // file size ~14 GB

  ONX_Model model;
  for (int i = 0; i < count; i++)
  {
    // The "expert" function adds "count" many brep objects
    // and has the same effect as making "count" copies of
    // the original without waiting for them bo te created.
    // bManaged = false means ONX_Model is not managing
    // the brep.
    const bool bManaged = false;
    model.AddModelGeometryComponentForExperts(bManaged, bloated_500_megabyte_brep, false, nullptr, true);
  }

  const wchar_t* file_name = L"\\sample_write_big_model.3dm";

  ON_wString path;
  CRhinoFileUtilities::GetWindowsSpecialFolderPath(CSIDL_DESKTOPDIRECTORY, path);
  path += file_name;

  unsigned int brep_count = model.Manifest().ActiveComponentCount(ON_ModelComponent::Type::ModelGeometry);
  text_log.Print(
    L"ONX_Model has %u bloated breps and will be saved in <DESKTOP>%ls\n",
    brep_count,
    file_name
  );

  const int version = ON_BinaryArchive::CurrentArchiveVersion();
  model.m_sStartSectionComments = __FILE__ "sample_write_big_model" __DATE__;

  ON_TextLog error_log;
  bool rc = model.Write(path, version, &error_log);

  error_counter.AddLibraryErrors();

  if (rc)
    text_log.Print(L"Test file written successfully.\n");
  else
    text_log.Print(L"Error writing test file.\n");

  error_counter.Dump(text_log);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleWriteBigModel command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
