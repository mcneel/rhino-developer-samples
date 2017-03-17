#include "stdafx.h"
#include "SampleModalDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleModalDialog command
//

class CCommandSampleModalDialog : public CRhinoCommand
{
public:
  CCommandSampleModalDialog() {}
  ~CCommandSampleModalDialog() {}
  UUID CommandUUID()
  {
    // {A2DCBCBD-B91B-4FF4-88DF-70D13107F0BF}
    static const GUID SampleModalDialogCommand_UUID =
    { 0xA2DCBCBD, 0xB91B, 0x4FF4, { 0x88, 0xDF, 0x70, 0xD1, 0x31, 0x07, 0xF0, 0xBF } };
    return SampleModalDialogCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleModalDialog"; }
  const wchar_t* LocalCommandName() { return L"SampleModalDialog"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleModalDialog object
static class CCommandSampleModalDialog theSampleModalDialogCommand;

CRhinoCommand::result CCommandSampleModalDialog::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  CSampleModalDialog dialog(pParentWnd);
  dialog.DoModal();

  return CRhinoCommand::success;
}

//
// END SampleModalDialog command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
