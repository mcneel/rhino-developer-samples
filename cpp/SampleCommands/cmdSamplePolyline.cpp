#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePolyline command
//

class CGetPolylinePoints : public CRhinoGetPoint
{
public:
  CGetPolylinePoints() {}
  virtual void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);
  int GetPoints();

public:
  ON_3dPointArray m_point_array;
};

int CGetPolylinePoints::GetPoints()
{
  m_point_array.Empty();

  SetCommandPrompt(L"Start of polyline");
  AcceptNothing();

  CRhinoGet::result res = GetPoint();

  if (res == CRhinoGet::point)
  {
    m_point_array.Append(Point());

    SetCommandPrompt(L"Next point of polyline");

    for (;; )
    {
      SetBasePoint(Point());

      res = GetPoint();

      if (res == CRhinoGet::point)
      {
        ON_3dPoint pt = Point();
        if (pt.DistanceTo(m_point_array[m_point_array.Count() - 1]) > ON_ZERO_TOLERANCE)
          m_point_array.Append(Point());
        continue;
      }

      if (res == CRhinoGet::nothing)
        break;

      return 0;
    }
  }

  return m_point_array.Count();
}

void CGetPolylinePoints::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  if (m_point_array.Count() > 0)
  {
    int i;
    for (i = 1; i < m_point_array.Count(); i++)
    {
      dp.DrawPoint(m_point_array[i - 1]);
      dp.DrawLine(m_point_array[i - 1], m_point_array[i]);
    }

    dp.DrawPoint(m_point_array[i - 1]);
    dp.DrawPoint(pt);
    dp.DrawLine(m_point_array[i - 1], pt);
  }

  CRhinoGetPoint::DynamicDraw(dp, pt);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSamplePolyline : public CRhinoCommand
{
public:
  CCommandSamplePolyline() {}
  ~CCommandSamplePolyline() {}
  UUID CommandUUID()
  {
    // {26FF367F-DF76-4CEF-A540-232D8F0BC825}
    static const GUID SamplePolylineCommand_UUID =
    { 0x26FF367F, 0xDF76, 0x4CEF, { 0xA5, 0x40, 0x23, 0x2D, 0x8F, 0x0B, 0xC8, 0x25 } };
    return SamplePolylineCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePolyline"; }
  const wchar_t* LocalCommandName() { return L"SamplePolyline"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePolyline object
static class CCommandSamplePolyline theSamplePolylineCommand;

CRhinoCommand::result CCommandSamplePolyline::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoCommand::result rc = CRhinoCommand::nothing;
  ON_Polyline pline;

  if (context.IsInteractive())
  {
    // Our way...
    CGetPolylinePoints gp;
    int count = gp.GetPoints();
    if (count > 1)
    {
      pline = gp.m_point_array;
      if (pline.IsValid())
        rc = CRhinoCommand::success;
    }
  }
  else
  {
    // The Rhino way...
    CArgsRhinoGetPolyline args;
    args.SetFirstPointPrompt(L"Start of polyline");
    args.SetSecondPointPrompt(L"Next point of polyline");
    rc = RhinoGetPolyline(args, pline);
  }

  if (rc == CRhinoCommand::success)
  {
    context.m_doc.AddCurveObject(pline);
    context.m_doc.Redraw();
  }

  return rc;
}

//
// END SamplePolyline command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
