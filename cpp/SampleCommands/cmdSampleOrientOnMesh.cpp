#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOrientOnMesh command
//

class CRhGetMeshPoint : public CRhinoGetXform
{
public:
  CRhGetMeshPoint(const ON_Mesh* mesh, const ON_Plane& object_plane);
  BOOL CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform);

private:
  bool MeshFacePlane(ON_MESH_POINT mp, ON_Plane& plane);

private:
  const ON_Mesh* m_mesh;
  ON_Plane m_object_plane;

  // Use your favorite map, dictionary, etc. here...
  CMap<int, int, ON_Plane, ON_Plane&> m_face_planes;
};

CRhGetMeshPoint::CRhGetMeshPoint(const ON_Mesh* mesh, const ON_Plane& object_plane)
  : m_mesh(mesh)
{
  m_object_plane = object_plane;
}

BOOL CRhGetMeshPoint::CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform)
{
  UNREFERENCED_PARAMETER(vp);

  BOOL rc = FALSE;
  ON_MESH_POINT point;
  if (0 != m_mesh && m_mesh->GetClosestPoint(pt, &point))
  {
    ON_Plane face_plane;
    if (MeshFacePlane(point, face_plane))
    {
      xform.Rotation(m_object_plane, face_plane);
      rc = xform.IsValid() ? TRUE : FALSE;
    }
  }
  return rc;
}

bool CRhGetMeshPoint::MeshFacePlane(ON_MESH_POINT point, ON_Plane& plane)
{
  bool rc = false;
  if (point.m_ci.m_type == ON_COMPONENT_INDEX::mesh_face)
  {
    if (0 != point.m_mesh && 0 <= point.m_ci.m_index && point.m_ci.m_index < point.m_mesh->FaceCount())
    {
      if (m_face_planes.Lookup(point.m_ci.m_index, plane))
        rc = true;
      else
      {
        ON_3dVector normal = ON_UNSET_POINT;
        if (point.m_mesh->HasFaceNormals())
          normal = point.m_mesh->m_FN[point.m_ci.m_index];
        else
        {
          if (point.m_mesh->HasDoublePrecisionVertices())
            point.m_mesh->m_F[point.m_ci.m_index].ComputeFaceNormal(point.m_mesh->DoublePrecisionVertices().Array(), normal);
          else
            point.m_mesh->m_F[point.m_ci.m_index].ComputeFaceNormal(point.m_mesh->m_V.Array(), normal);
        }

        if (normal.IsValid())
        {
          plane = ON_Plane(point.m_P, normal);
          m_face_planes.SetAt(point.m_ci.m_index, plane);
          rc = true;
        }
      }
    }
  }
  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleOrientOnMesh : public CRhinoCommand
{
public:
  CCommandSampleOrientOnMesh() {}
  ~CCommandSampleOrientOnMesh() {}
  UUID CommandUUID()
  {
    // {986D21C4-BC03-4E05-B530-668F7A8EE420}
    static const GUID SampleOrientOnMeshCommand_UUID =
    { 0x986D21C4, 0xBC03, 0x4E05, { 0xB5, 0x30, 0x66, 0x8F, 0x7A, 0x8E, 0xE4, 0x20 } };
    return SampleOrientOnMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleOrientOnMesh"; }
  const wchar_t* LocalCommandName() const { return L"SampleOrientOnMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleOrientOnMesh object
static class CCommandSampleOrientOnMesh theSampleOrientOnMeshCommand;

CRhinoCommand::result CCommandSampleOrientOnMesh::RunCommand(const CRhinoCommandContext& context)
{
  // Select objects to orient
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to orient");
  go.EnableSubObjectSelect(FALSE);
  go.EnableIgnoreGrips(true);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Point to move from
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point to move from");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  // Calculate source plane
  CRhinoView* view = gp.View();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_Plane plane = view->ActiveViewport().ConstructionPlane().m_plane;
  plane.SetOrigin(gp.Point());

  // Mesh to orient on
  CRhinoGetObject gm;
  gm.SetCommandPrompt(L"Mesh to orient on");
  gm.SetGeometryFilter(CRhinoGetObject::mesh_object);
  gm.EnablePreSelect(FALSE);
  gm.EnableDeselectAllBeforePostSelect(false);
  gm.GetObjects(1, 1);
  if (gm.CommandResult() != CRhinoCommand::success)
    return gm.CommandResult();

  const ON_Mesh* mesh = gm.Object(0).Mesh();
  if (0 == mesh)
    return CRhinoCommand::failure;

  // Point on mesh to orient to
  CRhGetMeshPoint gmp(mesh, plane);
  gmp.SetCommandPrompt(L"Point on mesh to orient to");
  gmp.AppendObjects(go);
  gmp.GetXform();
  if (gmp.CommandResult() != CRhinoCommand::success)
    return gmp.CommandResult();

  // One final calculation
  view = gmp.View();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_Xform xform;
  if (gmp.CalculateTransform(view->ActiveViewport(), gmp.Point(), xform))
  {
    for (int i = 0; i < go.ObjectCount(); i++)
    {
      CRhinoObjRef obj_ref = go.Object(i);
      context.m_doc.TransformObject(obj_ref, xform, false);
    }
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleOrientOnMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
