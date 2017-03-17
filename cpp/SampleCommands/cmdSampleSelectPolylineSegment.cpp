#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSelectPolylineSegment command
//

#pragma region SampleSelectPolylineSegment command

class CRhGetPolylineSegment : public CRhinoGetPoint
{
public:
  CRhGetPolylineSegment(const ON_PolylineCurve* curve);

  bool CalculatePolylineSegment(const ON_3dPoint& pt, int& index0, int& index1);

  void OnMouseMove(CRhinoViewport& vp, const ON_3dPoint& pt, const CPoint* point);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

public:
  const ON_PolylineCurve* m_curve;
  bool m_draw;
  int m_index0;
  int m_index1;
};

CRhGetPolylineSegment::CRhGetPolylineSegment(const ON_PolylineCurve* curve)
  : m_curve(curve)
  , m_draw(false)
  , m_index0(-1)
  , m_index1(-1)
{
}

bool CRhGetPolylineSegment::CalculatePolylineSegment(const ON_3dPoint& pt, int& index0, int& index1)
{
  if (0 == m_curve)
    return false;

  index0 = index1 = -1;

  double t = 0.0;
  if (m_curve->GetClosestPoint(pt, &t))
  {
    int count = m_curve->m_t.Count();
    index0 = ON_SearchMonotoneArray(m_curve->m_t, count, t);
    if (index0 >= 0 && index0 <= count - 1)
    {
      if (index0 == count - 1)
      {
        if (m_curve->IsClosed())
          index1 = 0;
        else
        {
          index0 = count - 2;
          index1 = count - 1;
        }
      }
      else
        index1 = m_index0 + 1;
      return true;
    }
  }

  return false;
}

void CRhGetPolylineSegment::OnMouseMove(CRhinoViewport& vp, const ON_3dPoint& pt, const CPoint* point)
{
  m_draw = CalculatePolylineSegment(pt, m_index0, m_index1);
  CRhinoGetPoint::OnMouseMove(vp, pt, point);
}

void CRhGetPolylineSegment::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  if (m_draw && m_curve)
  {
    ON_Color saved_color = dp.SetObjectColor(RhinoApp().AppSettings().SelectedObjectColor());
    dp.DrawLine(m_curve->m_pline[m_index0], m_curve->m_pline[m_index1]);
    dp.SetObjectColor(saved_color);
  }
  CRhinoGetPoint::DynamicDraw(dp, pt);
}

////////////////////////////////////////////////////////////////

class CRhGetPolylineObject : public CRhinoGetObject
{
public:
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;
};

bool CRhGetPolylineObject::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
{
  UNREFERENCED_PARAMETER(object);
  UNREFERENCED_PARAMETER(component_index);
  if (geometry && ON_PolylineCurve::Cast(geometry))
    return true;
  return false;
}

////////////////////////////////////////////////////////////////

class CCommandSampleSelectPolylineSegment : public CRhinoCommand
{
public:
  CCommandSampleSelectPolylineSegment() {}
  ~CCommandSampleSelectPolylineSegment() {}
  UUID CommandUUID()
  {
    // {D03215CF-283A-4D6F-8D25-21D4681ED09F}
    static const GUID SampleSelectPolylineSegmentCommand_UUID =
    { 0xD03215CF, 0x283A, 0x4D6F, { 0x8D, 0x25, 0x21, 0xD4, 0x68, 0x1E, 0xD0, 0x9F } };
    return SampleSelectPolylineSegmentCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSelectPolylineSegment"; }
  const wchar_t* LocalCommandName() const { return L"SampleSelectPolylineSegment"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSelectPolylineSegment object
static class CCommandSampleSelectPolylineSegment theSampleSelectPolylineSegmentCommand;

CRhinoCommand::result CCommandSampleSelectPolylineSegment::RunCommand(const CRhinoCommandContext& context)
{
  CRhGetPolylineObject go;
  go.SetCommandPrompt(L"Select polyline");
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef object_ref = go.Object(0);

  const CRhinoObject* object = object_ref.Object();
  const ON_PolylineCurve* curve = ON_PolylineCurve::Cast(object_ref.Curve());
  if (0 == object || 0 == curve)
    return CRhinoCommand::failure;

  object->Select(false);
  context.m_doc.Redraw();

  CRhGetPolylineSegment gp(curve);
  gp.SetCommandPrompt(L"Select polyline segment");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint point = gp.Point();

  int index0 = -1;
  int index1 = -1;

  if (gp.CalculatePolylineSegment(point, index0, index1))
  {
    ON_wString point_str;
    RhinoFormatPoint(point, point_str);
    RhinoApp().Print(L"Curve point = %s\n", point_str);
    RhinoApp().Print(L"Curve index0 = %d\n", index0);
    RhinoApp().Print(L"Curve index1 = %d\n", index1);
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSelectPolylineSegment command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
