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
  CCommandSampleViewCaptureToFile() = default;
  ~CCommandSampleViewCaptureToFile() = default;
  UUID CommandUUID() override
  {
    // {A3016DC5-FEBC-40C3-A05A-81538658DFE2}
    static const GUID SampleViewCaptureToFileCommand_UUID =
    { 0xA3016DC5, 0xFEBC, 0x40C3, { 0xA0, 0x5A, 0x81, 0x53, 0x86, 0x58, 0xDF, 0xE2 } };
    return SampleViewCaptureToFileCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleViewCaptureToFile"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleViewCaptureToFile object
static class CCommandSampleViewCaptureToFile theSampleViewCaptureToFileCommand;

CRhinoCommand::result CCommandSampleViewCaptureToFile::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL rc = gf.DisplayFileDialog(CRhinoGetFileDialog::save_bitmap_dialog, 0, RhinoApp().MainWnd());
  if (!rc)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return nothing;

  CRhinoView* view = RhinoApp().ActiveView();
  if (nullptr == view)
    return CRhinoCommand::failure;

  CRhinoDisplayPipeline* dp = view->ActiveViewport().DisplayPipeline();
  if (nullptr == dp)
    return CRhinoCommand::failure;

  ON_4iRect rect;
  view->GetClientRect(rect);

  CRhinoDib dib;
  if (!dib.CreateDib(rect.Width(), rect.Height(), 24, true))
    return CRhinoCommand::failure;

  const CDisplayPipelineAttributes* pDA = CRhinoDisplayAttrsMgr::StdWireframeAttrs();
  //const CDisplayPipelineAttributes* pDA = CRhinoDisplayAttrsMgr::StdShadedAttrs();
  //const CDisplayPipelineAttributes* pDA = CRhinoDisplayAttrsMgr::StdShadedAttrs();

  CDisplayPipelineAttributes da(*pDA);
  da.m_contextDrawToDC = CDisplayPipelineAttributes::ContextsForDrawToDC::kUIPreview;
  da.m_bUseDocumentGrid = false;
  da.m_bDrawGrid = false;
  da.m_bDrawAxes = false;
  da.m_bDrawWorldAxes = false;
  da.m_groundPlaneUsage = CDisplayPipelineAttributes::GroundPlaneUsages::Custom;
  da.m_customGroundPlaneSettings.on = false;
  da.m_bCastShadows = false;

  bool rc = dp->DrawToDib(dib, rect.Width(), rect.Height(), da);
  if (rc)
    dib.WriteToFile(filename);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleViewCaptureToFile command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
