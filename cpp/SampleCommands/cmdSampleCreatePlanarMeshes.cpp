#include "stdafx.h"

/*
Description:
  Meshes a closed, planar polyline.
*/
bool RhMeshPolyline(int closed_point_count, const ON_3dPoint* points, ON_Mesh* mesh)
{
  bool rc = false;

  const int open_point_count = closed_point_count - 1;

  if (closed_point_count > 2 && nullptr != points)
  {
    if (nullptr == mesh)
      mesh = new ON_Mesh();
    else
      mesh->Destroy();

    int* triangles = (int*)onmalloc((open_point_count - 2) * sizeof(int) * 3);
    memset(triangles, 0, (open_point_count - 2) * sizeof(int) * 3);

    RhinoTriangulate3dPolygon(open_point_count, 3, (const double*)points, 3, triangles);

    mesh->m_dV.SetCapacity(open_point_count);
    mesh->m_V.SetCapacity(open_point_count);
    for (int i = 0; i < open_point_count; i++)
      mesh->SetVertex(i, points[i]);

    const int tri_count = open_point_count - 2;

    mesh->m_F.SetCapacity(tri_count);
    for (int i = 0; i < tri_count; i++)
    {
      ON_MeshFace& face = mesh->m_F.AppendNew();
      face.vi[0] = triangles[i * 3];
      face.vi[1] = triangles[i * 3 + 1];
      face.vi[2] = triangles[i * 3 + 2];
      face.vi[3] = face.vi[2];
    }

    onfree(triangles); // Don't leak...

    rc = mesh->IsValid();
    if (!rc)
    {
      delete mesh;
      mesh = nullptr;
    }
  }

  return rc;
}

/*
Description:
  Creates planar meshes from a list of polyline curves.
*/
int RhCreatePlanarMeshes(
  const ON_SimpleArray<const ON_PolylineCurve*>& curves,
  ON_SimpleArray<ON_Mesh*>& meshes
)
{
  const int curve_count = curves.Count();
  if (0 == curve_count)
    return 0;

  for (int i = 0; i < curve_count; i++)
  {
    const ON_PolylineCurve* curve = curves[i];
    if (nullptr != curve && curve->IsClosed() && curve->m_pline.Count() >= 4)
    {
      const int point_count = curve->m_pline.Count();
      if (point_count >= 4)
      {
        ON_Mesh* mesh = new ON_Mesh();

        if (4 == point_count)
        {
          // Add triangle
          mesh->SetVertex(0, curve->m_pline[0]);
          mesh->SetVertex(1, curve->m_pline[1]);
          mesh->SetVertex(2, curve->m_pline[2]);
          ON_MeshFace& face = mesh->m_F.AppendNew();
          face.vi[0] = 0;
          face.vi[1] = 1;
          face.vi[2] = face.vi[3] = 2;
        }
        else if (5 == point_count)
        {
          // Add quad
          mesh->SetVertex(0, curve->m_pline[0]);
          mesh->SetVertex(1, curve->m_pline[1]);
          mesh->SetVertex(2, curve->m_pline[2]);
          mesh->SetVertex(3, curve->m_pline[3]);
          ON_MeshFace& face = mesh->m_F.AppendNew();
          face.vi[0] = 0;
          face.vi[1] = 1;
          face.vi[2] = 2;
          face.vi[3] = 3;
        }
        else
        {
          // Add ngon
          bool rc = RhMeshPolyline(point_count, curve->m_pline.Array(), mesh);
          if (rc && nullptr != mesh)
          {
            ON_SimpleArray<unsigned int> vertices;
            for (int vi = 0; vi < mesh->m_V.Count(); vi++)
              vertices.Append(vi);

            ON_SimpleArray<unsigned int> face_indices;
            for (int fi = 0; fi < mesh->m_F.Count(); fi++)
              face_indices.Append(fi);

            mesh->AddNgon(vertices.Count(), vertices.Array(), face_indices.Count(), face_indices.Array());
          }
        }

        if (nullptr != mesh)
        {
          mesh->ComputeVertexNormals();
          mesh->Compact();
          if (mesh->IsValid())
            meshes.Append(mesh);
          else
            delete mesh;
        }
      }
    }
  }

  return meshes.Count();
}

/*
Description:
  Limits selectiion to closed planar polylines.
*/
class CRhinoGetClosedPlanarPolyline : public CRhinoGetObject
{
public:
  CRhinoGetClosedPlanarPolyline(double tolerance)
    :m_tolerance(tolerance)
  {
  }

  // virtual override
  bool CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
  ) const override
  {
    const ON_PolylineCurve* polyline_curve = ON_PolylineCurve::Cast(geometry);
    if (
      nullptr != polyline_curve &&
      polyline_curve->IsClosed() &&
      polyline_curve->IsPlanar(nullptr, m_tolerance)
      )
      return true;
    return false;
  }

private:
  double m_tolerance;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCreatePlanarMeshes command
//

#pragma region SampleCreatePlanarMeshes command

class CCommandSampleCreatePlanarMeshes : public CRhinoCommand
{
public:
  CCommandSampleCreatePlanarMeshes() = default;
  UUID CommandUUID() override
  {
    // {C76A7061-8CF3-484D-A46B-07CBD3A46D06}
    static const GUID SampleCreatePlanarMeshesCommand_UUID =
    { 0xC76A7061, 0x8CF3, 0x484D,{ 0xA4, 0x6B, 0x07, 0xCB, 0xD3, 0xA4, 0x6D, 0x06 } };
    return SampleCreatePlanarMeshesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCreatePlanarMeshes"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCreatePlanarMeshes object
static class CCommandSampleCreatePlanarMeshes theSampleCreatePlanarMeshesCommand;

CRhinoCommand::result CCommandSampleCreatePlanarMeshes::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetClosedPlanarPolyline go(context.m_doc.AbsoluteTolerance());
  go.SetCommandPrompt(L"Select closed, planar polylines to mesh");
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  ON_SimpleArray<const ON_PolylineCurve*> curves(go.ObjectCount());
  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const ON_PolylineCurve* polyline_curve = ON_PolylineCurve::Cast(go.Object(i).Curve());
    if (nullptr == polyline_curve)
      return CRhinoCommand::failure;

    curves.Append(polyline_curve);
  }

  ON_SimpleArray<ON_Mesh*> meshes;
  RhCreatePlanarMeshes(curves, meshes);
  for (int i = 0; i < meshes.Count(); i++)
  {
    CRhinoMeshObject* mesh_obj = new CRhinoMeshObject();
    mesh_obj->SetMesh(meshes[i]);
    context.m_doc.AddObject(mesh_obj);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCreatePlanarMeshes command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
