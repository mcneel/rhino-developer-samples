#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawArrowhead command
//

#pragma region SampleDrawArrowheadConduit conduit

class CSampleDrawArrowheadConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawArrowheadConduit(ON_Plane plane, ON_Line line, double scale);

  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT channel, bool& terminate);

protected:
  bool GetArrowHead(ON_2dVector dir, ON_2dPoint tip, double scale, ON_3dPointArray& triangle);

protected:
  static double m_default_arrow_size;
  bool m_bDraw;
  ON_Line m_line;
  ON_Plane m_plane;
  ON_3dPointArray m_arrowhead;
};

double CSampleDrawArrowheadConduit::m_default_arrow_size = 1.0;

CSampleDrawArrowheadConduit::CSampleDrawArrowheadConduit(ON_Plane plane, ON_Line line, double scale)
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY)
{
  m_bDraw = false;
  m_plane = plane;
  m_line = line;

  double x = 0.0, y = 0.0;

  m_plane.ClosestPointTo(line.from, &x, &y);
  ON_2dPoint from(x, y);

  m_plane.ClosestPointTo(line.to, &x, &y);
  ON_2dPoint to(x, y);

  ON_2dVector dir(from - to);
  dir.Unitize();

  m_bDraw = GetArrowHead(dir, from, scale, m_arrowhead);
}

bool CSampleDrawArrowheadConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT channel, bool& terminate)
{
  UNREFERENCED_PARAMETER(terminate);

  if (channel == CSupportChannels::SC_CALCBOUNDINGBOX)
  {
    m_pChannelAttrs->m_BoundingBox.Union(m_line.BoundingBox());
  }
  else if (channel == CSupportChannels::SC_DRAWOVERLAY)
  {
    dp.DrawLine(m_line.from, m_line.to, m_pDisplayAttrs->m_ObjectColor | 0xFF000000, m_pDisplayAttrs->m_nLineThickness);
    if (m_bDraw)
      dp.DrawPolygon(m_arrowhead.Array(), m_arrowhead.Count(), m_pDisplayAttrs->m_ObjectColor | 0xFF000000, true);
  }
  return true;
}

bool CSampleDrawArrowheadConduit::GetArrowHead(ON_2dVector dir, ON_2dPoint tip, double scale, ON_3dPointArray& triangle)
{
  double arrow_size = CSampleDrawArrowheadConduit::m_default_arrow_size * scale;

  ON_2dPointArray corners(3);
  corners.SetCount(3);

  ON_2dVector up(-dir.y, dir.x);
  corners[0].Set(tip.x, tip.y);
  corners[1].x = tip.x + arrow_size * (0.25 * up.x - dir.x);
  corners[1].y = tip.y + arrow_size * (0.25 * up.y - dir.y);
  corners[2].x = corners[1].x - 0.5 * arrow_size * up.x;
  corners[2].y = corners[1].y - 0.5 * arrow_size * up.y;

  triangle.Reserve(corners.Count());
  triangle.SetCount(corners.Count());

  for (int i = 0; i < corners.Count(); i++)
    triangle[i] = m_plane.PointAt(corners[i].x, corners[i].y);

  return true;
}

#pragma endregion

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleDrawArrowhead command

class CCommandSampleDrawArrowhead : public CRhinoCommand
{
public:
  CCommandSampleDrawArrowhead() {}
  ~CCommandSampleDrawArrowhead() {}
  UUID CommandUUID()
  {
    // {CD687B7C-E24D-4E2C-9C4B-5DC2AA1BC235}
    static const GUID SampleDrawArrowheadCommand_UUID =
    { 0xCD687B7C, 0xE24D, 0x4E2C, { 0x9C, 0x4B, 0x5D, 0xC2, 0xAA, 0x1B, 0xC2, 0x35 } };
    return SampleDrawArrowheadCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDrawArrowhead"; }
  const wchar_t* LocalCommandName() const { return L"SampleDrawArrowhead"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDrawArrowhead object
static class CCommandSampleDrawArrowhead theSampleDrawArrowheadCommand;

CRhinoCommand::result CCommandSampleDrawArrowhead::RunCommand(const CRhinoCommandContext& context)
{
  ON_Line line;
  ON_Plane plane;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Start of line");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  line.from = gp.Point();
  plane = RhinoActiveCPlane();
  plane.SetOrigin(line.from);

  gp.SetCommandPrompt(L"End of line");
  gp.Constrain(plane);
  gp.SetBasePoint(line.from);
  gp.DrawLineFromPoint(line.from, TRUE);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  line.to = plane.ClosestPointTo(gp.Point());
  if (!line.IsValid())
    return CRhinoCommand::nothing;

  CSampleDrawArrowheadConduit conduit(plane, line, 1.0);
  conduit.Enable(context.m_doc.RuntimeSerialNumber());
  context.m_doc.Redraw();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDrawArrowhead command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
