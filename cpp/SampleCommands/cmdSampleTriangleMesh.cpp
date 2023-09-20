#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTriangleMesh command
//

class CTriangle
{
public:
  CTriangle();
  CTriangle(ON_3dPoint a, ON_3dPoint b, ON_3dPoint c);
  ON_3dPoint operator[](int) const;
  bool IsValid() const;
public:
  ON_3dPoint m_a[3];
};

CTriangle::CTriangle()
{
  m_a[0] = m_a[1] = m_a[2] = ON_3dPoint::UnsetPoint;
}

CTriangle::CTriangle(ON_3dPoint a, ON_3dPoint b, ON_3dPoint c)
{
  m_a[0] = a; m_a[1] = b; m_a[2] = c;
}

ON_3dPoint CTriangle::operator[](int i) const
{
  return (i <= 0) ? m_a[0] : ((i >= 2) ? m_a[2] : m_a[1]);
}

bool CTriangle::IsValid() const
{
  return m_a[0].IsValid() && m_a[1].IsValid() && m_a[2].IsValid();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

ON_Mesh* CreateMeshFromTriangles(const ON_SimpleArray<CTriangle>& triangles)
{
  const int triangle_count = triangles.Count();
  if (0 == triangle_count)
    return nullptr;

  ON_Mesh* mesh = new ON_Mesh(triangle_count, triangle_count * 3, true, false);

  int vi = 0, fi = 0;
  for (int i = 0; i < triangle_count; i++)
  {
    for (int j = 0; j < 3; j++)
      mesh->SetVertex(vi++, triangles[i][j]);
    mesh->SetTriangle(fi++, vi - 3, vi - 2, vi - 1);
  }

  mesh->ComputeVertexNormals();
  mesh->Compact();

  if (!mesh->IsValid())
  {
    delete mesh;
    mesh = nullptr;
  }

  return mesh;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleTriangleMesh : public CRhinoCommand
{
public:
  CCommandSampleTriangleMesh() = default;
  UUID CommandUUID() override
  {
    // {9C0155A9-5F97-459A-A3F3-E818DD4BA9AA}
    static const GUID SampleTriangleMeshCommand_UUID =
    { 0x9C0155A9, 0x5F97, 0x459A, { 0xA3, 0xF3, 0xE8, 0x18, 0xDD, 0x4B, 0xA9, 0xAA } };
    return SampleTriangleMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleTriangleMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleTriangleMesh object
static class CCommandSampleTriangleMesh theSampleTriangleMeshCommand;

CRhinoCommand::result CCommandSampleTriangleMesh::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  const double d = sqrt(2.0) / 2.0;
  ON_3dPoint p0 = ON_3dPoint(0.0, 0.0, d);
  ON_3dPoint p1 = ON_3dPoint(0.0, 0.0, -d);
  ON_3dPoint p2 = ON_3dPoint(d, 0.0, 0.0);
  ON_3dPoint p3 = ON_3dPoint(-d, 0.0, 0.0);
  ON_3dPoint p4 = ON_3dPoint(0.0, d, 0.0);
  ON_3dPoint p5 = ON_3dPoint(0.0, -d, 0.0);

  ON_SimpleArray<CTriangle> triangles(8);
  triangles.Append(CTriangle(p0, p2, p4));
  triangles.Append(CTriangle(p0, p4, p3));
  triangles.Append(CTriangle(p0, p3, p5));
  triangles.Append(CTriangle(p0, p5, p2));
  triangles.Append(CTriangle(p1, p2, p5));
  triangles.Append(CTriangle(p1, p5, p3));
  triangles.Append(CTriangle(p1, p3, p4));
  triangles.Append(CTriangle(p1, p4, p2));

  ON_Mesh* mesh = CreateMeshFromTriangles(triangles);
  if (mesh)
  {
    CRhinoMeshObject* mesh_obj = new CRhinoMeshObject();
    mesh_obj->SetMesh(mesh);
    mesh = nullptr;
    doc->AddObject(mesh_obj);
    doc->Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleTriangleMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
