#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleImportEarthAnchor command
//

#pragma region SampleImportEarthAnchor command

class CCommandSampleImportEarthAnchor : public CRhinoCommand
{
public:
  CCommandSampleImportEarthAnchor() {}
  ~CCommandSampleImportEarthAnchor() {}
  UUID CommandUUID()
  {
    // {D533B21F-5B7F-4890-BA29-56F8C2217061}
    static const GUID SampleImportEarthAnchorCommand_UUID =
    { 0xD533B21F, 0x5B7F, 0x4890, { 0xBA, 0x29, 0x56, 0xF8, 0xC2, 0x21, 0x70, 0x61 } };
    return SampleImportEarthAnchorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleImportEarthAnchor"; }
  const wchar_t* LocalCommandName() const { return L"SampleImportEarthAnchor"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleImportEarthAnchor object
static class CCommandSampleImportEarthAnchor theSampleImportEarthAnchorCommand;

CRhinoCommand::result CCommandSampleImportEarthAnchor::RunCommand( const CRhinoCommandContext& context )
{
  CWnd* pMainWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  if (0 == pMainWnd)
    return CRhinoCommand::failure;
 
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL rc = gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_3dm_file_dialog, 0, pMainWnd );
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

  FILE* archive_fp = ON::OpenFile(filename, L"rb");
  if (0 == archive_fp)
    return CRhinoCommand::failure;

  ON_3dmSettings settings;
  bool bSuccess = false;

  ON_BinaryFile archive(ON::archive_mode::read3dm, archive_fp);
  for (;;)
  {
    // STEP 1: REQUIRED - Read start section
    int file_version = 0;
    ON_String start_section_comments;
    if (!archive.Read3dmStartSection(&file_version, start_section_comments))
      break;

    // STEP 2: REQUIRED - Read properties section
    ON_3dmProperties properties;
    if (!archive.Read3dmProperties(properties))
      break;

    // STEP 3: REQUIRED - Read properties section
    if( !archive.Read3dmSettings(settings) )
      break;

    // Must have worked
    bSuccess = true;
    break;
  }

  ON::CloseFile(archive_fp);

  if (bSuccess)
  {
    context.m_doc.Properties().SetEarthAnchorPoint(settings.m_earth_anchor_point);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleImportEarthAnchor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
