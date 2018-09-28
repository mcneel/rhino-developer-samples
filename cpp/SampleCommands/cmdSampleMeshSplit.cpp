#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshSplit command
//

#pragma region SampleMeshSplit command

class CCommandSampleMeshSplit : public CRhinoCommand
{
public:
  CCommandSampleMeshSplit() = default;
  UUID CommandUUID() override
  {
    // {A28A208-8478-4819-8165-B72C8BFD4493}
    static const GUID SampleMeshSplitCommand_UUID =
    { 0xA28A208, 0x8478, 0x4819, { 0x81, 0x65, 0xB7, 0x2C, 0x8B, 0xFD, 0x44, 0x93 } };
    return SampleMeshSplitCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMeshSplit"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMeshSplit object
static class CCommandSampleMeshSplit theSampleMeshSplitCommand;

CRhinoCommand::result CCommandSampleMeshSplit::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go0;
  go0.SetCommandPrompt(L"Select mesh to split");
  go0.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go0.EnableSubObjectSelect(FALSE);
  go0.GetObjects(1, 1);
  if (go0.CommandResult() != CRhinoCommand::success)
    return go0.CommandResult();

  const CRhinoObjRef& mesh_ref = go0.Object(0);
  const CRhinoObject* obj = mesh_ref.Object();
  const ON_Mesh* mesh = mesh_ref.Mesh();
  if (nullptr == obj || nullptr == mesh)
    return CRhinoCommand::failure;

  mesh->Topology();
  if (false == mesh->HasFaceNormals())
  {
    ON_Mesh* pMesh = const_cast<ON_Mesh*>(mesh);
    if (nullptr != pMesh)
      pMesh->ComputeFaceNormals();
  }

  CRhinoGetObject go1;
  go1.SetCommandPrompt(L"Select cutting mesh");
  go1.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go1.EnableSubObjectSelect(FALSE);
  go1.EnablePreSelect(FALSE);
  go1.EnableDeselectAllBeforePostSelect(false);
  go1.GetObjects(1, 1);
  if (go1.CommandResult() != CRhinoCommand::success)
    return go1.CommandResult();

  const ON_Mesh* cutter = mesh_ref.Mesh();
  if (nullptr == cutter)
    return CRhinoCommand::failure;

  cutter->Topology();
  if (false == cutter->HasFaceNormals())
  {
    ON_Mesh* pMesh = const_cast<ON_Mesh*>(cutter);
    if (nullptr != pMesh)
      pMesh->ComputeFaceNormals();
  }

  ON_SimpleArray<const ON_Mesh*> InMeshes0;
  InMeshes0.Append(mesh);

  ON_SimpleArray<const ON_Mesh*> InMeshes1;
  InMeshes1.Append(cutter);

  ON_SimpleArray<ON_Mesh*> OutMeshes;
  double tol = ON_SQRT_EPSILON * 10;

  bool bResult = false;
  bool rc = RhinoMeshBooleanSplit(
    InMeshes0,
    InMeshes1,
    tol,
    tol,
    &bResult,
    OutMeshes,
    false
  );

  if (rc)
  {
    ON_3dmObjectAttributes attributes = obj->Attributes();
    attributes.m_uuid = ON_nil_uuid;

    for (int i = 0; i < OutMeshes.Count(); i++)
    {
      CRhinoMeshObject* mesh_obj = new CRhinoMeshObject(attributes);
      mesh_obj->SetMesh(OutMeshes[i]);
      OutMeshes[i] = nullptr;
      if (i == 0)
        context.m_doc.ReplaceObject(obj, mesh_obj);
      else
        context.m_doc.AddObject(mesh_obj);
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMeshSplit command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
