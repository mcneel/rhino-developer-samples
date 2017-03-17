/////////////////////////////////////////////////////////////////////////////
// cmdSampleTabDialog.cpp : command file
//

#include "StdAfx.h"
#include "SampleTabDialogPlugIn.h"
#include "SampleTabDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTabDialog command
//

#pragma region SampleTabDialog command

class CCommandSampleTabDialog : public CRhinoCommand
{
public:
  CCommandSampleTabDialog() {}
  ~CCommandSampleTabDialog() {}
  UUID CommandUUID()
  {
    // {C105EC1D-A717-47A4-AB26-2320DC6EF8D8}
    static const GUID SampleTabDialogCommand_UUID =
    { 0xC105EC1D, 0xA717, 0x47A4, { 0xAB, 0x26, 0x23, 0x20, 0xDC, 0x6E, 0xF8, 0xD8 } };
    return SampleTabDialogCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleTabDialog"; }
  const wchar_t* LocalCommandName() const { return L"SampleTabDialog"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

static class CCommandSampleTabDialog theSampleTabDialogCommand;

CRhinoCommand::result CCommandSampleTabDialog::RunCommand( const CRhinoCommandContext& context )
{
  CSampleTabDialog dialog(CWnd::FromHandle(RhinoApp().MainWnd()));
  INT_PTR rc = dialog.DoModal();
  return (rc == IDOK) ? CRhinoCommand::success : CRhinoCommand::cancel;
}

#pragma endregion

//
// END SampleTabDialog command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
