#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGradeLine command
//

class CRhinoGradeLinePoint : public CRhinoGetPoint
{
public:
  CRhinoGradeLinePoint();

  void SetGrade(double grade);
  bool CalculateGradePoint(CRhinoViewport& vp, const ON_3dPoint& pt);
  ON_3dPoint GradePoint() const;

  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p2d);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

private:
  bool m_draw;
  double m_grade;
  ON_3dPoint m_grade_pt;
};

CRhinoGradeLinePoint::CRhinoGradeLinePoint()
{
  m_draw = false;
  m_grade = 0.0;
}

void CRhinoGradeLinePoint::SetGrade(double grade)
{
  m_grade = grade;
}

bool CRhinoGradeLinePoint::CalculateGradePoint(CRhinoViewport& vp, const ON_3dPoint& pt)
{
  ON_3dPoint base_pt = BasePoint();
  ON_Plane plane = vp.ConstructionPlane().m_plane;
  plane.SetOrigin(base_pt);

  ON_3dPoint plane_pt = plane.ClosestPointTo(pt);
  double run = plane_pt.DistanceTo(base_pt);
  if (run < 0.000001) // whatever...
    return false;

  double angle = atan(m_grade / 100.0);
  double rise = run * tan(angle);
  m_grade_pt = plane_pt + (plane.zaxis * rise);

  return true;
}

ON_3dPoint CRhinoGradeLinePoint::GradePoint() const
{
  return m_grade_pt;
}

void CRhinoGradeLinePoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p2d)
{
  m_draw = CalculateGradePoint(vp, pt);
  CRhinoGetPoint::OnMouseMove(vp, flags, pt, p2d);
}

void CRhinoGradeLinePoint::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  if (m_draw)
  {
    dp.DrawLine(BasePoint(), m_grade_pt);
    dp.DrawPoint(m_grade_pt);
  }
  CRhinoGetPoint::DynamicDraw(dp, pt);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleGradeLine : public CRhinoCommand
{
public:
  CCommandSampleGradeLine() { m_grade = 0.0; }
  ~CCommandSampleGradeLine() {}
  UUID CommandUUID()
  {
    // {857D4AFB-2984-45A9-84F1-57FFA6D5BD00}
    static const GUID SampleGradeLineCommand_UUID =
    { 0x857D4AFB, 0x2984, 0x45A9, { 0x84, 0xF1, 0x57, 0xFF, 0xA6, 0xD5, 0xBD, 0x00 } };
    return SampleGradeLineCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGradeLine"; }
  const wchar_t* LocalCommandName() { return L"SampleGradeLine"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

public:
  double m_grade;
};

// The one and only CCommandSampleGradeLine object
static class CCommandSampleGradeLine theSampleGradeLineCommand;

CRhinoCommand::result CCommandSampleGradeLine::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Start of line");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint base_pt = gp.Point();
  double grade = m_grade;

  CRhinoGradeLinePoint gg;
  gg.SetCommandPrompt(L"End of line");
  gg.AddCommandOptionNumber(RHCMDOPTNAME(L"Grade"), &grade, L"Percent grade", false, 0.0, 100.0);
  gg.SetBasePoint(base_pt);
  gg.ConstrainToConstructionPlane();
  for (;;)
  {
    gg.SetGrade(grade);
    CRhinoGet::result res = gg.GetPoint();
    if (res == CRhinoGet::option)
      continue;
    else if (res != CRhinoGet::point)
      return CRhinoCommand::cancel;
    break;
  }

  CRhinoView* view = gg.View();
  if (view)
  {
    if (gg.CalculateGradePoint(view->ActiveViewport(), gg.Point()))
    {
      ON_Line line(base_pt, gg.GradePoint());
      context.m_doc.AddCurveObject(line);
      context.m_doc.Redraw();
      m_grade = grade;
      return CRhinoCommand::success;
    }
  }

  return CRhinoCommand::failure;
}

//
// END SampleGradeLine command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
