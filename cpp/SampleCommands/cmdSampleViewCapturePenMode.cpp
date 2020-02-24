#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleViewCapturePenMode command
//

#pragma region SampleViewCapturePenMode command

class CCommandSampleViewCapturePenMode : public CRhinoCommand
{
public:
  CCommandSampleViewCapturePenMode() = default;
  UUID CommandUUID() override
  {
    // {8E559EC5-53C2-410B-B5AB-9775C8FC5FCA}
    static const GUID SampleViewCapturePenModeCommand_UUID =
    { 0x8E559EC5, 0x53C2, 0x410B, { 0xB5, 0xAB, 0x97, 0x75, 0xC8, 0xFC, 0x5F, 0xCA } };
    return SampleViewCapturePenModeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleViewCapturePenMode"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleViewCapturePenMode object
static class CCommandSampleViewCapturePenMode theSampleViewCapturePenModeCommand;

CRhinoCommand::result CCommandSampleViewCapturePenMode::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoView* view = RhinoApp().ActiveView();
  if (nullptr == view)
    return CRhinoCommand::failure;

  const CDisplayPipelineAttributes* pen_attrs = CRhinoDisplayAttrsMgr::FindDisplayAttrs(ON_StandardDisplayModeId::Pen);
  if (nullptr == pen_attrs)
    return CRhinoCommand::failure;

  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL rc = gf.DisplayFileDialog(CRhinoGetFileDialog::save_bitmap_dialog, 0, RhinoApp().MainWnd());
  if (!rc)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return nothing;

  // Create a viewport AND pipeline based on the targeted attribute's 
  // pipeline class (in this case "Pen").
  CRhinoViewport vp;
  vp.CopyFrom(view->ActiveViewport(), true);
  vp.SetDisplayMode(ON_StandardDisplayModeId::Pen);

  // Now create the correct pipeline object based on the attribute's pipeline class.
  CRhinoDisplayPipeline* dp = ::CreateDisplayPipeline(pen_attrs->Pipeline());
  if (nullptr == dp)
    return CRhinoCommand::failure;

  // Get view dimensions...
  ON_4iRect rect;
  view->GetClientRect(rect);

  CRhinoPrintInfo rpi;
  rpi.SetViewport(&vp);
  rpi.SetLayoutRectangles(rect);

  CRhinoDib dib;

  // Get a working copy of the attributes we want to capture with.
  CDisplayPipelineAttributes capAttrs = *pen_attrs; 

  // A working copy allows us to "force" specific attributes and effects.
  // For example, turns shadows ON even though they may be turned off in
  // the current Pen display mode.
  //capAttrs.m_bCastShadows = true; 

  // Do the capture
  bool res = rpi.DrawToSingleDib(dib, &capAttrs, dp, false);
  if (res)
    dib.WriteToFile(filename);

  // Delete the pipe we created above...
  delete dp;

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleViewCapturePenMode command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
