#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleReadGeometryFromFile command
//

#pragma region SampleReadGeometryFromFile command

class CCommandSampleReadGeometryFromFile : public CRhinoCommand
{
public:
  CCommandSampleReadGeometryFromFile() = default;
  UUID CommandUUID() override
  {
    // {C86E88C2-5AC-4446-9D09-19157B5F92A2}
    static const GUID SampleReadGeometryFromFileCommand_UUID =
    { 0xC86E88C2, 0x5AC, 0x4446, { 0x9D, 0x09, 0x19, 0x15, 0x7B, 0x5F, 0x92, 0xA2 } };
    return SampleReadGeometryFromFileCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleReadGeometryFromFile"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  CRhinoCommand::result ReadGeometryFromFile(CRhinoDoc& doc, const wchar_t* filename);
  void AddGeometryToDocument(CRhinoDoc& doc, const ON_Geometry* geometry);
};

// The one and only CCommandSampleReadGeometryFromFile object
static class CCommandSampleReadGeometryFromFile theSampleReadGeometryFromFileCommand;

CRhinoCommand::result CCommandSampleReadGeometryFromFile::RunCommand(const CRhinoCommandContext& context)
{
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

  ReadGeometryFromFile(context.m_doc, filename);

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

CRhinoCommand::result  CCommandSampleReadGeometryFromFile::ReadGeometryFromFile(CRhinoDoc& doc, const wchar_t* filename)
{
  if (nullptr == filename || 0 == filename[0])
    return CRhinoCommand::failure;

  ONX_Model model;
  if (!model.Read(filename))
  {
    RhinoApp().Print(L"Error reading 3dm file\n");
    return CRhinoCommand::failure;
  }

  // Iterate the model geoemtry looking for instance reference objects

  ONX_ModelComponentIterator it(model, ON_ModelComponent::Type::ModelGeometry);
  const ON_ModelComponent* model_component = nullptr;
  for (model_component = it.FirstComponent(); nullptr != model_component; model_component = it.NextComponent())
  {
    const ON_ModelGeometryComponent* model_geometry = ON_ModelGeometryComponent::Cast(model_component);
    if (nullptr != model_geometry)
    {
      // Skip instance definition geometry
      const ON_3dmObjectAttributes* attribues = model_geometry->Attributes(nullptr);
      if (nullptr == attribues || attribues->IsInstanceDefinitionObject())
        continue;

      const ON_Geometry* geometry = model_geometry->Geometry(nullptr);
      if (nullptr == geometry)
        continue;

      // Skip instance reference objects
      const ON_InstanceRef* iref = ON_InstanceRef::Cast(geometry);
      if (nullptr != iref)
        continue;

      AddGeometryToDocument(doc, geometry);
    }
  }

  return CRhinoCommand::success;
}

void CCommandSampleReadGeometryFromFile::AddGeometryToDocument(CRhinoDoc& doc, const ON_Geometry* geometry)
{
  if (nullptr != geometry)
  {
    switch (geometry->ObjectType())
    {
    case ON::point_object:
    {
      const ON_Point* p = ON_Point::Cast(geometry);
      if (nullptr != p)
        doc.AddPointObject(p->point); // make a copy
    }
    break;

    case ON::pointset_object:
    {
      const ON_PointCloud* p = ON_PointCloud::Cast(geometry);
      if (nullptr != p)
        doc.AddPointCloudObject(p->m_P.Count(), p->m_P.Array()); // make a copy
    }
    break;

    case ON::curve_object:
    {
      const ON_Curve* p = ON_Curve::Cast(geometry);
      if (nullptr != p)
        doc.AddCurveObject(*p); // make a copy
    }
    break;

    case ON::brep_object:
    {
      const ON_Brep* p = ON_Brep::Cast(geometry);
      if (nullptr != p)
        doc.AddBrepObject(*p); // make a copy
    }
    break;

    case ON::mesh_object:
    {
      const ON_Mesh* p = ON_Mesh::Cast(geometry);
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
// END SampleReadGeometryFromFile command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
