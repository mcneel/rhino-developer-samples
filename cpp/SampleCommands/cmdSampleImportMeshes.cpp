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
  CCommandSampleImportMeshes() = default;
  UUID CommandUUID() override
  {
    // {C02E65BA-1D43-4C38-90F1-9A33C91F10FC}
    static const GUID SampleImportMeshesCommand_UUID =
    { 0xC02E65BA, 0x1D43, 0x4C38, { 0x90, 0xF1, 0x9A, 0x33, 0xC9, 0x1F, 0x10, 0xFC } };
    return SampleImportMeshesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleImportMeshes"; }
 CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleImportMeshes object
static class CCommandSampleImportMeshes theSampleImportMeshesCommand;

CRhinoCommand::result CCommandSampleImportMeshes::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL rc = gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_3dm_file_dialog, 0, RhinoApp().MainWnd());
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

  ON_BinaryFile archive(ON::archive_mode::read3dm, archive_fp);

  ONX_Model model;
  rc = model.Read(archive) ? TRUE : FALSE;

  ON::CloseFile(archive_fp);

  if (!rc)
  {
    RhinoApp().Print(L"Error reading file\n");
    return CRhinoCommand::failure;
  }

  const ON_UnitSystem& from_model_units = model.m_settings.m_ModelUnitsAndTolerances.m_unit_system;
  const ON_UnitSystem& to_model_units = context.m_doc.ModelUnits();
  double scale = ON::UnitScale(from_model_units, to_model_units);

  bool bScale = (0.0 != scale || 1.0 != scale);
  ON_Xform xform = ON_Xform::IdentityTransformation;
  if (bScale)
    xform = ON_Xform::DiagonalTransformation(scale);

  int num_imported = 0;

  ONX_ModelComponentIterator it(model, ON_ModelComponent::Type::ModelGeometry);
  const ON_ModelComponent* model_component = nullptr;
  for (model_component = it.FirstComponent(); nullptr != model_component; model_component = it.NextComponent())
  {
    const ON_ModelGeometryComponent* model_geometry = ON_ModelGeometryComponent::Cast(model_component);
    if (nullptr != model_geometry)
    {
      const ON_Mesh* const_mesh = ON_Mesh::Cast(model_geometry->Geometry(nullptr));
      if (nullptr != const_mesh)
      {
        ON_Mesh* mesh = new ON_Mesh(*const_mesh);
        if (bScale)
          mesh->Transform(xform);

        CRhinoMeshObject* mesh_obj = new CRhinoMeshObject();
        mesh_obj->SetMesh(mesh);
        mesh = nullptr;

        context.m_doc.AddObject(mesh_obj);
        num_imported++;
      }
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
