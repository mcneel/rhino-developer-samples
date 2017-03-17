#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSave3DS command
//

#pragma region SampleSave3DS command

class CCommandSampleSave3DS : public CRhinoScriptCommand
{
public:
  CCommandSampleSave3DS() {}
  ~CCommandSampleSave3DS() {}
  UUID CommandUUID()
  {
    // {4231C990-96FD-44FD-A720-3CFD150809B1}
    static const GUID SampleSave3DSCommand_UUID =
    { 0x4231C990, 0x96FD, 0x44FD, { 0xA7, 0x20, 0x3C, 0xFD, 0x15, 0x08, 0x09, 0xB1 } };
    return SampleSave3DSCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSave3DS"; }
  const wchar_t* LocalCommandName() const { return L"SampleSave3DS"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSave3DS object
static class CCommandSampleSave3DS theSampleSave3DSCommand;

CRhinoCommand::result CCommandSampleSave3DS::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString filename;

  if (context.IsInteractive())
  {
    const wchar_t* pszDefExt = L".3ds";
    const wchar_t* pszFilter = L"3D Studio (*.3ds)|*.3ds||";
    DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

    CFileDialog dlg(FALSE, pszDefExt, 0, dwFlags, pszFilter, pParentWnd, 0, TRUE);
    dlg.m_ofn.lpstrTitle = L"SaveAs 3D Studio";
    dlg.m_ofn.nFilterIndex = 1;
    if (dlg.DoModal() == IDCANCEL)
      return CRhinoCommand::cancel;

    filename = dlg.GetPathName();
  }
  else
  {
    CRhinoGetString gs;
    gs.SetCommandPrompt(L"3D Studio file name");
    gs.GetString();
    if (gs.CommandResult() != CRhinoCommand::success)
      return gs.CommandResult();

    filename = gs.String();
  }

  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  // Ensure that the filename has a ".3ds" extension
  filename.SetLength(_MAX_PATH);
  PathAddExtension(filename.Array(), L".3ds");
  filename.ShrinkArray();

  // Script Rhino's SaveAs command. Note, in case the file name
  // string contains spaces, we will want to surround the string
  // with double-quote characters so the command line parser
  // will deal with the string property.
  ON_wString script;
  script.Format(L"_-SaveAs \"%s\" _Enter", filename);
  RhinoApp().RunScript(script, 0);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSave3DS command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
