#include "StdAfx.h"
#include "SampleUserInterfacePlugIn.h"
#include "SampleCirclePreviewDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCirclePreview command
//

class CCommandSampleCirclePreview : public CRhinoCommand
{
public:
  CCommandSampleCirclePreview() {}
  ~CCommandSampleCirclePreview() {}
  UUID CommandUUID()
  {
    // {6A65EEE7-1E50-46D7-834D-2D8108AEA73E}
    static const GUID SampleCirclePreviewCommand_UUID =
    { 0x6A65EEE7, 0x1E50, 0x46D7, { 0x83, 0x4D, 0x2D, 0x81, 0x08, 0xAE, 0xA7, 0x3E } };
    return SampleCirclePreviewCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCirclePreview"; }
  const wchar_t* LocalCommandName() { return L"SampleCirclePreview"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCirclePreview object
static class CCommandSampleCirclePreview theSampleCirclePreviewCommand;

CRhinoCommand::result CCommandSampleCirclePreview::RunCommand(const CRhinoCommandContext& context)
{
  CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  CSampleCirclePreviewDialog dialog(pParentWnd, context.m_doc.RuntimeSerialNumber());
  if (dialog.DoModal() == IDOK)
    context.m_doc.AddCurveObject(dialog.m_preview.m_circle);
  return CRhinoCommand::success;
}

//
// END SampleCirclePreview command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
