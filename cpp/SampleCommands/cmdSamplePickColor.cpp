#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePickColor command
//

class CCommandSamplePickColor : public CRhinoCommand
{
public:
  CCommandSamplePickColor() {}
  ~CCommandSamplePickColor() {}
  UUID CommandUUID()
  {
    // {1E2F414B-80BC-4801-8581-127B87742426}
    static const GUID SamplePickColorCommand_UUID =
    { 0x1E2F414B, 0x80BC, 0x4801, { 0x85, 0x81, 0x12, 0x7B, 0x87, 0x74, 0x24, 0x26 } };
    return SamplePickColorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePickColor"; }
  const wchar_t* LocalCommandName() { return L"SamplePickColor"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePickColor object
static class CCommandSamplePickColor theSamplePickColorCommand;

CRhinoCommand::result CCommandSamplePickColor::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetPoint gp;
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return CRhinoCommand::success;

  CRhinoView* view = gp.View();
  if (!view)
    return CRhinoCommand::failure;

  ON_3dPoint world_point = gp.Point();

  ON_Xform w2s;
  view->Viewport().VP().GetXform(ON::world_cs, ON::screen_cs, w2s);
  ON_3dPoint screen_pt = world_point;
  screen_pt.Transform(w2s);

  CPoint point;
  point.x = (int)screen_pt.x;
  point.y = (int)screen_pt.y;

  ::ClientToScreen(view->GetSafeHwnd(), &point);

  HDC hDC = CreateDC(L"DISPLAY", 0, 0, 0);
  COLORREF color = GetPixel(hDC, point.x, point.y);
  DeleteDC(hDC);

  RhinoApp().Print(L"RGB - (%d,%d,%d)\n", GetRValue(color), GetGValue(color), GetBValue(color));

  return CRhinoCommand::success;
}

//
// END SamplePickColor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
