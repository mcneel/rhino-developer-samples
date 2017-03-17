#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSurfaceNormal command
//

class CGetSurfaceNormal : public CRhinoGetPoint
{
public:
  CGetSurfaceNormal(const ON_BrepFace& face);

  bool CalculateSurfaceNormal(const ON_3dPoint& pt);

  // CRhinoGetPoint overrides
  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* pt2d);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

  ON_3dVector SurfaceNormal() const;

protected:
  const ON_BrepFace& m_face;
  ON_3dVector m_normal;
  bool m_bDraw;
};

CGetSurfaceNormal::CGetSurfaceNormal(const ON_BrepFace& face)
  : m_face(face)
{
  m_normal = ON_3dVector::ZAxis; // some default
  m_bDraw = false;
}

bool CGetSurfaceNormal::CalculateSurfaceNormal(const ON_3dPoint& pt)
{
  bool rc = false;

  // Get the parameters of the point on the
  // surface that is closest to pt
  double u, v;
  if (m_face.GetClosestPoint(pt, &u, &v))
  {
    // Get the surface normal
    m_normal = m_face.NormalAt(u, v);
    // If the face orientation is opposite of 
    // the natural surface orientation, then
    // reverse the direction of the vector.
    if (m_face.m_bRev)
      m_normal.Reverse();
    rc = true;
  }

  return rc;
}

void CGetSurfaceNormal::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* pt2d)
{
  m_bDraw = CalculateSurfaceNormal(pt);

  CRhinoGetPoint::OnMouseMove(vp, flags, pt, pt2d);
}

void CGetSurfaceNormal::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  if (m_bDraw)
    dp.DrawDirectionArrow(pt, m_normal);

  CRhinoGetPoint::DynamicDraw(dp, pt);
}

ON_3dVector CGetSurfaceNormal::SurfaceNormal() const
{
  return m_normal;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleSurfaceNormal : public CRhinoCommand
{
public:
  CCommandSampleSurfaceNormal() {}
  ~CCommandSampleSurfaceNormal() {}
  UUID CommandUUID()
  {
    // {83F51E52-10CB-419B-BE7D-39C7DB6EB8C9}
    static const GUID SampleSurfaceNormalCommand_UUID =
    { 0x83F51E52, 0x10CB, 0x419B, { 0xBE, 0x7D, 0x39, 0xC7, 0xDB, 0x6E, 0xB8, 0xC9 } };
    return SampleSurfaceNormalCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSurfaceNormal"; }
  const wchar_t* LocalCommandName() { return L"SampleSurfaceNormal"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSurfaceNormal object
static class CCommandSampleSurfaceNormal theSampleSurfaceNormalCommand;

CRhinoCommand::result CCommandSampleSurfaceNormal::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  // Select a surface
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Get the selected face
  const CRhinoObjRef& ref = go.Object(0);
  const ON_BrepFace* face = ref.Face();
  if (0 == face)
    return CRhinoCommand::failure;

  // Pick a point on the surface. Constrain
  // picking to the face.
  CGetSurfaceNormal gp(*face);
  gp.SetCommandPrompt(L"Select point on surface");
  gp.Constrain(*face);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint pt = gp.Point();
  if (gp.CalculateSurfaceNormal(pt))
  {
    ON_3dVector normal = gp.SurfaceNormal();
    // TODO...
  }

  return CRhinoCommand::success;
}

//
// END SampleSurfaceNormal command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
