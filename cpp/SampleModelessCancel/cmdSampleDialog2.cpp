#include "stdafx.h"
#include "SampleModelessCancelPlugIn.h"
#include "SampleDialog2.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDialog2 command
//

#pragma region SampleDialog2 command

class CCommandSampleDialog2 : public CRhinoModelessDialogCommand
{
public:
  CCommandSampleDialog2() = default;
  UUID CommandUUID() override
  {
    // {5FBBC7A9-1C70-4097-A718-EB48A7115F3A}
    static const GUID SampleDialog2Command_UUID =
    { 0x5FBBC7A9, 0x1C70, 0x4097, { 0xA7, 0x18, 0xEB, 0x48, 0xA7, 0x11, 0x5F, 0x3A } };
    return SampleDialog2Command_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDialog2"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

  /////////////////////////////////////////////////////////////////////////////
  // Additional overrides

  void DisposeDialog() override;

private:
  CSampleDialog2* m_pDialog = nullptr;
};

// The one and only CCommandSampleDialog2 object
static class CCommandSampleDialog2 theSampleDialog2Command;

CRhinoCommand::result CCommandSampleDialog2::RunCommand(const CRhinoCommandContext& context)
{
  if (m_pDialog)
  {
    m_pDialog->SetFocus();
    return CRhinoCommand::success;
  }

  CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  m_pDialog = new CSampleDialog2(pParentWnd, this);
  if (m_pDialog->Create(CSampleDialog2::IDD))
  {
    m_pDialog->ShowWindow(SW_SHOW);
    m_pDialog->UpdateWindow();
    return CRhinoCommand::success;
  }

  return CRhinoCommand::failure;
}

void CCommandSampleDialog2::DisposeDialog()
{
  m_pDialog = nullptr;
}

#pragma endregion

//
// END SampleDialog2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
