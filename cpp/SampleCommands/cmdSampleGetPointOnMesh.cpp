#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetPointOnMesh command
//

#pragma region SampleGetPointOnMesh command

class CCommandSampleGetPointOnMesh : public CRhinoCommand
{
public:
  CCommandSampleGetPointOnMesh() {}
  ~CCommandSampleGetPointOnMesh() {}
  UUID CommandUUID()
  {
    // {1DCF993-BDEF-4152-8DA0-7437AA74CED8}
    static const GUID SampleGetPointOnMeshCommand_UUID =
    { 0x1DCF993, 0xBDEF, 0x4152, { 0x8D, 0xA0, 0x74, 0x37, 0xAA, 0x74, 0xCE, 0xD8 } };
    return SampleGetPointOnMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGetPointOnMesh"; }
  const wchar_t* LocalCommandName() const { return L"SampleGetPointOnMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleGetPointOnMesh object
static class CCommandSampleGetPointOnMesh theSampleGetPointOnMeshCommand;

CRhinoCommand::result CCommandSampleGetPointOnMesh::RunCommand(const CRhinoCommandContext& context)
{
  // Pick a mesh
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Validate the pick
  const CRhinoMeshObject* mesh_object = CRhinoMeshObject::Cast(go.Object(0).Object());
  if (0 == mesh_object)
    return CRhinoCommand::failure;

  // Pick a point on the mesh
  ON_MESH_POINT point;
  int rc = RhinoGetPointOnMesh(mesh_object, L"Point on mesh", FALSE, point);
  if (rc != 0)
    return CRhinoCommand::cancel;

  // Add the picked point and print results
  context.m_doc.AddPointObject(point.m_P);
  RhinoApp().Print(L"Added point on face %d, with %g, %g, %g, %g as barycentric coordinates.\n",
    point.m_face_index,
    point.m_t[0],
    point.m_t[1],
    point.m_t[2],
    point.m_t[3]
  );

  // Was the pick on a face?
  if (point.m_ci.m_type == ON_COMPONENT_INDEX::mesh_face)
  {
    // Validate mesh
    if (point.m_mesh)
    {
      ON_3dVector normal = ON_UNSET_POINT;

      // Does the mesh have face normals?
      if (point.m_mesh->HasFaceNormals())
      {
        // Get the face normal
        normal = point.m_mesh->m_FN[point.m_ci.m_index];
      }
      else
      {
        // Compute the face normal
        if (point.m_mesh->HasDoublePrecisionVertices())
          point.m_mesh->m_F[point.m_ci.m_index].ComputeFaceNormal(point.m_mesh->DoublePrecisionVertices().Array(), normal);
        else
          point.m_mesh->m_F[point.m_ci.m_index].ComputeFaceNormal(point.m_mesh->m_V.Array(), normal);
      }

      // Validate normal
      if (normal.IsValid())
      {
        // Add normal line, with arrow, and print results
        ON_Line line(point.m_P, point.m_P + normal);

        ON_3dmObjectAttributes attributes;
        context.m_doc.GetDefaultObjectAttributes(attributes);
        attributes.m_object_decoration = ON::end_arrowhead;

        context.m_doc.AddCurveObject(line, &attributes);
        context.m_doc.Redraw();

        ON_wString normal_str;
        RhinoFormatPoint(normal, normal_str);
        RhinoApp().Print(L"Added line normal to face %d, with % as normal direction.\n", point.m_ci.m_index, normal_str);
      }
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGetPointOnMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
