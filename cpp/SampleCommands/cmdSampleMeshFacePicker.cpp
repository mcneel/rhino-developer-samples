#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshFacePicker command
//

class CRhGetMeshFaceRef : public CRhinoGetObject
{
public:
  CRhGetMeshFaceRef(unsigned int runtime_object_serial_number);
  //CRhinoGetObject override
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;

private:
  unsigned int m_runtime_object_serial_number;
};

CRhGetMeshFaceRef::CRhGetMeshFaceRef(unsigned int runtime_object_serial_number)
  : m_runtime_object_serial_number(runtime_object_serial_number)
{
}

bool CRhGetMeshFaceRef::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
{
  UNREFERENCED_PARAMETER(geometry);
  UNREFERENCED_PARAMETER(component_index);
  return (nullptr != object && object->RuntimeSerialNumber() == m_runtime_object_serial_number);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleMeshFacePicker command

class CCommandSampleMeshFacePicker : public CRhinoCommand
{
public:
  CCommandSampleMeshFacePicker() {}
  ~CCommandSampleMeshFacePicker() {}
  UUID CommandUUID()
  {
    // {D938E82B-90B-4DB7-8E10-55B9C7A8DC73}
    static const GUID SampleMeshFacePickerCommand_UUID =
    { 0xD938E82B, 0x90B, 0x4DB7,{ 0x8E, 0x10, 0x55, 0xB9, 0xC7, 0xA8, 0xDC, 0x73 } };
    return SampleMeshFacePickerCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshFacePicker"; }
  const wchar_t* LocalCommandName() const { return L"SampleMeshFacePicker"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshFacePicker object
static class CCommandSampleMeshFacePicker theSampleMeshFacePickerCommand;

CRhinoCommand::result CCommandSampleMeshFacePicker::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoMeshObject* mesh_object = CRhinoMeshObject::Cast(go.Object(0).Object());
  if (0 == mesh_object)
    return CRhinoCommand::failure;

  CRhGetMeshFaceRef gf(mesh_object->RuntimeSerialNumber());
  gf.SetCommandPrompt(L"Select mesh face");
  gf.SetGeometryFilter(CRhinoGetObject::meshface_filter);
  gf.GetObjects(1, 1);
  if (gf.CommandResult() != CRhinoCommand::success)
    return gf.CommandResult();

  const ON_MeshComponentRef* mesh_face_ref = gf.Object(0).MeshComponentRef();
  if (nullptr == mesh_face_ref)
    return CRhinoCommand::failure;

  RhinoApp().Print(L"Selected mesh face index = %d\n", mesh_face_ref->MeshFaceIndex());

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMeshFacePicker command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
