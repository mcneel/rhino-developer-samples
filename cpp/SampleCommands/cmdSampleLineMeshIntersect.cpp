#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLineMeshIntersect command
//

#pragma region SampleLineMeshIntersect command

class CCommandSampleLineMeshIntersect : public CRhinoCommand
{
public:
  CCommandSampleLineMeshIntersect() {}
  ~CCommandSampleLineMeshIntersect() {}
  UUID CommandUUID()
  {
    // {6353A345-E93D-4863-B574-185FB17059E2}
    static const GUID SampleLineMeshIntersectCommand_UUID =
    { 0x6353A345, 0xE93D, 0x4863, { 0xB5, 0x74, 0x18, 0x5F, 0xB1, 0x70, 0x59, 0xE2 } };
    return SampleLineMeshIntersectCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleLineMeshIntersect"; }
  const wchar_t* LocalCommandName() const { return L"SampleLineMeshIntersect"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleLineMeshIntersect object
static class CCommandSampleLineMeshIntersect theSampleLineMeshIntersectCommand;

CRhinoCommand::result CCommandSampleLineMeshIntersect::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject gm;
  gm.SetCommandPrompt(L"Select mesh to intersect");
  gm.SetGeometryFilter(CRhinoGetObject::mesh_object);
  gm.GetObjects(1, 1);
  if (gm.CommandResult() != CRhinoCommand::success)
    return gm.CommandResult();

  const ON_Mesh* mesh = gm.Object(0).Mesh();
  if (0 == mesh)
    return CRhinoCommand::failure;

  CRhinoGetObject gl;
  gl.SetCommandPrompt(L"Select line to intersect with");
  gl.SetGeometryFilter(CRhinoGetObject::curve_object);
  gl.SetGeometryAttributeFilter(CRhinoGetObject::open_curve);
  gl.EnablePreSelect(FALSE);
  gl.EnableDeselectAllBeforePostSelect(FALSE);
  gl.GetObjects(1, 1);
  if (gl.CommandResult() != CRhinoCommand::success)
    return gl.CommandResult();

  const ON_Curve* curve = gl.Object(0).Curve();
  if (0 == curve)
    return CRhinoCommand::failure;

  const ON_LineCurve* line_curve = ON_LineCurve::Cast(curve);
  if (0 == line_curve)
  {
    RhinoApp().Print(L"Not a line curve.\n");
    return CRhinoCommand::nothing;
  }

  ON_3dPointArray points;
  points.Append(line_curve->m_line.from);
  points.Append(line_curve->m_line.to);

  const ON_MeshTree* mesh_tree = mesh->MeshTree(true);
  if (mesh_tree)
  {
    ON_SimpleArray<ON_CMX_EVENT> cmx;
    if (mesh_tree->IntersectPolyline(2, points.Array(), cmx))
    {
      for (int i = 0; i < cmx.Count(); i++)
      {
        RhinoApp().Print(L"Intesection found at face index = %d.\n", cmx[i].m_M[0].m_face_index);
        CRhinoPointObject* point_object = context.m_doc.AddPointObject(cmx[i].m_M[0].m_P);
        if (point_object)
          point_object->Select();
      }
      context.m_doc.Redraw();
    }

    if (1 == cmx.Count())
      RhinoApp().Print(L"1 intesection found.\n");
    else
      RhinoApp().Print(L"%d intesections found.\n", cmx.Count());
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleLineMeshIntersect command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
