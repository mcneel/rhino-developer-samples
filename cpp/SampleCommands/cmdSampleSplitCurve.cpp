#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSplitCurve command
//

class CRhinoGetCurvePoints : public CRhinoGetPoint
{
public:
  CRhinoGetCurvePoints();

  CRhinoGet::result GetPoints();
  int Points(ON_3dPointArray& points);
  void DynamicDraw(CRhinoDisplayPipeline&, const ON_3dPoint&);

public:
  ON_wString m_first_prompt;
  ON_wString m_next_prompt;
  ON_3dPointArray m_points;
};

CRhinoGetCurvePoints::CRhinoGetCurvePoints()
{
  m_first_prompt = L"First split point";
  m_next_prompt = L"Next split point";
}

int CRhinoGetCurvePoints::Points(ON_3dPointArray& points)
{
  points.Empty();
  points.Append(m_points.Count(), m_points.Array());
  return m_points.Count();
}

CRhinoGet::result CRhinoGetCurvePoints::GetPoints()
{
  m_points.Empty();

  SetCommandPrompt(m_first_prompt);
  AcceptNothing();

  CRhinoGet::result res = GetPoint();

  if (res == CRhinoGet::point)
  {
    m_points.Append(Point());

    SetCommandPrompt(m_next_prompt);
    ClearCommandOptions();

    for (;; )
    {
      res = GetPoint();

      if (res == CRhinoGet::point)
      {
        m_points.Append(Point());
        continue;
      }

      if (res == CRhinoGet::nothing)
        res = CRhinoGet::point;

      break;
    }
  }

  return res;
}

