#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSelectFiles command
//

class CCommandSampleSelectFiles : public CRhinoCommand
{
public:
  CCommandSampleSelectFiles() {}
  ~CCommandSampleSelectFiles() {}
  UUID CommandUUID()
  {
    // {36E2A92C-2605-45A1-89BA-9F234FDD5D92}
    static const GUID SampleSelectFilesCommand_UUID =
    { 0x36E2A92C, 0x2605, 0x45A1, { 0x89, 0xBA, 0x9F, 0x23, 0x4F, 0xDD, 0x5D, 0x92 } };
    return SampleSelectFilesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSelectFiles"; }
  const wchar_t* LocalCommandName() { return L"SampleSelectFiles"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSelectFiles object
static class CCommandSampleSelectFiles theSampleSelectFilesCommand;

CRhinoCommand::result CCommandSampleSelectFiles::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CString strTitle = L"Open";
  CString strFilter = L"Rhino 3D Models (*.3dm)|*.3dm|All Files (*.*)|*.*||";
  CString strDefPath = L"";
  CString strDefFile = L"";
  CString strDefExt = L"";

  DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT;
  CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

  CFileDialog dialog(TRUE, strDefExt, strDefFile, dwFlags, strFilter, pParentWnd, 0, TRUE);
  dialog.m_ofn.lpstrTitle = strTitle;
  dialog.m_ofn.lpstrInitialDir = strDefPath;
  dialog.m_ofn.nFilterIndex = 1;

  DWORD dwNumFileNames = 1024; // Some arbitrarily big number...
  DWORD dwFileNameMaxLength = MAX_PATH + 1;
  DWORD dwBufferSize = (dwNumFileNames * dwFileNameMaxLength) + 1;

  CString strBuffer;
  dialog.m_ofn.lpstrFile = strBuffer.GetBuffer(dwBufferSize);
  dialog.m_ofn.nMaxFile = dwBufferSize;
  dialog.m_ofn.nFileOffset = 0;

  INT_PTR rc = dialog.DoModal();
  strBuffer.ReleaseBuffer();

  if (rc == IDOK)
  {
    POSITION pos = dialog.GetStartPosition();
    while (pos)
    {
      CString strPathName = dialog.GetNextPathName(pos);
      if (!strPathName.IsEmpty())
        RhinoApp().Print(L"%s\n", strPathName);
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleSelectFiles command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
