#include "stdafx.h"

#define RHINO_CLIPBOARD_MIN_VERSION 4
#define RHINO_CLIPBOARD_MAX_VERSION RhinoVersion::App3dmVersionNumber()

static unsigned int Internal_GetClipboardFormat(
  unsigned int rhino_version
)
{
  static unsigned int clipboard_formats[16] = { 0 };

  if (rhino_version < RHINO_CLIPBOARD_MIN_VERSION || rhino_version > RHINO_CLIPBOARD_MAX_VERSION)
    return 0;

  if (0 == clipboard_formats[0])
  {
    clipboard_formats[0] = 1;
    const unsigned int capacity = (unsigned int)(sizeof(clipboard_formats) / sizeof(clipboard_formats[0]));
    for (unsigned int ver = RHINO_CLIPBOARD_MIN_VERSION; ver <= RHINO_CLIPBOARD_MAX_VERSION && ver < capacity; ver++)
    {
      ON_wString format;
      if (ver <= 5)
        format.Format(L"Rhino %d.0 3DM Clip global mem", ver);
      else
        format.Format(L"Rhino %d 3DM Geometry", ver);
      const wchar_t* pszFormat = format.Array();
      clipboard_formats[ver] = ::RegisterClipboardFormat(pszFormat);
    }
  }

  return clipboard_formats[rhino_version];
}

static bool Internal_GetTemporaryFileName(
  ON_wString& filename
)
{
  wchar_t pszPath[_MAX_PATH + 2];
  wchar_t pszFileName[sizeof(pszPath) / sizeof(pszPath[0])];
  const unsigned int s_capacity = (unsigned int)(sizeof(pszPath) / sizeof(pszPath[0]));

  memset(pszPath, 0, sizeof(pszPath));
  memset(pszFileName, 0, sizeof(pszFileName));

  const DWORD path_length = ::GetTempPath(s_capacity - 2, pszPath);
  if (path_length <= 0 || path_length >= s_capacity)
    return false;

  pszPath[path_length] = 0;
  const unsigned int temp_file_id = ::GetTempFileName(pszPath, L"rh$", 0, pszFileName);
  if (0 == temp_file_id)
    return false;

  pszFileName[s_capacity - 1] = 0;
  filename = pszFileName;

  return true;
}

