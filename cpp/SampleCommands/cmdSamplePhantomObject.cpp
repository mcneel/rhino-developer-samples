#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePhantomObject command
//

#pragma region SamplePhantomObject command

class CSamplePhantomObject : public CRhinoPhantomObject
{
public:
  CSamplePhantomObject()
  {
    ON_3fPoint v[] =
    {
      ON_3fPoint(0.0, 0.0, 0.0),
      ON_3fPoint(1.0, 0.0, 0.0),
      ON_3fPoint(1.0, 1.0, 0.0),
      ON_3fPoint(0.0, 1.0, 0.0),
      ON_3fPoint(0.0, 0.0, 1.0),
      ON_3fPoint(1.0, 0.0, 1.0),
      ON_3fPoint(1.0, 1.0, 1.0),
      ON_3fPoint(0.0, 1.0, 1.0)
    };

    m_mesh.m_V.Append(_countof(v), v);

    ON_MeshFace f[] =
    {
      {0, 1, 2, 3},
      {0, 1, 1 + 4, 0 + 4},
      {1, 2, 2 + 4, 1 + 4},
      {2, 3, 3 + 4, 2 + 4},
      {3, 0, 0 + 4, 3 + 4},
      {7, 6, 5, 4}
    };

    m_mesh.m_F.Append(_countof(f), f);

    m_mesh.ComputeVertexNormals();
    m_mesh.ComputeFaceNormals();

    m_geometry = &m_mesh;
    m_geomem_type = 1;
  }

  ~CSamplePhantomObject()
  {
    m_geometry = NULL;
    m_geomem_type = 0;
  }

  virtual	ON_BoundingBox BoundingBox() const
  {
    if (!m_bbox.IsValid())
      const_cast<CSamplePhantomObject*>(this)->m_bbox = m_mesh.BoundingBox();
    return m_bbox;
  }

  virtual	ON_BoundingBox BoundingBox(const CRhinoView*) const
  {
    return BoundingBox();
  }

  virtual	void Draw(CRhinoDisplayPipeline& dp) const
  {
    dp.DrawWireframeMesh(m_mesh, Attributes().DrawColor(Document(), dp.GetRhinoVP()->ViewportId()), false);
  }

  virtual	bool IsMeshable(ON::mesh_type mesh_type) const
  {
    UNREFERENCED_PARAMETER(mesh_type);
    return true;
  }

  virtual int MeshCount(ON::mesh_type mesh_type, const ON_MeshParameters* mp) const
  {
    UNREFERENCED_PARAMETER(mesh_type);
    UNREFERENCED_PARAMETER(mp);
    return 1;
  }

  virtual	int	CreateMeshes(ON::mesh_type mesh_type, const ON_MeshParameters& mp, bool bIgnoreCustom)
  {
    UNREFERENCED_PARAMETER(mesh_type);
    UNREFERENCED_PARAMETER(mp);
    UNREFERENCED_PARAMETER(bIgnoreCustom);
    return 1;
  }

  virtual	void DestroyMeshes(ON::mesh_type mesh_type, bool bDeleteMeshes)
  {
    UNREFERENCED_PARAMETER(mesh_type);
    UNREFERENCED_PARAMETER(bDeleteMeshes);
  }

  virtual	int	GetMeshes(ON::mesh_type mesh_type, ON_SimpleArray<const ON_Mesh*>& meshes) const
  {
    UNREFERENCED_PARAMETER(mesh_type);
    meshes.Append(&m_mesh);
    return 1;
  }

public:
  ON_Mesh m_mesh;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSamplePhantomObject : public CRhinoCommand
{
public:
  CCommandSamplePhantomObject() {}
  ~CCommandSamplePhantomObject() {}
  UUID CommandUUID()
  {
    // {73E99022-5A65-4081-BDE9-7C70C4A88706}
    static const GUID SamplePhantomObjectCommand_UUID =
    { 0x73E99022, 0x5A65, 0x4081, { 0xBD, 0xE9, 0x7C, 0x70, 0xC4, 0xA8, 0x87, 0x06 } };
    return SamplePhantomObjectCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePhantomObject"; }
  const wchar_t* LocalCommandName() const { return L"SamplePhantomObject"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePhantomObject object
static class CCommandSamplePhantomObject theSamplePhantomObjectCommand;

CRhinoCommand::result CCommandSamplePhantomObject::RunCommand(const CRhinoCommandContext& context)
{
  CSamplePhantomObject* phantom = new CSamplePhantomObject();
  context.m_doc.AddObject(phantom);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePhantomObject command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
