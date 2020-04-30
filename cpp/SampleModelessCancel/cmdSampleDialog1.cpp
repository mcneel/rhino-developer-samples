#include "stdafx.h"
#include "SampleModelessCancelPlugIn.h"
#include "SampleDialog1.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDialog1 command
//

#pragma region SampleDialog1 command

class CCommandSampleDialog1 : public CRhinoModelessDialogCommand
{
public:
  CCommandSampleDialog1() = default;
  UUID CommandUUID() override
  {
    // {720B9D6-FFA3-4452-A8E3-E074F2471AAA}
    static const GUID SampleDialog1Command_UUID =
    { 0x720B9D6, 0xFFA3, 0x4452, { 0xA8, 0xE3, 0xE0, 0x74, 0xF2, 0x47, 0x1A, 0xAA } };
    return SampleDialog1Command_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDialog1"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

  /////////////////////////////////////////////////////////////////////////////
  // Additional overrides

  void DisposeDialog() override;

private:
  CSampleDialog1* m_pDialog = nullptr;
};

// The one and only CCommandSampleDialog1 object
static class CCommandSampleDialog1 theSampleDialog1Command;

CRhinoCommand::result CCommandSampleDialog1::RunCommand(const CRhinoCommandContext& context)
{
  if (m_pDialog)
  {
    m_pDialog->SetFocus();
    return CRhinoCommand::success;
  }

  CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  m_pDialog = new CSampleDialog1(pParentWnd, this);
  if (m_pDialog->Create(CSampleDialog1::IDD))
  {
    m_pDialog->ShowWindow(SW_SHOW);
    m_pDialog->UpdateWindow();
    return CRhinoCommand::success;
  }

  return CRhinoCommand::failure;
}

void CCommandSampleDialog1::DisposeDialog()
{
  m_pDialog = nullptr;
}

#pragma endregion

//
// END SampleDialog1 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
