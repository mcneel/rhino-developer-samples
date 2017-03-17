#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePullCurveToMesh command
//

#pragma region SamplePullCurveToMesh command

class CCommandSamplePullCurveToMesh : public CRhinoCommand
{
public:
  CCommandSamplePullCurveToMesh() {}
  ~CCommandSamplePullCurveToMesh() {}
  UUID CommandUUID()
  {
    // {A3C5DF7E-8F4A-46F6-A4A1-588D58741675}
    static const GUID SamplePullCurveToMeshCommand_UUID =
    { 0xA3C5DF7E, 0x8F4A, 0x46F6,{ 0xA4, 0xA1, 0x58, 0x8D, 0x58, 0x74, 0x16, 0x75 } };
    return SamplePullCurveToMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePullCurveToMesh"; }
  const wchar_t* LocalCommandName() const { return L"SamplePullCurveToMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePullCurveToMesh object
static class CCommandSamplePullCurveToMesh theSamplePullCurveToMeshCommand;

CRhinoCommand::result CCommandSamplePullCurveToMesh::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select open mesh");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::open_mesh);
  go.GetObjects(1, 1);
  if (go.CommandResult() != success)
    return go.CommandResult();

  const CRhinoObjRef& ref = go.Object(0);
  const ON_Mesh* mesh = ref.Mesh();
  if (0 == mesh)
    return failure;

  CRhinoGetObject go1;
  go1.SetCommandPrompt(L"Select curve on mesh");
  go1.SetGeometryFilter(CRhinoGetObject::curve_object);
  go1.GetObjects(1, 1);
  if (go1.CommandResult() != CRhinoCommand::success)
    return go1.CommandResult();

  const ON_Curve* crv = go1.Object(0).Curve();
  if (0 == crv)
    return CRhinoCommand::failure;

  CRhinoPolylineOnMeshUserData ud;
  ON_PolylineCurve* polyline = RhinoPullCurveToMesh(crv, mesh, context.m_doc.AbsoluteTolerance(), &ud);

  for (int i = 0; i < ud.m_P.Count(); i++)
  {
    switch (ud.m_P[i].m_ci.m_type)
    {
    case ON_COMPONENT_INDEX::mesh_face:
      RhinoApp().Print(L"%d = ON_COMPONENT_INDEX::mesh_face\n", i);
      break;
    case ON_COMPONENT_INDEX::meshtop_edge:
      RhinoApp().Print(L"%d = ON_COMPONENT_INDEX::meshtop_edge\n", i);
      break;
    case ON_COMPONENT_INDEX::meshtop_vertex:
      RhinoApp().Print(L"%d = ON_COMPONENT_INDEX::meshtop_vertex\n", i);
      break;
    }

    context.m_doc.AddPointObject(ud.m_P[i].m_P);

    ON_wString wstr;
    wstr.Format(L"%d", i);

    ON_TextDot dot(ud.m_P[i].m_P, wstr, 0);
    CRhinoTextDot* obj = new CRhinoTextDot();
    obj->SetDot(dot);

    context.m_doc.AddObject(obj);
  }

  if (0 != polyline)
    delete polyline; // Don't leak...

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePullCurveToMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
