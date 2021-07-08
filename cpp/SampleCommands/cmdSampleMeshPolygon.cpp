#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshPolygon command
//

#pragma region SampleMeshPolygon command

class CCommandSampleMeshPolygon : public CRhinoCommand
{
public:
  CCommandSampleMeshPolygon() = default;
  UUID CommandUUID() override
  {
    // {DF9E975F-56FD-4DE1-B448-A61863AB7ECC}
    static const GUID SampleMeshPolygonCommand_UUID =
    { 0xDF9E975F, 0x56FD, 0x4DE1, { 0xB4, 0x48, 0xA6, 0x18, 0x63, 0xAB, 0x7E, 0xCC } };
    return SampleMeshPolygonCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMeshPolygon"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMeshPolygon object
static class CCommandSampleMeshPolygon theSampleMeshPolygonCommand;

CRhinoCommand::result CCommandSampleMeshPolygon::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select closed polyline to mesh");
  go.SetGeometryFilter(ON::curve_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::closed_curve);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curve = go.Object(0).Curve();
  if (nullptr == curve || !curve->IsClosed())
    return CRhinoCommand::failure;

  ON_SimpleArray<ON_3dPoint> points;
  if (0 == curve->IsPolyline(&points, nullptr))
    return CRhinoCommand::failure;

  context.m_doc.UnselectAll();

  if (points[0] == points[points.Count() - 1])
    points.SetCount(points.Count() - 1);

  const int point_count = points.Count();
  ON_SimpleArray<int> triangles((point_count - 2) * 3);
  triangles.SetCount(triangles.Capacity());

  int rc = RhinoTriangulate3dPolygon(point_count, 3, (const double*)points.Array(), 3, triangles.Array());
  if (rc != 0)
    return CRhinoCommand::failure;

  const int triangle_count = triangles.Count() / 3;
  ON_Mesh* mesh = new ON_Mesh(triangle_count, point_count, false, false);
  mesh->m_dV.Append(point_count, points.Array());
  mesh->UpdateSinglePrecisionVertices();

  for (int i = 0; i < triangle_count; i++)
  {
    ON_MeshFace& face = mesh->m_F.AppendNew();
    face.vi[0] = triangles[3 * i];
    face.vi[1] = triangles[(3 * i) + 1];
    face.vi[2] = face.vi[3] = triangles[(3 * i) + 2];
  }
  mesh->ComputeVertexNormals();

  if (!mesh->IsValid())
  {
    delete mesh;
    return CRhinoCommand::failure;
  }

  CRhinoMeshObject* mesh_obj = new CRhinoMeshObject();
  mesh_obj->SetMesh(mesh);
  context.m_doc.AddObject(mesh_obj);

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}
#pragma endregion

//
// END SampleMeshPolygon command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
