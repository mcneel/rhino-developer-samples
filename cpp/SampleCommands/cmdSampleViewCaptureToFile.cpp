#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleViewCaptureToFile command
//

#pragma region SampleViewCaptureToFile command

class CCommandSampleViewCaptureToFile : public CRhinoCommand
{
public:
  CCommandSampleViewCaptureToFile() {}
  ~CCommandSampleViewCaptureToFile() {}
  UUID CommandUUID()
  {
    // {A3016DC5-FEBC-40C3-A05A-81538658DFE2}
    static const GUID SampleViewCaptureToFileCommand_UUID =
    { 0xA3016DC5, 0xFEBC, 0x40C3, { 0xA0, 0x5A, 0x81, 0x53, 0x86, 0x58, 0xDF, 0xE2 } };
    return SampleViewCaptureToFileCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleViewCaptureToFile"; }
  const wchar_t* LocalCommandName() const { return L"SampleViewCaptureToFile"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleViewCaptureToFile object
static class CCommandSampleViewCaptureToFile theSampleViewCaptureToFileCommand;

CRhinoCommand::result CCommandSampleViewCaptureToFile::RunCommand(const CRhinoCommandContext& context)
{
  CWnd* pMainWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  if (0 == pMainWnd)
    return CRhinoCommand::failure;

  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL rc = gf.DisplayFileDialog(CRhinoGetFileDialog::save_bitmap_dialog, 0, pMainWnd);
  if (!rc)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return nothing;

  CRhinoView* view = RhinoApp().ActiveView();
  if (view)
  {
    CRect rect;
    view->GetClientRect(rect);

    CRhinoDib dib;
    if (dib.CreateDib(rect.Width(), rect.Height(), 24, true))
    {
      // Set these flags as you wish.
      BOOL bIgnoreHighlights = TRUE;
      BOOL bDrawTitle = FALSE;
      BOOL bDrawConstructionPlane = FALSE;
      BOOL bDrawWorldAxes = FALSE;

      CRhinoObjectIterator it(CRhinoObjectIterator::normal_or_locked_objects, CRhinoObjectIterator::active_and_reference_objects);

      if (view->ActiveViewport().View().m_display_mode_id == ON_StandardDisplayModeId::Wireframe)
      {
        context.m_doc.DrawToDC(it, dib, dib.Width(), dib.Height(),
          view->ActiveViewport().View(),
          bIgnoreHighlights,
          bDrawTitle,
          bDrawConstructionPlane,
          bDrawWorldAxes
        );
      }
      else
      {
        context.m_doc.RenderToDC(it, dib, dib.Width(), dib.Height(),
          view->ActiveViewport().View(),
          bIgnoreHighlights,
          bDrawTitle,
          bDrawConstructionPlane,
          bDrawWorldAxes,
          FALSE
        );
      }

      dib.WriteToFile(filename);
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleViewCaptureToFile command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
