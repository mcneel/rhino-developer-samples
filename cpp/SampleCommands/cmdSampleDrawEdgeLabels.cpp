#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawEdgeLabels command
//

class CSampleDrawEdgeLabelsConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawEdgeLabelsConduit(const ON_SimpleArray<ON_3dPoint>& points, int text_height);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel) override;

private:
  void DrawLabel(CRhinoDisplayPipeline& dp, int index, const ON_3dPoint& pt, const ON_Xform& w2s);

private:
  ON_SimpleArray<ON_3dPoint> m_points;
  int m_text_height;
};

CSampleDrawEdgeLabelsConduit::CSampleDrawEdgeLabelsConduit(const ON_SimpleArray<ON_3dPoint>& points, int text_height)
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWOVERLAY)
  , m_points(points)
  , m_text_height(text_height)
{
}

bool CSampleDrawEdgeLabelsConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  switch (nActiveChannel)
  {
  case CSupportChannels::SC_DRAWOVERLAY:
  {
    ON_Color old_color = dp.SetObjectColor(RhinoApp().AppSettings().TrackingColor());

    ON_Xform w2s;
    dp.VP().GetXform(ON::world_cs, ON::screen_cs, w2s);

    for (int i = 0; i < m_points.Count(); i++)
      DrawLabel(dp, i, m_points[i], w2s);

    dp.SetObjectColor(old_color);
  }
  break;

  default:
    break;
  }

  return true;
}

void CSampleDrawEdgeLabelsConduit::DrawLabel(CRhinoDisplayPipeline& dp, int index, const ON_3dPoint& point, const ON_Xform& w2s)
{
  const CRhinoDisplayEngine* engine = dp.Engine(false);
  if (nullptr == engine)
    return;

  float dpi_scale = engine->DpiScale();
  int text_height = (int)(m_text_height * dpi_scale);

  ON_3dPoint screen_point = w2s * point;
  screen_point.x += (int)(-5 * dpi_scale);
  screen_point.y -= (int)(5 * dpi_scale);

  ON_wString str;
  str.Format(L"%d", index);

  ON_4iRect rect;
  if (!CRhinoDisplayPipeline::MeasureString(rect, str, screen_point, false, text_height, L"System"))
    return;

  rect.InflateRect(3, -3, 3, 3);

  dp.DrawString(str, screen_point, false, text_height, L"System");
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleDrawEdgeLabels : public CRhinoCommand
{
public:
  CCommandSampleDrawEdgeLabels() = default;
  UUID CommandUUID() override
  {
    // {CACB53E1-B0CF-411B-BB82-94EC55A0CFDE}
    static const GUID SampleDrawEdgeLabelsCommand_UUID =
    { 0xCACB53E1, 0xB0CF, 0x411B, { 0xBB, 0x82, 0x94, 0xEC, 0x55, 0xA0, 0xCF, 0xDE } };
    return SampleDrawEdgeLabelsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDrawEdgeLabels"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDrawEdgeLabels object
static class CCommandSampleDrawEdgeLabels theSampleDrawEdgeLabelsCommand;

CRhinoCommand::result CCommandSampleDrawEdgeLabels::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select edges");
  go.SetGeometryFilter(CRhinoGetObject::curve_object | CRhinoGetObject::polyedge_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::mated_edge | CRhinoGetObject::edge_curve);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  ON_SimpleArray<ON_3dPoint> points;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const ON_BrepEdge* edge = go.Object(i).Edge();
    if (nullptr == edge)
      return CRhinoCommand::failure;

    double t = edge->Domain().Mid();
    points.Append(edge->PointAt(t));
  }

  CSampleDrawEdgeLabelsConduit conduit(points, 14);
  conduit.Enable(doc->RuntimeSerialNumber());
  doc->Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  doc->Redraw();

  return CRhinoCommand::success;
}

//
// END SampleDrawEdgeLabels command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
