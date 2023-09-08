#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOpen3dmFile command
//

#pragma region SampleOpen3dmFile command

// Commands that script other commands must be derived from
// CRhinoScriptCommand, not CRhinoCommand.
class CCommandSampleOpen3dmFile : public CRhinoScriptCommand
{
public:
  CCommandSampleOpen3dmFile() = default;
  UUID CommandUUID() override
  {
    // {3B60E4C-E304-46C2-AF80-E554A9F86740}
    static const GUID SampleOpen3dmFileCommand_UUID =
    { 0x3B60E4C, 0xE304, 0x46C2, { 0xAF, 0x80, 0xE5, 0x54, 0xA9, 0xF8, 0x67, 0x40 } };
    return SampleOpen3dmFileCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleOpen3dmFile"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleOpen3dmFile object
static class CCommandSampleOpen3dmFile theSampleOpen3dmFileCommand;

CRhinoCommand::result CCommandSampleOpen3dmFile::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  bool rc = gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_3dm_file_dialog, nullptr, RhinoApp().MainWnd());
  if (!rc)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"File not found\n");
    return CRhinoCommand::failure;
  }

  // In case the path string contains spaces, wrap the string in double-quote characters.
  ON_wString script;
  script.Format(L"_-Open \"%ls\" _Enter", static_cast<const wchar_t*>(filename));
  RhinoApp().RunScript(doc->RuntimeSerialNumber(), static_cast<const wchar_t*>(script), 0);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleOpen3dmFile command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
