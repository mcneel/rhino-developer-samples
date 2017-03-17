#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddSearchPath command
//

#pragma region SampleAddSearchPath command

class CCommandSampleAddSearchPath : public CRhinoCommand
{
public:
  CCommandSampleAddSearchPath() {}
  ~CCommandSampleAddSearchPath() {}
  UUID CommandUUID()
  {
    // {DE0F201C-6D7D-4F7E-8B02-1A704EB84306}
    static const GUID SampleAddSearchPathCommand_UUID =
    { 0xDE0F201C, 0x6D7D, 0x4F7E, { 0x8B, 0x02, 0x1A, 0x70, 0x4E, 0xB8, 0x43, 0x06 } };
    return SampleAddSearchPathCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAddSearchPath"; }
  const wchar_t* LocalCommandName() const { return L"SampleAddSearchPath"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  int AddFileSearchFolder(const wchar_t* folder, int index = -1);
  bool BrowseForFolder(ON_wString& folder);
};

// The one and only CCommandSampleAddSearchPath object
static class CCommandSampleAddSearchPath theSampleAddSearchPathCommand;

CRhinoCommand::result CCommandSampleAddSearchPath::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  ON_wString folder;
  if (BrowseForFolder(folder))
    AddFileSearchFolder((const wchar_t*)folder);

  return CRhinoCommand::success;
}

int CCommandSampleAddSearchPath::AddFileSearchFolder(const wchar_t* folder, int index)
{
  int rc = -1;

  if (folder && folder[0] && CRhinoFileUtilities::DirExists(folder))
  {
    CRhinoDirectoryManager& dm = RhinoApp().RhinoDirectoryManager();
    const int count = dm.SearchPathCount();
    for (int i = 0; i < count; i++)
    {
      if (0 == on_wcsicmp(folder, dm.SearchPath(i)))
      {
        rc = i;
        break;
      }
    }

    if (rc == -1)
    {
      if (index <= -1)
        rc = dm.AppendSearchPath(folder);
      else if (dm.InsertSearchPath(index, folder))
        rc = index;
    }
  }

  return rc;
}

bool CCommandSampleAddSearchPath::BrowseForFolder(ON_wString& folder)
{
  bool rc = false;

  BROWSEINFO browse_info;
  memset(&browse_info, 0, sizeof(browse_info));
  browse_info.ulFlags = BIF_USENEWUI;
  browse_info.hwndOwner = RhinoApp().MainWnd();
  browse_info.lpszTitle = L"Select Folder";

  ::OleInitialize(NULL);

  LPITEMIDLIST pIDL = ::SHBrowseForFolder(&browse_info);
  if (0 != pIDL)
  {
    wchar_t szFolder[MAX_PATH + 1];
    if (0 != ::SHGetPathFromIDList(pIDL, szFolder))
      rc = true;

    ::CoTaskMemFree(pIDL);
    folder = szFolder;
  }

  ::OleUninitialize();

  return rc;
}

#pragma endregion

//
// END SampleAddSearchPath command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
