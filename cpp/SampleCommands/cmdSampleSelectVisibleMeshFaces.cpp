#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSelectVisibleMeshFaces command
//

#pragma region SampleSelectVisibleMeshFaces command

class CCommandSampleSelectVisibleMeshFaces : public CRhinoCommand
{
public:
  CCommandSampleSelectVisibleMeshFaces() {}
  ~CCommandSampleSelectVisibleMeshFaces() {}
  UUID CommandUUID()
  {
    // {7BD2E090-17CE-4C88-9A43-931291B13DC3}
    static const GUID SampleSelectVisibleMeshFacesCommand_UUID =
    { 0x7BD2E090, 0x17CE, 0x4C88, { 0x9A, 0x43, 0x93, 0x12, 0x91, 0xB1, 0x3D, 0xC3 } };
    return SampleSelectVisibleMeshFacesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSelectVisibleMeshFaces"; }
  const wchar_t* LocalCommandName() const { return L"SampleSelectVisibleMeshFaces"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSelectVisibleMeshFaces object
static class CCommandSampleSelectVisibleMeshFaces theSampleSelectVisibleMeshFacesCommand;

CRhinoCommand::result CCommandSampleSelectVisibleMeshFaces::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh");
  go.SetGeometryFilter(ON::mesh_object);
  go.EnablePreSelect(false);
  go.EnableUnselectObjectsOnExit(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoView* view = go.View();
  if (0 == view)
    return CRhinoCommand::failure;

  const CRhinoMeshObject* mesh_obj = CRhinoMeshObject::Cast(go.Object(0).Object());
  if (0 == mesh_obj)
    return CRhinoCommand::failure;

  ON_Mesh* mesh = const_cast<ON_Mesh*>(mesh_obj->Mesh());
  if (0 == mesh)
    return CRhinoCommand::failure;

  mesh_obj->Select(false);
  context.m_doc.Redraw();

  if (!mesh->HasFaceNormals())
    mesh->ComputeFaceNormals();

  ON_3fVector dir(view->ActiveViewport().VP().CameraZ());
  double min_angle = 0.0;
  double max_angle = 90.0 * (ON_PI / 180);

  for (int fi = 0; fi < mesh->m_F.Count(); fi++)
  {
    const ON_3fVector& norm = mesh->m_FN[fi];
    double dot = ON_DotProduct(dir, norm) / (dir.Length() * norm.Length());
    double angle = acos(dot);
    if (min_angle <= angle && angle <= max_angle)
    {
      ON_COMPONENT_INDEX ci(ON_COMPONENT_INDEX::mesh_face, fi);
      mesh_obj->SelectSubObject(ci, true, true);
    }
  }
  context.m_doc.Redraw();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSelectVisibleMeshFaces command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
