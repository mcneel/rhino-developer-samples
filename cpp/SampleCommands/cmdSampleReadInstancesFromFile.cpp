#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleReadInstancesFromFile command
//

#pragma region SampleReadInstancesFromFile command

class CCommandSampleReadInstancesFromFile : public CRhinoCommand
{
public:
  CCommandSampleReadInstancesFromFile() = default;
  UUID CommandUUID() override
  {
    // {72A22580-3CE8-4B10-B8E8-AE31F945C073}
    static const GUID SampleReadInstancesFromFileCommand_UUID =
    { 0x72A22580, 0x3CE8, 0x4B10, { 0xB8, 0xE8, 0xAE, 0x31, 0xF9, 0x45, 0xC0, 0x73 } };
    return SampleReadInstancesFromFileCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleReadInstancesFromFile"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  int ReadInstanceObjectsFromFile(CRhinoDoc& doc, const wchar_t* filename);
  int ReadInstanceDefinitionGeometry(ONX_Model& model, const ON_InstanceRef& iref, const ON_Xform& xform, ON_SimpleArray<ON_Geometry*>& geometry);
  void AddGeometryToDocument(CRhinoDoc& doc, ON_Geometry* geometry);
};

// The one and only CCommandSampleReadInstancesFromFile object
static class CCommandSampleReadInstancesFromFile theSampleReadInstancesFromFileCommand;

CRhinoCommand::result CCommandSampleReadInstancesFromFile::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Prompt the user for a 3dm file

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

  ReadInstanceObjectsFromFile(*doc, filename);

  doc->Redraw();

  return CRhinoCommand::success;
}

int CCommandSampleReadInstancesFromFile::ReadInstanceObjectsFromFile(CRhinoDoc& doc, const wchar_t* filename)
{
  if (nullptr == filename || 0 == filename[0])
    return -1;

  ONX_Model model;
  if (!model.Read(filename))
  {
    RhinoApp().Print(L"Error reading 3dm file\n");
    return -1;
  }

  int rc = 0;

  // Iterate the model geoemtry looking for instance reference objects

  ONX_ModelComponentIterator it(model, ON_ModelComponent::Type::ModelGeometry);
  const ON_ModelComponent* model_component = nullptr;
  for (model_component = it.FirstComponent(); nullptr != model_component; model_component = it.NextComponent())
  {
    const ON_ModelGeometryComponent* model_geometry = ON_ModelGeometryComponent::Cast(model_component);
    if (nullptr != model_geometry)
    {
      const ON_InstanceRef* iref = ON_InstanceRef::Cast(model_geometry->Geometry(nullptr));
      if (nullptr != iref)
      {
        // Get and transform the reference's instance definition geometry
        ON_Xform xform = ON_Xform::IdentityTransformation;
        ON_SimpleArray<ON_Geometry*> geometry_list;
        int geometry_count = ReadInstanceDefinitionGeometry(model, *iref, xform, geometry_list);

        // TODO: do something with the geometry
        for (int i = 0; i < geometry_count; i++)
        {
          ON_Geometry* geometry = geometry_list[i];
          if (nullptr != geometry)
          {
            AddGeometryToDocument(doc, geometry);
            // The above function adds a copy of the geometry to the document.
            // So delete it so we don't leak.
            delete geometry;
          }
        }
      }
    }
  }

  return rc;
}

int CCommandSampleReadInstancesFromFile::ReadInstanceDefinitionGeometry(ONX_Model& model, const ON_InstanceRef& iref, const ON_Xform& xf, ON_SimpleArray<ON_Geometry*>& geometry_list)
{
  const ON_ModelComponentReference& idef_component_ref = model.ComponentFromId(ON_ModelComponent::Type::InstanceDefinition, iref.m_instance_definition_uuid);
  const ON_InstanceDefinition* idef = ON_InstanceDefinition::Cast(idef_component_ref.ModelComponent());
  if (idef)
  {
    ON_Xform xform = xf * iref.m_xform;
    const bool do_xform = (xform.IsValid() && !xform.IsZero() && !xform.IsIdentity());

    const ON_SimpleArray<ON_UUID>& geometry_id_list = idef->InstanceGeometryIdList();
    const int geometry_id_count = geometry_id_list.Count();
    if (geometry_id_count > 0)
    {
      for (int i = 0; i < geometry_id_count; i++)
      {
        const ON_ModelComponentReference& model_component_ref = model.ComponentFromId(ON_ModelComponent::Type::ModelGeometry, geometry_id_list[i]);
        const ON_ModelGeometryComponent* model_geometry = ON_ModelGeometryComponent::Cast(model_component_ref.ModelComponent());
        if (nullptr != model_geometry)
        {
          const ON_Geometry* geometry = model_geometry->Geometry(nullptr);
          if (nullptr != geometry)
          {
            const ON_InstanceRef* nested_iref = ON_InstanceRef::Cast(geometry);
            if (nested_iref)
            {
              ReadInstanceDefinitionGeometry(model, *nested_iref, xform, geometry_list);
            }
            else
            {
              // Copy the geometry. Note, we are responsible for the memory allocated here.
              // So make to dispose of it, otherwise you'll leak memory.
              ON_Geometry* duplicate = geometry->Duplicate();
              if (nullptr != duplicate)
              {
                if (do_xform)
                  duplicate->Transform(xform);
                geometry_list.Append(duplicate);
              }
            }
          }
        }
      }
    }
  }

  return geometry_list.Count();
}

void CCommandSampleReadInstancesFromFile::AddGeometryToDocument(CRhinoDoc& doc, ON_Geometry* geometry)
{
  if (nullptr != geometry)
  {
    switch (geometry->ObjectType())
    {
    case ON::point_object:
    {
      ON_Point* p = ON_Point::Cast(geometry);
      if (nullptr != p)
        doc.AddPointObject(p->point); // make a copy
    }
    break;

    case ON::pointset_object:
    {
      ON_PointCloud* p = ON_PointCloud::Cast(geometry);
      if (nullptr != p)
        doc.AddPointCloudObject(p->m_P.Count(), p->m_P.Array()); // make a copy
    }
    break;

    case ON::curve_object:
    {
      ON_Curve* p = ON_Curve::Cast(geometry);
      if (nullptr != p)
        doc.AddCurveObject(*p); // make a copy
    }
    break;

    case ON::brep_object:
    {
      ON_Brep* p = ON_Brep::Cast(geometry);
      if (nullptr != p)
        doc.AddBrepObject(*p); // make a copy
    }
    break;

    case ON::mesh_object:
    {
      ON_Mesh* p = ON_Mesh::Cast(geometry);
      if (nullptr != p)
        doc.AddMeshObject(*p); // make a copy
    }
    break;

    // TODO: Add support for other objects...
    }
  }
}

#pragma endregion

//
// END SampleReadInstancesFromFile command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