void CRhinoGetCurvePoints::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  if (m_points.Count() > 0)
  {
    for (int i = 0; i < m_points.Count(); i++)
      dp.DrawPoint(m_points[i]);
  }

  CRhinoGetPoint::DynamicDraw(dp, pt);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleSplitCurve : public CRhinoCommand
{
public:
  CCommandSampleSplitCurve() { m_seg_count = 2; m_seg_length = 1.0; }
  ~CCommandSampleSplitCurve() {}
  UUID CommandUUID()
  {
    // {EB78D283-5B6B-4B66-BBE6-872A623A73AF}
    static const GUID SampleSplitCurveCommand_UUID =
    { 0xEB78D283, 0x5B6B, 0x4B66, { 0xBB, 0xE6, 0x87, 0x2A, 0x62, 0x3A, 0x73, 0xAF } };
    return SampleSplitCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSplitCurve"; }
  const wchar_t* LocalCommandName() { return L"SampleSplitCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CRhinoCommand::result PickCurveParameters(CRhinoDoc& doc, const ON_Curve& crv, ON_SimpleArray<double>& crv_t);
  CRhinoCommand::result DivideCurveSegments(CRhinoDoc& doc, const ON_Curve& crv, ON_SimpleArray<double>& crv_t);
  CRhinoCommand::result DivideCurveLength(CRhinoDoc& doc, const ON_Curve& crv, ON_SimpleArray<double>& crv_t);

  int m_seg_count;
  double m_seg_length;
};

// The one and only CCommandSampleSplitCurve object
static class CCommandSampleSplitCurve theSampleSplitCurveCommand;

CRhinoCommand::result CCommandSampleSplitCurve::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to split");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& crv_obj_ref = go.Object(0);
  const CRhinoObject* crv_obj = crv_obj_ref.Object();
  const ON_Curve* crv = crv_obj_ref.Curve();
  if (0 == crv_obj || 0 == crv)
    return CRhinoCommand::failure;

  ON_SimpleArray<double> crv_t;
  CRhinoCommand::result rc = PickCurveParameters(context.m_doc, *crv, crv_t);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_3dmObjectAttributes atts(crv_obj->Attributes());

  int i;
  for (i = 0; i < crv_t.Count() - 1; i++)
  {
    ON_Interval interval(crv_t[i], crv_t[i + 1]);
    if (crv->Domain().Includes(interval))
    {
      ON_Curve* new_crv = ON_TrimCurve(*crv, interval);
      if (new_crv)
      {
        CRhinoCurveObject* new_crv_obj = new CRhinoCurveObject(atts);
        new_crv_obj->SetCurve(new_crv);
        context.m_doc.AddObject(new_crv_obj);
      }
    }
  }

  context.m_doc.DeleteObject(crv_obj_ref);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleSplitCurve::PickCurveParameters(CRhinoDoc& doc, const ON_Curve& crv, ON_SimpleArray<double>& crv_t)
{
  crv_t.Empty();

  CRhinoGetCurvePoints gp;
  gp.Constrain(crv);
  int l_option = gp.AddCommandOption(RHCMDOPTNAME(L"Length"));
  /*int s_option =*/ gp.AddCommandOption(RHCMDOPTNAME(L"Segments"));
  CRhinoGet::result res = gp.GetPoints();
  if (res == CRhinoGet::option)
  {
    const CRhinoCommandOption* option = gp.Option();
    if (0 == option)
      return CRhinoCommand::failure;
    if (option->m_option_index == l_option)
      return DivideCurveLength(doc, crv, crv_t);
    else
      return DivideCurveSegments(doc, crv, crv_t);
  }
  else if (res != CRhinoGet::point)
    return CRhinoCommand::cancel;

  ON_3dPointArray points;
  int i, point_count = gp.Points(points);
  for (i = 0; i < point_count; i++)
  {
    double t = 0;
    if (crv.GetClosestPoint(points[i], &t))
      crv_t.Append(t);
  }

  crv_t.Append(crv.Domain().Min());
  crv_t.Append(crv.Domain().Max());
  crv_t.QuickSort(&ON_CompareIncreasing<double>);

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleSplitCurve::DivideCurveSegments(CRhinoDoc& doc, const ON_Curve& crv, ON_SimpleArray<double>& crv_t)
{
  UNREFERENCED_PARAMETER(doc);

  crv_t.Empty();

  CRhinoGetInteger gi;
  gi.SetCommandPrompt(L"Number of segments to create");
  gi.SetDefaultInteger(m_seg_count);
  gi.SetLowerLimit(2);
  gi.GetInteger();
  if (gi.CommandResult() != CRhinoCommand::success)
    return gi.CommandResult();

  int seg_count = abs(gi.Number());

  crv_t.SetCapacity(seg_count);
  bool rc = RhinoDivideCurve(crv, seg_count, 0, false, true, 0, &crv_t);
  if (!rc)
    return CRhinoCommand::failure;

  if (crv.IsClosed())
    crv_t.Append(crv.Domain().Max());

  crv_t.QuickSort(&ON_CompareIncreasing<double>);

  m_seg_count = seg_count;

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleSplitCurve::DivideCurveLength(CRhinoDoc& doc, const ON_Curve& crv, ON_SimpleArray<double>& crv_t)
{
  UNREFERENCED_PARAMETER(doc);

  crv_t.Empty();

  double crv_length = 0.0;
  if (!crv.GetLength(&crv_length))
    return CRhinoCommand::failure;

  ON_wString prompt;
  prompt.Format(L"Curve length is %g. Length of segments", crv_length);

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(prompt);
  gn.SetDefaultNumber(m_seg_length < crv_length ? m_seg_length : 1.0);
  gn.SetLowerLimit(0.0, TRUE);
  gn.SetUpperLimit(crv_length);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  double seg_length = fabs(gn.Number());

  crv_t.SetCapacity((int)floor(crv_length / seg_length));
  bool rc = RhinoDivideCurve(crv, 0, seg_length, false, true, 0, &crv_t);
  if (!rc)
    return CRhinoCommand::failure;

  if (fmod(crv_length, seg_length) > ON_ZERO_TOLERANCE)
    crv_t.Append(crv.Domain().Max());

  crv_t.QuickSort(&ON_CompareIncreasing<double>);

  m_seg_length = seg_length;

  return CRhinoCommand::success;
}

//
// END SampleSplitCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
