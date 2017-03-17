#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshVolume command
//

class CCommandSampleMeshVolume : public CRhinoCommand
{
public:
  CCommandSampleMeshVolume() {}
  ~CCommandSampleMeshVolume() {}
  UUID CommandUUID()
  {
    // {A7DB77A2-C4C5-4000-A708-1ED9FFFE110B}
    static const GUID SampleMeshVolumeCommand_UUID =
    { 0xA7DB77A2, 0xC4C5, 0x4000, { 0xA7, 0x08, 0x1E, 0xD9, 0xFF, 0xFE, 0x11, 0x0B } };
    return SampleMeshVolumeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshVolume"; }
  const wchar_t* LocalCommandName() { return L"SampleMeshVolume"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshVolume object
static class CCommandSampleMeshVolume theSampleMeshVolumeCommand;

CRhinoCommand::result CCommandSampleMeshVolume::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select solid meshes for volume calculation");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::closed_mesh);
  go.EnableSubObjectSelect(FALSE);
  go.EnableGroupSelect();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  int i;
  ON_SimpleArray<const ON_Mesh*> meshes;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Mesh* mesh = ON_Mesh::Cast(go.Object(i).Geometry());
    if (mesh)
      meshes.Append(mesh);
  }

  const int mesh_count = meshes.Count();
  if (0 == mesh_count)
    return nothing;

  ON_BoundingBox bbox;
  for (i = 0; i < mesh_count; i++)
    meshes[i]->GetBoundingBox(bbox, TRUE);
  ON_3dPoint base_point = bbox.Center();

  double total_volume = 0.0;
  double total_error_estimate = 0.0;
  for (i = 0; i < mesh_count; i++)
  {
    double error_estimate = 0.0;
    double volume = meshes[i]->Volume(base_point, &error_estimate);
    RhinoApp().Print(L"Mesh%d = %f (+/- %f)\n", i, volume, error_estimate);
    total_volume += volume;
    total_error_estimate += error_estimate;
  }

  RhinoApp().Print(L"Total volume = %f (+/- %f)\n", total_volume, total_error_estimate);

  return CRhinoCommand::success;
}

//
// END SampleMeshVolume command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
