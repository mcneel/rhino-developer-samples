#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDump3dmInstanceDefinitions command
//

#pragma region SampleDump3dmInstanceDefinitions command

class CCommandSampleDump3dmInstanceDefinitions : public CRhinoCommand
{
public:
  CCommandSampleDump3dmInstanceDefinitions() = default;
  UUID CommandUUID() override
  {
    // {41C4E860-1E36-48D8-AE89-39061D30C4BB}
    static const GUID SampleDump3dmInstanceDefinitionsCommand_UUID =
    { 0x41C4E860, 0x1E36, 0x48D8,{ 0xAE, 0x89, 0x39, 0x06, 0x1D, 0x30, 0xC4, 0xBB } };
    return SampleDump3dmInstanceDefinitionsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDump3dmInstanceDefinitions"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  void DumpInstanceDefinition(ONX_Model& model, const ON_UUID& idef_id, ON_TextLog& dump, bool bRoot);
  ON_wString ObjectTypeToString(ON::object_type type);
};

// The one and only CCommandSampleDump3dmInstanceDefinitions object
static class CCommandSampleDump3dmInstanceDefinitions theSampleDump3dmInstanceDefinitionsCommand;

CRhinoCommand::result CCommandSampleDump3dmInstanceDefinitions::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString str;
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL rc = gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_3dm_file_dialog, nullptr, RhinoApp().MainWnd());
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

  ON_wString writer;
  ON_TextLog dump(writer);
  dump.SetIndentSize(4);

  ONX_ModelComponentIterator it(model, ON_ModelComponent::Type::InstanceDefinition);
  const ON_ModelComponent* model_component = nullptr;
  for (model_component = it.FirstComponent(); nullptr != model_component; model_component = it.NextComponent())
  {
    const ON_InstanceDefinition* idef = ON_InstanceDefinition::Cast(model_component);
    if (nullptr != idef)
    {
      DumpInstanceDefinition(model, idef->Id(), dump, true);
    }
  }

  RhinoApp().Print(L"%ls\n", static_cast<const wchar_t*>(writer));

  return CRhinoCommand::success;
}

void CCommandSampleDump3dmInstanceDefinitions::DumpInstanceDefinition(ONX_Model& model, const ON_UUID& idef_id, ON_TextLog& dump, bool bRoot)
{
  const ON_ModelComponentReference& idef_component_ref = model.ComponentFromId(ON_ModelComponent::Type::InstanceDefinition, idef_id);
  const ON_InstanceDefinition* idef = ON_InstanceDefinition::Cast(idef_component_ref.ModelComponent());
  if (idef)
  {
    // Dump output
    ON_wString node = (bRoot) ? L"\x2500" : L"\x2514";
    dump.Print(L"%ls instance_definition %d = %ls\n", static_cast<const wchar_t*>(node), idef->Index(), static_cast<const wchar_t*>(idef->Name()));

    const ON_SimpleArray<ON_UUID>& geometry_id_list = idef->InstanceGeometryIdList();
    const int geometry_id_count = geometry_id_list.Count();
    if (geometry_id_count > 0)
    {
      dump.PushIndent();
      for (int i = 0; i < geometry_id_count; i++)
      {
        const ON_ModelComponentReference& model_component_ref = model.ComponentFromId(ON_ModelComponent::Type::ModelGeometry, geometry_id_list[i]);
        const ON_ModelGeometryComponent* model_geometry = ON_ModelGeometryComponent::Cast(model_component_ref.ModelComponent());
        if (nullptr != model_geometry)
        {
          const ON_Geometry* geometry = model_geometry->Geometry(nullptr);
          if (nullptr != geometry)
          {
            const ON_InstanceRef* iref = ON_InstanceRef::Cast(geometry);
            if (iref)
            {
              DumpInstanceDefinition(model, iref->m_instance_definition_uuid, dump, false);
            }
            else
            {
              ON_wString type = ObjectTypeToString(geometry->ObjectType());
              dump.Print(L"\x2514 object %d = %ls\n", i, static_cast<const wchar_t*>(type));
            }
          }
        }
      }
      dump.PopIndent();
    }
  }
}

ON_wString CCommandSampleDump3dmInstanceDefinitions::ObjectTypeToString(ON::object_type type)
{
  ON_wString rc = L"Unknown";
  switch (type)
  {
  case ON::point_object: rc = L"point"; break;
  case ON::pointset_object: rc = L"pointset"; break;
  case ON::curve_object: rc = L"curve"; break;
  case ON::surface_object: rc = L"surface"; break;
  case ON::brep_object: rc = L"brep"; break;
  case ON::mesh_object: rc = L"mesh"; break;
  case ON::layer_object: rc = L"layer"; break;
  case ON::material_object: rc = L"material"; break;
  case ON::light_object: rc = L"light"; break;
  case ON::annotation_object: rc = L"annotation"; break;
  case ON::userdata_object: rc = L"userdata"; break;
  case ON::instance_definition: rc = L"instance_definition"; break;
  case ON::instance_reference: rc = L"instance_reference"; break;
  case ON::text_dot: rc = L"text_dot"; break;
  case ON::grip_object: rc = L"grip"; break;
  case ON::detail_object: rc = L"detail"; break;
  case ON::hatch_object: rc = L"hatch"; break;
  case ON::morph_control_object: rc = L"morph_control"; break;
  case ON::subd_object: rc = L"subd"; break;
  case ON::cage_object: rc = L"cage"; break;
  case ON::phantom_object: rc = L"phantom"; break;
  case ON::clipplane_object: rc = L"clipplane"; break;
  case ON::extrusion_object: rc = L"extrusion"; break;
  }
  return rc;
}

#pragma endregion

//
// END SampleDump3dmInstanceDefinitions command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
