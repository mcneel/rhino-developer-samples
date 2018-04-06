#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDump3dmInstanceDefinitionsEx command
//

#pragma region SampleDump3dmInstanceDefinitionsEx command

class CCommandSampleDump3dmInstanceDefinitionsEx : public CRhinoCommand
{
public:
  CCommandSampleDump3dmInstanceDefinitionsEx() = default;
  UUID CommandUUID() override
  {
    // {AE86D79E-88A4-40FE-8819-1C07ED04C4F8}
    static const GUID SampleDump3dmInstanceDefinitionsExCommand_UUID =
    { 0xAE86D79E, 0x88A4, 0x40FE,{ 0x88, 0x19, 0x1C, 0x07, 0xED, 0x04, 0xC4, 0xF8 } };
    return SampleDump3dmInstanceDefinitionsExCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDump3dmInstanceDefinitionsEx"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  void DumpInstanceDefinition(ONX_Model& model, const ON_UUID& idef_id, ON_TextLog& dump, bool bRoot);
  ON_wString ObjectTypeToString(ON::object_type type);
};

// The one and only CCommandSampleDump3dmInstanceDefinitionsEx object
static class CCommandSampleDump3dmInstanceDefinitionsEx theSampleDump3dmInstanceDefinitionsExCommand;

CRhinoCommand::result CCommandSampleDump3dmInstanceDefinitionsEx::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);

  HWND hParentWnd = RhinoApp().MainWnd();
  if (!gf.DisplayFileDialog(CRhinoGetFileDialog::open_rhino_3dm_file_dialog, nullptr, hParentWnd))
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

  ON_BinaryFile archive(ON::archive_mode::read3dm, filename);
  if (!archive.FileIsOpen())
  {
    RhinoApp().Print(L"Unable to open file\n");
    return CRhinoCommand::failure;
  }

  ONX_Model model;
  if (!model.Read(archive))
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

void CCommandSampleDump3dmInstanceDefinitionsEx::DumpInstanceDefinition(ONX_Model& model, const ON_UUID& idef_id, ON_TextLog& dump, bool bRoot)
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
      ON_TextLogIndent indent(dump);
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
    }
  }
}

ON_wString CCommandSampleDump3dmInstanceDefinitionsEx::ObjectTypeToString(ON::object_type type)
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
// END SampleDump3dmInstanceDefinitionsEx command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
