#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSaveCopy command
//

#pragma region SampleSaveCopy command

class CCommandSampleSaveCopy : public CRhinoCommand
{
public:
  CCommandSampleSaveCopy() = default;
  UUID CommandUUID() override
  {
    // {71F54182-4A9B-494D-9F8F-8BB24BFFCFDB}
    static const GUID SampleSaveCopyCommand_UUID =
    { 0x71F54182, 0x4A9B, 0x494D,{ 0x9F, 0x8F, 0x8B, 0xB2, 0x4B, 0xFF, 0xCF, 0xDB } };
    return SampleSaveCopyCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSaveCopy"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSaveCopy object
static class CCommandSampleSaveCopy theSampleSaveCopyCommand;

CRhinoCommand::result CCommandSampleSaveCopy::RunCommand(const CRhinoCommandContext& context)
{
  /*
  Unlike the IncrementalSave command, this command will save a copy of the
  current document and not alter the active document's name.
  */

  ON_wString path = context.m_doc.GetPathName();
  if (path.IsEmpty())
  {
    RhinoApp().Print(L"Please save this document before running this command.\n");
    return CRhinoCommand::nothing;
  }

  ON_wString drive, dir, fname, ext;
  ON_wString::SplitPath(path, &drive, &dir, &fname, &ext);

  ON_wString new_path;
  bool rc = false;
  for (int i = 1; i < 999; i++)
  {
    new_path.Format(
      L"%s%s%s %03d%s",
      static_cast<const wchar_t*>(drive),
      static_cast<const wchar_t*>(dir),
      static_cast<const wchar_t*>(fname),
      i,
      static_cast<const wchar_t*>(ext)
    );

    if (!CRhinoFileUtilities::FileExists(static_cast<const wchar_t*>(new_path)))
    {
      rc = true;
      break;
    }
  }

  if (!rc)
  {
    RhinoApp().Print(L"Unable to save incremental file.\n");
    return CRhinoCommand::nothing;
  }

  const wchar_t* ptr_new_path = static_cast<const wchar_t*>(new_path);

  FILE* fp = ON::OpenFile(ptr_new_path, L"wb");
  if (nullptr == fp)
  {
    RhinoApp().Print(L"Unable to open %s.\n", ptr_new_path);
    return CRhinoCommand::failure;
  }

  ON_BinaryFile archive(ON::archive_mode::write3dm, fp);
  CRhinoFileWriteOptions fwo(CRhinoFileWriteOptions::Defaults);
  rc = context.m_doc.Write3dmFile(archive, fwo);
  ON::CloseFile(fp);

  if (rc)
    RhinoApp().Print(L"File successfully saved as %s.\n", ptr_new_path);
  else
    RhinoApp().Print(L"Failed to save %s.\n", ptr_new_path);

  return (rc) ? CRhinoCommand::success : CRhinoCommand::failure;
}

#pragma endregion

//
// END SampleSaveCopy command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
