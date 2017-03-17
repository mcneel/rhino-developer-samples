#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshBooleanIntersection command
//

class CCommandSampleMeshBooleanIntersection : public CRhinoCommand
{
public:
  CCommandSampleMeshBooleanIntersection() {}
  ~CCommandSampleMeshBooleanIntersection() {}
  UUID CommandUUID()
  {
    // {2D0CADB1-AAAF-4A5C-AA4B-EA2A5253DA3B}
    static const GUID SampleMeshBooleanIntersectionCommand_UUID =
    { 0x2D0CADB1, 0xAAAF, 0x4A5C, { 0xAA, 0x4B, 0xEA, 0x2A, 0x52, 0x53, 0xDA, 0x3B } };
    return SampleMeshBooleanIntersectionCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshBooleanIntersection"; }
  const wchar_t* LocalCommandName() { return L"SampleMeshBooleanIntersection"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshBooleanIntersection object
static class CCommandSampleMeshBooleanIntersection theSampleMeshBooleanIntersectionCommand;

CRhinoCommand::result CCommandSampleMeshBooleanIntersection::RunCommand(const CRhinoCommandContext& context)
{
  int i = 0;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select first set of meshes");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  ON_SimpleArray<const ON_Mesh*> InMeshes0(go.ObjectCount());
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    if (0 == mesh)
      return CRhinoCommand::failure;
    InMeshes0.Append(mesh);
  }

  go.SetCommandPrompt(L"Select second set of meshes");
  go.EnablePreSelect(false);
  go.EnableDeselectAllBeforePostSelect(false);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  ON_SimpleArray<const ON_Mesh*> InMeshes1(go.ObjectCount());
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    if (0 == mesh)
      return CRhinoCommand::failure;
    InMeshes1.Append(mesh);
  }

  double intersection_tolerance = ON_SQRT_EPSILON * 10;
  double overlap_tolerance = ON_SQRT_EPSILON * 10;

  ON_SimpleArray<ON_Mesh*> OutMeshes;
  bool bSomethingHappened = false;
  bool rc = RhinoMeshBooleanIntersection(
    InMeshes0,
    InMeshes1,
    intersection_tolerance,
    overlap_tolerance,
    &bSomethingHappened,
    OutMeshes
  );

  if (!rc)
  {
    RhinoApp().Print(L"No mesh intersections found.\n");
    return CRhinoCommand::nothing;
  }

  for (i = 0; i < OutMeshes.Count(); i++)
  {
    CRhinoMeshObject* mesh_obj = new CRhinoMeshObject();
    mesh_obj->SetMesh(OutMeshes[i]);

    OutMeshes[i] = 0;

    if (context.m_doc.AddObject(mesh_obj))
      mesh_obj->Select(true);
    else
      delete mesh_obj;
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleMeshBooleanIntersection command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