static void Internal_CloseAndDeleteFile(
  FILE*& fp, 
  const wchar_t*& pszFileName
)
{
  if (nullptr != fp)
  {
    fclose(fp);
    fp = nullptr;
  }

  if (nullptr != pszFileName && 0 != pszFileName[0])
  {
    ::DeleteFile(pszFileName);
    pszFileName = nullptr;
  }
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePaste command
//

#pragma region SamplePaste command

class CCommandSamplePaste : public CRhinoCommand
{
public:
  CCommandSamplePaste() = default;
  UUID CommandUUID() override
  {
    // {585DE5BE-F855-4E02-BED4-A089BE4CE33D}
    static const GUID SamplePasteCommand_UUID =
    { 0x585de5be, 0xf855, 0x4e02, { 0xbe, 0xd4, 0xa0, 0x89, 0xbe, 0x4c, 0xe3, 0x3d } };
    return SamplePasteCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePaste"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  CRhinoCommand::result ReadClipboard(
    const CRhinoCommandContext& context,
    COleDataObject& odo,
    unsigned int clipboard_format
  );

  CRhinoCommand::result ReadClipboardData(
    const CRhinoCommandContext& context,
    HGLOBAL hMemory,
    unsigned int clipboard_format
  );

  CRhinoCommand::result ReadTemporaryFile(
    const CRhinoCommandContext& context,
    const wchar_t* pszFileName
  );
};

// The one and only CCommandSamplePaste object
static class CCommandSamplePaste theSamplePasteCommand;

CRhinoCommand::result CCommandSamplePaste::RunCommand(const CRhinoCommandContext& context)
{
  COleDataObject odo;
  if (!odo.AttachClipboard())
  {
    RhinoApp().Print(L"Unable to attach to clipboard.\n");
    return CRhinoCommand::failure;
  }

  unsigned int clipboard_format = 0;
  for (unsigned int ver = RHINO_CLIPBOARD_MAX_VERSION; ver >= RHINO_CLIPBOARD_MIN_VERSION; ver--)
  {
    unsigned int format = Internal_GetClipboardFormat(ver);
    if (0 == format && ver == RHINO_CLIPBOARD_MAX_VERSION)
      return CRhinoCommand::failure;

    if (odo.IsDataAvailable((CLIPFORMAT)format))
    {
      clipboard_format = format;
      break;
    }
  }

  if (0 == clipboard_format)
  {
    RhinoApp().Print(L"Nothing to paste.\n");
    return CRhinoCommand::nothing;
  }

  CRhinoCommand::result rc = ReadClipboard(context, odo, clipboard_format);

  return rc;
}

CRhinoCommand::result CCommandSamplePaste::ReadClipboard(
  const CRhinoCommandContext& context, 
  COleDataObject& odo, 
  unsigned int clipboard_format
)
{
  if (clipboard_format < 1)
    return CRhinoCommand::nothing;

  FORMATETC fe;
  memset(&fe, 0, sizeof(fe));
  fe.cfFormat = (CLIPFORMAT)clipboard_format;
  fe.tymed = TYMED_HGLOBAL;
  fe.lindex = -1;
  fe.dwAspect = DVASPECT_CONTENT;
  fe.ptd = nullptr;

  HGLOBAL hMemory = odo.GetGlobalData(fe.cfFormat, &fe);
  if (nullptr == hMemory)
  {
    RhinoApp().Print(L"Failed to extract data from the clipboard.\n");
    return CRhinoCommand::failure;
  }

  CRhinoCommand::result rc = ReadClipboardData(context, hMemory, clipboard_format);

  if (hMemory)
    ::GlobalFree(hMemory);
  hMemory = nullptr;

  return rc;
}

CRhinoCommand::result CCommandSamplePaste::ReadClipboardData(
  const CRhinoCommandContext& context, 
  HGLOBAL hMemory, 
  unsigned int clipboard_format
)
{
  CRhinoCommand::result rc = CRhinoCommand::failure;

  FILE* fp = nullptr;
  const wchar_t* pszFileName = nullptr;
  ON_wString filename;
  bool bHaveGlobalLock = false;

  for (;;)
  {
    char* ptr = (char*)::GlobalLock(hMemory);
    if (nullptr == ptr)
    {
      RhinoApp().Print(L"Failed to lock the global memory block.\n");
      break;
    }

    bHaveGlobalLock = true;

    if (Internal_GetTemporaryFileName(filename))
      pszFileName = static_cast<const wchar_t*>(filename);
    if (nullptr == pszFileName || 0 == pszFileName[0])
    {
      RhinoApp().Print(L"Failed to get a temporary filename.\n");
      break;
    }

    size_t size = ::GlobalSize(hMemory);
    if (size <= 0)
    {
      RhinoApp().Print(L"Failed to get a valid clipboard memory block.\n");
      break;
    }

    errno_t err = _wfopen_s(&fp, pszFileName, L"wb");
    if (0 != err || nullptr == fp)
    {
      RhinoApp().Print(L"Failed to open the temporary file.\n");
      break;
    }

    if (fwrite(ptr, size, 1, fp) != 1)
    {
      RhinoApp().Print(L"Failed to begin writing to the temporary file.\n");
      break;
    }

    if (ferror(fp))
    {
      RhinoApp().Print(L"Failed to write the temporary file.\n");
      break;
    }

    if (fflush(fp))
    {
      RhinoApp().Print(L"Failed to flush the temporary file.\n");
      break;
    }

    if (fclose(fp))
    {
      fp = nullptr;
      RhinoApp().Print(L"Failed to close the temporary file.\n");
      break;
    }

    fp = nullptr;

    if (bHaveGlobalLock)
    {
      ::GlobalUnlock(hMemory);
      bHaveGlobalLock = false;
    }

    rc = ReadTemporaryFile(context, pszFileName);

    break;
  }

  if (bHaveGlobalLock)
  {
    ::GlobalUnlock(hMemory);
    bHaveGlobalLock = false;
  }

  Internal_CloseAndDeleteFile(fp, pszFileName);

  return rc;
}

CRhinoCommand::result CCommandSamplePaste::ReadTemporaryFile(
  const CRhinoCommandContext& context, 
  const wchar_t* pszFileName
)
{
  if (nullptr == pszFileName || 0 == pszFileName[0])
    return CRhinoCommand::failure;

  FILE* archive_fp = ON::OpenFile(pszFileName, L"rb");
  if (nullptr == archive_fp)
  {
    RhinoApp().Print(L"Failed to open the temporary 3dm file.\n");
    return CRhinoCommand::failure;
  }

  ON_BinaryFile archive(ON::archive_mode::read3dm, archive_fp);

  ON_wString writer;
  ON_TextLog error_log(writer);
  error_log.SetIndentSize(2);

  ONX_Model model;
  bool rc = model.Read(archive, &error_log);

  ON::CloseFile(archive_fp);

  if (!rc)
  {
    RhinoApp().Print(L"Failed to read the temporary 3dm file.\n");
    if (writer.IsNotEmpty())
      RhinoApp().Print(L"%ls\n", static_cast<const wchar_t*>(writer));
    return CRhinoCommand::failure;
  }

  //
  // TODO: add the contents of model to the document
  //

  return CRhinoCommand::success;
}


#pragma endregion

//
// END SamplePaste command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCopyToClipboard command
//

#pragma region SampleCopyToClipboard command

class CCommandSampleCopyToClipboard : public CRhinoCommand
{
public:
  CCommandSampleCopyToClipboard() = default;
  UUID CommandUUID() override
  {
    // {CCB38D47-A41E-423F-891A-8A99ABC4DB88}
    static const GUID SampleCopyToClipboardCommand_UUID =
    { 0xCCB38D47, 0xA41E, 0x423F, { 0x89, 0x1A, 0x8A, 0x99, 0xAB, 0xC4, 0xDB, 0x88 } };
    return SampleCopyToClipboardCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCopyToClipboard"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

  HGLOBAL WriteClipboardData(
    const ONX_Model& model, 
    unsigned int rhino_version
  );

  bool WriteTemporaryFile(
    const ONX_Model& model, 
    const wchar_t* pszFileName, 
    unsigned int rhino_version
  );

private:
  void CreateModelForClipboard(
    ONX_Model& model
  );
};

// The one and only CCommandSampleCopyToClipboard object
static class CCommandSampleCopyToClipboard theSampleCopyToClipboardCommand;

CRhinoCommand::result CCommandSampleCopyToClipboard::RunCommand(const CRhinoCommandContext& context)
{
  const unsigned int rhino_version = RHINO_CLIPBOARD_MAX_VERSION;
  const unsigned int clipboard_format = Internal_GetClipboardFormat(RHINO_CLIPBOARD_MAX_VERSION);
  if (0 == clipboard_format)
    return CRhinoCommand::failure;

  ONX_Model model;
  CreateModelForClipboard(model);

  HGLOBAL hMemory = WriteClipboardData(model, rhino_version);
  if (nullptr == hMemory)
    return CRhinoCommand::failure;

  COleDataSource* ods = new COleDataSource();
  ods->CacheGlobalData((CLIPFORMAT)clipboard_format, hMemory);
  ods->SetClipboard();
  // ods is reference counted - do not delete...

  return CRhinoCommand::success;
}

HGLOBAL CCommandSampleCopyToClipboard::WriteClipboardData(
  const ONX_Model& model, 
  unsigned int rhino_version
)
{
  HGLOBAL hClipboardMemory = nullptr;
  ON_wString filename;
  const wchar_t* pszFileName = nullptr;
  FILE* fp = nullptr;

  for (;;)
  {
    unsigned int clipboard_format = Internal_GetClipboardFormat(rhino_version);
    if (0 == clipboard_format)
      break;

    if (Internal_GetTemporaryFileName(filename))
      pszFileName = filename;
    if (nullptr == pszFileName || 0 == pszFileName[0])
      break;

    if (!WriteTemporaryFile(model, pszFileName, rhino_version))
    {
      pszFileName = nullptr;
      break;
    }

    errno_t err = _wfopen_s(&fp, pszFileName, L"rb");
    if (0 != err || nullptr == fp)
    {
      RhinoApp().Print(L"Failed to open the temporary file.\n");
      break;
    }

    __int64 file_size = _filelengthi64(_fileno(fp));
    if (file_size <= 0)
    {
      RhinoApp().Print(L"The temporary file is empty.\n");
      break;
    }

    HGLOBAL hMemory = GlobalAlloc(GMEM_ZEROINIT | GMEM_DDESHARE | GMEM_MOVEABLE, file_size);
    if (nullptr == hMemory)
    {
      RhinoApp().Print(L"Failed to allocate clipboard memory.\n");
      break;
    }

    unsigned long* ptr = (unsigned long*)GlobalLock(hMemory);
    if (nullptr == ptr)
    {
      GlobalFree(hMemory);
      RhinoApp().Print(L"Failed to lock clipboard memory.\n");
      break;
    }

    if (fread(&ptr[0], file_size, 1, fp) != 1)
    {
      GlobalUnlock(hMemory);
      GlobalFree(hMemory);
      RhinoApp().Print(L"Failed to read the temporary file into clipboard memory.\n");
      break;
    }

    GlobalUnlock(hMemory);

    if (ferror(fp))
    {
      GlobalFree(hMemory);
      RhinoApp().Print(L"Failed to read the temporary file.\n");
      break;
    }

    hClipboardMemory = hMemory;
    break;
  }

  Internal_CloseAndDeleteFile(fp, pszFileName);

  return hClipboardMemory;
}

bool CCommandSampleCopyToClipboard::WriteTemporaryFile(
  const ONX_Model& model,
  const wchar_t* pszFileName, 
  unsigned int rhino_version
)
{
  if (nullptr == pszFileName || 0 == pszFileName[0])
    return false;

  FILE* fp = ON::OpenFile(pszFileName, L"wb");
  if (nullptr == fp)
  {
    RhinoApp().Print(L"Failed to open the temporary 3dm file.\n");
    return false;
  }

  ON_BinaryFile archive(ON::archive_mode::write3dm, fp);

  ON_wString writer;
  ON_TextLog error_log(writer);
  error_log.SetIndentSize(2);
  bool rc = model.Write(archive, rhino_version, &error_log);

  ON::CloseFile(fp);

  if (!rc)
  {
    RhinoApp().Print(L"Failed to write the temporary 3dm file.\n");
    if (writer.IsNotEmpty())
      RhinoApp().Print(L"%ls\n", static_cast<const wchar_t*>(writer));
    ::DeleteFile(pszFileName);
  }

  return rc;
}

void CCommandSampleCopyToClipboard::CreateModelForClipboard(
  ONX_Model& model
)
{
  model.m_sStartSectionComments = "This was file created with the Rhino C/C++ SDK.";
  model.m_properties.m_Application.m_application_name = L"Rhino Developer Sample";
  model.m_properties.m_Application.m_application_URL = L"https://github.com/mcneel/rhino-developer-samples";
  model.m_properties.m_RevisionHistory.NewRevision();

  model.m_settings.m_ModelUnitsAndTolerances.m_unit_system = ON::LengthUnitSystem::Millimeters;
  model.m_settings.m_ModelUnitsAndTolerances.m_absolute_tolerance = 0.01;
  model.m_settings.m_ModelUnitsAndTolerances.m_angle_tolerance = ON_PI / 180.0; // radians
  model.m_settings.m_ModelUnitsAndTolerances.m_relative_tolerance = 0.01; // 1%

  model.AddDefaultLayer(nullptr, ON_Color::UnsetColor);
  const int point1_layer_index = model.AddLayer(L"my layer", ON_Color::Black);
  const int pointcloud_layer_index = model.AddLayer(L"red points", ON_Color::SaturatedRed);
  const int point2_layer_index = model.AddLayer(L"one blue point", ON_Color::SaturatedBlue);

  ON_Point* point1 = new ON_Point(ON_3dPoint(1.0, 4.0, 5.0));
  ON_3dmObjectAttributes* point1_attributes = new ON_3dmObjectAttributes();
  point1_attributes->m_layer_index = point1_layer_index;
  point1_attributes->m_name = L"first point";
  model.AddManagedModelGeometryComponent(point1, point1_attributes);

  ON_PointCloud* pointcloud = new ON_PointCloud();
  pointcloud->AppendPoint(ON_3dPoint(1.0, 6.0, 5.0));
  pointcloud->AppendPoint(ON_3dPoint(1.5, 4.5, 6.0));
  pointcloud->AppendPoint(ON_3dPoint(2.0, 5.0, 7.0));
  ON_3dmObjectAttributes* pointcloud_attributes = new ON_3dmObjectAttributes();
  pointcloud_attributes->m_layer_index = pointcloud_layer_index;
  pointcloud_attributes->m_name = L"three points";
  model.AddManagedModelGeometryComponent(pointcloud, pointcloud_attributes);

  ON_Point* point2 = new ON_Point(ON_3dPoint(3.0, 2.0, 4.0));
  ON_3dmObjectAttributes* point2_attributes = new ON_3dmObjectAttributes();
  point2_attributes->m_layer_index = point2_layer_index;
  point2_attributes->m_name = L"second point";
  model.AddManagedModelGeometryComponent(point2, point2_attributes);
}

#pragma endregion

//
// END SampleCopyToClipboard command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
