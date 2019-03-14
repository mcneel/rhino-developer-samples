#include "stdafx.h"
#include "SampleCommandsPlugIn.h"
#include "SampleDocumentUserData.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleReadDocumentUserData command
//

#pragma region SampleReadDocumentUserData command

class CCommandSampleReadDocumentUserData : public CRhinoCommand
{
public:
  CCommandSampleReadDocumentUserData() = default;
  UUID CommandUUID() override
  {
    // {50BD772E-69D8-4C0F-B165-EE396D579C45}
    static const GUID SampleReadDocumentUserDataCommand_UUID =
    { 0x50BD772E, 0x69D8, 0x4C0F, { 0xB1, 0x65, 0xEE, 0x39, 0x6D, 0x57, 0x9C, 0x45 } };
    return SampleReadDocumentUserDataCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleReadDocumentUserData"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleReadDocumentUserData object
static class CCommandSampleReadDocumentUserData theSampleReadDocumentUserDataCommand;

CRhinoCommand::result CCommandSampleReadDocumentUserData::RunCommand(const CRhinoCommandContext& context)
{
  // Demonstrates how to read our document user data from a 3dm file.
  // Note, can be done just using openNURBS.

  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  bool rc = gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_3dm_file_dialog, 0, RhinoApp().MainWnd()) ? true : false;
  if (!rc)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  const wchar_t* psz_filename = static_cast<const wchar_t*>(filename);
  if (!CRhinoFileUtilities::FileExists(psz_filename))
  {
    RhinoApp().Print(L"File not found\n");
    return CRhinoCommand::failure;
  }

  FILE* archive_fp = ON::OpenFile(filename, L"rb");
  if (0 == archive_fp)
  {
    RhinoApp().Print(L"Unable to open file\n");
    return CRhinoCommand::failure;
  }

  ON_BinaryFile archive(ON::archive_mode::read3dm, archive_fp);
  ONX_Model model;
  rc = model.Read(archive);
  ON::CloseFile(archive_fp);
  if (!rc)
  {
    RhinoApp().Print(L"Error reading file\n");
    return CRhinoCommand::failure;
  }

  // Search for plug-in's document user data
  ONX_Model_UserData* model_ud = nullptr;
  for (unsigned int i = 0; i < model.m_userdata_table.UnsignedCount(); i++)
  {
    ONX_Model_UserData* ptr = model.m_userdata_table[i];
    if (nullptr != ptr && ptr->m_uuid == SampleCommandsPlugIn().PlugInID())
    {
      model_ud = ptr;
      break;
    }
  }

  if (nullptr == model_ud)
  {
    RhinoApp().Print(L"SampleDocumentUserData not found\n");
    return CRhinoCommand::failure;
  }

  // Create a buffer archive using using the model user data's goo
  ON_Read3dmBufferArchive buffer_archive(
    model_ud->m_goo.m_value,
    (const void*)model_ud->m_goo.m_goo,
    false,
    model.m_3dm_file_version,
    model.m_3dm_opennurbs_version
  );

  // Try reading our user data
  CSampleDocumentUserData user_data;
  rc = user_data.Read(buffer_archive);
  if (rc)
  {
    for (int i = 0; i < user_data.Count(); i++)
    {
      ON_wString str = user_data.GetAt(i);
      if (!str.IsEmpty())
        RhinoApp().Print(L"SampleDocumentUserData(%d) = %s\n", i, static_cast<const wchar_t*>(str));
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleReadDocumentUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
