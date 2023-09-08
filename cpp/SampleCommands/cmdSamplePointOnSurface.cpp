#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN CGetSurfacePoint class
//

class CGetSurfacePoint : public CRhinoGetPoint
{
public:
  CGetSurfacePoint(const ON_BrepFace& face);
  ~CGetSurfacePoint();

  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const ON_2iPoint* pt2d) override;
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint&) override;

private:
  void DeleteCurves();

private:
  const ON_BrepFace& m_face;
  ON_SimpleArray<ON_Curve*> m_curves;
};

CGetSurfacePoint::CGetSurfacePoint(const ON_BrepFace& face)
  : m_face(face)
{
  Constrain(m_face);
  m_curves.SetCapacity(4);
}

CGetSurfacePoint::~CGetSurfacePoint()
{
  DeleteCurves();
}

void CGetSurfacePoint::DeleteCurves()
{
  for (int i = 0; i < m_curves.Count(); i++)
  {
    if (nullptr != m_curves[i])
      delete m_curves[i];
    m_curves[i] = nullptr;
  }
  m_curves.Empty();
}

void CGetSurfacePoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const ON_2iPoint* pt2d)
{
  DeleteCurves();

  double u = ON_UNSET_VALUE, v = ON_UNSET_VALUE;
  const ON_Surface* srf = PointOnSurface(&u, &v);
  if (nullptr != srf)
  {
    RhinoGetIsoCurves(m_face, 0, v, m_curves);
    RhinoGetIsoCurves(m_face, 1, u, m_curves);
  }

  CRhinoGetPoint::OnMouseMove(vp, flags, pt, pt2d);
}

void CGetSurfacePoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  for (int i = 0; i < m_curves.Count(); i++)
  {
    if (nullptr != m_curves[i])
      dp.DrawCurve(*m_curves[i]);
  }

  CRhinoGetPoint::DynamicDraw(dp, pt);
}

//
// END CGetSurfacePoint class
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePointOnSurface command
//

#pragma region SamplePointOnSurface command

class CCommandSamplePointOnSurface : public CRhinoCommand
{
public:
  CCommandSamplePointOnSurface() = default;
  UUID CommandUUID() override
  {
    // {720FEA17-D4BE-4EFA-8C92-25834029A503}
    static const GUID SamplePointOnSurfaceCommand_UUID =
    { 0x720FEA17, 0xD4BE, 0x4EFA,{ 0x8C, 0x92, 0x25, 0x83, 0x40, 0x29, 0xA5, 0x03 } };
    return SamplePointOnSurfaceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePointOnSurface"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSamplePointOnSurface object
static class CCommandSamplePointOnSurface theSamplePointOnSurfaceCommand;

CRhinoCommand::result CCommandSamplePointOnSurface::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(TRUE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const CRhinoObject* obj = obj_ref.Object();
  const ON_BrepFace* face = obj_ref.Face();
  if (nullptr == obj || nullptr == face)
    return CRhinoCommand::failure;

  obj->Select(false);

  CGetSurfacePoint gp(*face);
  gp.SetCommandPrompt(L"Pick location on surface");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint point = gp.Point();

  ON_wString point_str;
  RhinoFormatPoint(point, point_str);
  RhinoApp().Print(L"Surface point = %s\n", static_cast<const wchar_t*>(point_str));

  doc->AddPointObject(point);
  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePointOnSurface command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
