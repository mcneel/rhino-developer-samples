#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleViewportDecoration command
//

class CSampleViewportDecorationEventWatcher : public CRhinoEventWatcher
{
public:
  CSampleViewportDecorationEventWatcher();
  ~CSampleViewportDecorationEventWatcher() {}

  void SetActiveView(CRhinoView* active_view);

  // CRhinoEventWatcher overrides
  void OnSetActiveView(CRhinoView* new_active_view);

private:
  CRhinoView* m_old_active_view;
};

CSampleViewportDecorationEventWatcher::CSampleViewportDecorationEventWatcher()
  : m_old_active_view(0)
{
}

void CSampleViewportDecorationEventWatcher::SetActiveView(CRhinoView* active_view)
{
  m_old_active_view = active_view;
}

void CSampleViewportDecorationEventWatcher::OnSetActiveView(CRhinoView* new_active_view)
{
  // Regenerate the old active view (to remove decoration)
  if (0 != m_old_active_view)
    m_old_active_view->Redraw(CRhinoView::regenerate_display_hint);

  // Regenerate the new active view (to draw decoration)
  if (0 != new_active_view)
    new_active_view->Redraw(CRhinoView::regenerate_display_hint);

  m_old_active_view = new_active_view;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// Font enumeration callback
int CALLBACK EnumFontFamiliesExCallback(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lpntme);
  UNREFERENCED_PARAMETER(FontType);

  if (0 != lpelfe && 0 != lParam)
  {
    ON_wString str(lpelfe->elfFullName);
    ON_ClassArray<ON_wString>* pFontFaces = (ON_ClassArray<ON_wString>*)lParam;
    pFontFaces->Append(str);
  }
  return 1;
}

class CSampleViewportDecorationConduit : public CRhinoDisplayConduit
{
public:
  CSampleViewportDecorationConduit();
  ~CSampleViewportDecorationConduit() {}

  // CRhinoDisplayConduit override
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannelID, bool& bTerminate);

private:
  const wchar_t* m_default_font_face;
  ON_ClassArray<ON_wString> m_font_faces;
};

CSampleViewportDecorationConduit::CSampleViewportDecorationConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWFOREGROUND)
  , m_default_font_face(L"Arial")
{
  // Find some font faces to draw with
  LOGFONT lf;
  memset(&lf, 0, sizeof(lf));
  wcscpy_s(lf.lfFaceName, LF_FACESIZE, m_default_font_face);
  lf.lfCharSet = ANSI_CHARSET;

  HDC hDC = ::GetDC(0);
  EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC)EnumFontFamiliesExCallback, (LPARAM)&m_font_faces, 0);
  ReleaseDC(0, hDC);
}

bool CSampleViewportDecorationConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannelID, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  // Get the active view
  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view)
    return true;

  // Get the viewport that we are currently drawing in
  CRhinoViewport* vp = dp.GetRhinoVP();
  if (nullptr == vp || view->ActiveViewportID() != vp->ViewportId())
    return true;

  if (nChannelID == CSupportChannels::SC_DRAWFOREGROUND)
  {
    // String to draw
    const wchar_t* str = L"Hello Rhino!";
    const ON_Color color(0, 0, 0);

    // Determine rect of text string
    const bool bMiddle = false;
    const int height = 12;
    CRect rect;
    dp.MeasureString(rect, str, ON_2dPoint(0, 0), bMiddle, height, m_default_font_face);

    // Use the screen port to determine text location
    int vp_left, vp_right, vp_top, vp_bottom;
    vp->VP().GetScreenPort(&vp_left, &vp_right, &vp_bottom, &vp_top);
    int vp_width = vp_right - vp_left;
    int vp_height = vp_bottom - vp_top;

    // Make sure text will fit on string
    const int x_gap = 6;
    const int y_gap = 6;
    if (rect.Width() + (2 * x_gap) < vp_width || rect.Height() + (2 * y_gap) < vp_height)
    {
      // Cook up text location (lower right corner of viewport)
      ON_2dPoint point(vp_right - rect.Width() - x_gap, vp_bottom - y_gap);

      // Draw text
      if (0 == m_font_faces.Count())
        dp.DrawString(str, color, point, bMiddle, height, m_default_font_face);
      else
      {
        for (int i = 0; i < m_font_faces.Count(); i++)
        {
          dp.DrawString(str, color, point, bMiddle, height, m_font_faces[i]);
          point.y -= 20.0;
        }
      }
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleViewportDecoration : public CRhinoCommand
{
public:
  CCommandSampleViewportDecoration();
  ~CCommandSampleViewportDecoration() {}
  UUID CommandUUID()
  {
    // {9A827A94-B266-4D97-BD50-AE388C9E6FB0}
    static const GUID SampleViewportDecorationCommand_UUID =
    { 0x9A827A94, 0xB266, 0x4D97, { 0xBD, 0x50, 0xAE, 0x38, 0x8C, 0x9E, 0x6F, 0xB0 } };
    return SampleViewportDecorationCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleViewportDecoration"; }
  const wchar_t* LocalCommandName() { return L"SampleViewportDecoration"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CSampleViewportDecorationConduit m_conduit;
  CSampleViewportDecorationEventWatcher m_watcher;
};

// The one and only CCommandSampleViewportDecoration object
static class CCommandSampleViewportDecoration theSampleViewportDecorationCommand;

CCommandSampleViewportDecoration::CCommandSampleViewportDecoration()
{
  // Register our event watcher
  m_watcher.Register();
}

CRhinoCommand::result CCommandSampleViewportDecoration::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  bool bEnable = m_conduit.IsEnabled();

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Viewport decoration options");
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Enable"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bEnable, &bEnable);
  go.AcceptNothing();
  for (;;)
  {
    CRhinoGet::result res = go.GetOption();

    if (res == CRhinoGet::option)
    {
      // Get the active view
      CRhinoView* view = RhinoApp().ActiveView();
      if (0 == view)
      {
        RhinoApp().Print(L"Unable to obtain the active view.\n");
        return CRhinoCommand::failure;
      }

      if (bEnable)
      {
        m_watcher.SetActiveView(view);
        m_watcher.Enable(TRUE);
        m_conduit.Enable();
      }
      else
      {
        m_watcher.SetActiveView(0);
        m_watcher.Enable(FALSE);
        m_conduit.Disable();
      }

      // Redraw the active view
      view->Redraw(CRhinoView::regenerate_display_hint);
      continue;
    }

    break;
  }

  return CRhinoCommand::success;
}

//
// END SampleViewportDecoration command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
