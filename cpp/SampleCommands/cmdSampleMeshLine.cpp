#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshLine command
//

class CCommandSampleMeshLine : public CRhinoCommand
{
public:
  CCommandSampleMeshLine() {}
  ~CCommandSampleMeshLine() {}
  UUID CommandUUID()
  {
    // {84BD3AC1-A648-4CF2-875F-177EAB6367D4}
    static const GUID SampleMeshLineCommand_UUID =
    { 0x84BD3AC1, 0xA648, 0x4CF2, { 0x87, 0x5F, 0x17, 0x7E, 0xAB, 0x63, 0x67, 0xD4 } };
    return SampleMeshLineCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshLine"; }
  const wchar_t* LocalCommandName() { return L"SampleMeshLine"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshLine object
static class CCommandSampleMeshLine theSampleMeshLineCommand;

CRhinoCommand::result CCommandSampleMeshLine::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoMeshObject* mesh_obj = CRhinoMeshObject::Cast(go.Object(0).Object());
  if (0 == mesh_obj)
    return CRhinoCommand::failure;

  ON_MESH_POINT start_point, end_point;

  int rc = RhinoGetPointOnMesh(mesh_obj, L"First point on mesh", true, start_point);
  if (rc == 0)
  {
    rc = RhinoGetPointOnMesh(mesh_obj, L"Second point on mesh", true, end_point);
    if (rc == 0)
    {
      ON_LineCurve line_crv(start_point.m_P, end_point.m_P);
      double tolerance = context.m_doc.AbsoluteTolerance();
      ON_PolylineCurve* polyline_crv = RhinoPullCurveToMesh(&line_crv, mesh_obj->Mesh(), tolerance);
      if (polyline_crv)
      {
        context.m_doc.AddCurveObject(*polyline_crv);
        delete polyline_crv; // Don't leak...
        context.m_doc.Redraw();
      }
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleMeshLine command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
