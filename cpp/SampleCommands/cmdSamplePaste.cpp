#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePaste command
//

#pragma region SamplePaste command

#define RHINO_CLIPBOARD_MIN_VERSION 4
#define RHINO_CLIPBOARD_MAX_VERSION RhinoVersion::App3dmVersionNumber()

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

private:
  unsigned int GetClipboardFormat(unsigned int rhino_version);
  bool GetTemporaryFileName(ON_wString& filename);
  void CloseAndDeleteTemporaryFile(FILE*& fp, const wchar_t*& pszFileName);
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
    unsigned int format = GetClipboardFormat(ver);
    if (0 == format && ver == RHINO_CLIPBOARD_MAX_VERSION)
      return CRhinoCommand::failure;

    if (odo.IsDataAvailable((CLIPFORMAT)format))
    {
      clipboard_format = format;
      break;
    }
  }

  if (clipboard_format < 1)
    return CRhinoCommand::nothing;

  CRhinoCommand::result rc = ReadClipboard(context, odo, clipboard_format);

  return rc;
}

CRhinoCommand::result CCommandSamplePaste::ReadClipboard(const CRhinoCommandContext& context, COleDataObject& odo, unsigned int clipboard_format)
{
  if (clipboard_format < 1)
    return CRhinoCommand::nothing;

  FORMATETC fe;
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

CRhinoCommand::result CCommandSamplePaste::ReadClipboardData(const CRhinoCommandContext& context, HGLOBAL hMemory, unsigned int clipboard_format)
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

    if (GetTemporaryFileName(filename))
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

  CloseAndDeleteTemporaryFile(fp, pszFileName);

  return rc;
}

CRhinoCommand::result CCommandSamplePaste::ReadTemporaryFile(const CRhinoCommandContext& context, const wchar_t* pszFileName)
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

  ONX_Model model;
  bool rc = model.Read(archive);

  ON::CloseFile(archive_fp);

  if (!rc)
  {
    RhinoApp().Print(L"Failed to read the temporary 3dm file.\n");
    return CRhinoCommand::failure;
  }

  //
  // TODO: add the contents of model to the document
  //

  return CRhinoCommand::success;
}

unsigned int CCommandSamplePaste::GetClipboardFormat(unsigned int rhino_version)
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

bool CCommandSamplePaste::GetTemporaryFileName(ON_wString& filename)
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

void CCommandSamplePaste::CloseAndDeleteTemporaryFile(FILE*& fp, const wchar_t*& pszFileName)
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

#pragma endregion

//
// END SamplePaste command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
