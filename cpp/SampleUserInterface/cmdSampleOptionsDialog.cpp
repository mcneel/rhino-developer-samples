#include "StdAfx.h"
#include "SampleOptionsListCtrlDlg.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOptionsDialog command
//

#pragma region SampleOptionsDialog command

class CCommandSampleOptionsDialog : public CRhinoCommand
{
public:
  CCommandSampleOptionsDialog() = default;
  ~CCommandSampleOptionsDialog() = default;
  UUID CommandUUID() override
  {
    // {C9704599-3C83-40AE-A4A8-A0A98827B1B6}
    static const GUID SampleOptionsDialogCommand_UUID =
    { 0xC9704599, 0x3C83, 0x40AE, { 0xA4, 0xA8, 0xA0, 0xA9, 0x88, 0x27, 0xB1, 0xB6 } };
    return SampleOptionsDialogCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleOptionsDialog"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};
static class CCommandSampleOptionsDialog theSampleOptionsDialogCommand;

CRhinoCommand::result CCommandSampleOptionsDialog::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CSampleOptionsListCtrlDlg dlg;
  INT_PTR rc = dlg.DoModal();

  return (rc == IDOK) ? CRhinoCommand::success : CRhinoCommand::cancel;
}

#pragma endregion

//
// END SampleOptionsDialog command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
