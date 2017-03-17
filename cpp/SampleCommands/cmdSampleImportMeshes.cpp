#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleImportMeshes command
//

#pragma region SampleImportMeshes command

class CCommandSampleImportMeshes : public CRhinoCommand
{
public:
  CCommandSampleImportMeshes() {}
  ~CCommandSampleImportMeshes() {}
  UUID CommandUUID()
  {
    // {C02E65BA-1D43-4C38-90F1-9A33C91F10FC}
    static const GUID SampleImportMeshesCommand_UUID =
    { 0xC02E65BA, 0x1D43, 0x4C38, { 0x90, 0xF1, 0x9A, 0x33, 0xC9, 0x1F, 0x10, 0xFC } };
    return SampleImportMeshesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleImportMeshes"; }
  const wchar_t* LocalCommandName() const { return L"SampleImportMeshes"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleImportMeshes object
static class CCommandSampleImportMeshes theSampleImportMeshesCommand;

CRhinoCommand::result CCommandSampleImportMeshes::RunCommand( const CRhinoCommandContext& context )
{
  CWnd* pMainWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  if (0 == pMainWnd)
    return CRhinoCommand::failure;
 
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL rc = gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_only_dialog, 0, pMainWnd);
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
  {
    RhinoApp().Print(L"Unable to open file\n");
    return CRhinoCommand::failure;
  }

  ON_BinaryFile archive(ON::read3dm, archive_fp);

  ONX_Model model;
  rc = model.Read(archive) ? TRUE : FALSE;

  ON::CloseFile( archive_fp );

  if (!rc)
  {
    RhinoApp().Print(L"Error reading file\n");
    return CRhinoCommand::failure;
  }

  int num_imported = 0;
  for (int i = 0; i < model.m_object_table.Count(); i++)
  {
    const ONX_Model_Object& model_object = model.m_object_table[i];
    const ON_Mesh* mesh = ON_Mesh::Cast(model_object.m_object);
    if (0 != mesh)
    {
      // CRhinoDoc::AddMeshObject makes a copy of the input mesh
      context.m_doc.AddMeshObject(*mesh);
      num_imported++;
    }
  }

  if (0 == num_imported)
    RhinoApp().Print(L"No meshes imported\n");
  else if (1 == num_imported)
    RhinoApp().Print(L"1 mesh imported\n");
  else
    RhinoApp().Print(L"%d meshes imported\n", num_imported);

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleImportMeshes command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
