#include "StdAfx.h"
#include "SampleThumbnailPreviewDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleThumbnailPreview command
//

#pragma region SampleThumbnailPreview command

class CCommandSampleThumbnailPreview : public CRhinoCommand
{
public:
  CCommandSampleThumbnailPreview() {}
  ~CCommandSampleThumbnailPreview() {}
  UUID CommandUUID()
  {
    // {52C573FE-16F4-4643-80A6-5CB9EDFB8189}
    static const GUID SampleThumbnailPreviewCommand_UUID =
    { 0x52C573FE, 0x16F4, 0x4643, { 0x80, 0xA6, 0x5C, 0xB9, 0xED, 0xFB, 0x81, 0x89 } };
    return SampleThumbnailPreviewCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleThumbnailPreview"; }
  const wchar_t* LocalCommandName() const { return L"SampleThumbnailPreview"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleThumbnailPreview object.  
static class CCommandSampleThumbnailPreview theSampleThumbnailPreviewCommand;

CRhinoCommand::result CCommandSampleThumbnailPreview::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CSampleThumbnailPreviewDialog dialog(CWnd::FromHandle(RhinoApp().MainWnd()));
  dialog.DoModal();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleThumbnailPreview command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
