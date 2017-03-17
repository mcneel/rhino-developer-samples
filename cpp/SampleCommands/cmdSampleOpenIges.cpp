#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOpenIges command
//

#pragma region SampleOpenIges command

// Commands that call CRhinoApp::RunScript MUST be derived from
// CRhinoScriptCommand, not CRhinoCommand.
class CCommandSampleOpenIges : public CRhinoScriptCommand
{
public:
  CCommandSampleOpenIges() {}
  ~CCommandSampleOpenIges() {}
  UUID CommandUUID()
  {
    // {BE07E174-656C-44E2-80EA-17048A601BF1}
    static const GUID SampleOpenIgesCommand_UUID =
    { 0xBE07E174, 0x656C, 0x44E2, { 0x80, 0xEA, 0x17, 0x04, 0x8A, 0x60, 0x1B, 0xF1 } };
    return SampleOpenIgesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleOpenIges"; }
  const wchar_t* LocalCommandName() const { return L"SampleOpenIges"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleOpenIges object
static class CCommandSampleOpenIges theSampleOpenIgesCommand;

CRhinoCommand::result CCommandSampleOpenIges::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString filename;

  if (context.IsInteractive())
  {
    DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    const wchar_t* szFilter = L"IGES Files (*.igs;*.iges)|*.igs; *.iges||";
    CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

#if defined(WIN64)
    CFileDialog dialog(TRUE, L"igs", 0, dwFlags, szFilter, pParentWnd, 0, TRUE);
#else
    CFileDialog dialog(TRUE, L"igs", 0, dwFlags, szFilter, pParentWnd);
#endif
    INT_PTR rc = dialog.DoModal();
    if (rc != IDOK)
      return CRhinoCommand::cancel;

    filename = dialog.GetPathName();
  }
  else
  {
    CRhinoGetString gs;
    gs.SetCommandPrompt(L"IGES file to open");
    gs.GetString();
    if (gs.CommandResult() != CRhinoCommand::success)
      return gs.CommandResult();

    filename = gs.String();
  }

  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"File \"%s\" not found.\n", filename);
    return CRhinoCommand::failure;
  }

  // Note, setting the document modified flag to false will prevent the
  // "Do you want to save this file..." mesasge from displaying when you
  // open a file (if the current document has been modified in any way).
  // But, you will (also) loose any modifications to the current document.
  // So, use the following line of code carefully.
  context.m_doc.SetModifiedFlag(FALSE);

  ON_wString script;
  script.Format(L"_-Open \"%s\" _Enter _Enter _Enter", filename);

  RhinoApp().RunScript(script, 0);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleOpenIges command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
