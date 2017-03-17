#include "stdafx.h"
#include "SampleUserInterfacePlugIn.h"
#include "SampleTabbedDockBarDialog.h"

IMPLEMENT_DYNCREATE(CSampleTabbedDockBarDialog, CRhinoTabbedDockBarDialog)

CSampleTabbedDockBarDialog::CSampleTabbedDockBarDialog()
  : CRhinoTabbedDockBarDialog()
{
}

CSampleTabbedDockBarDialog::~CSampleTabbedDockBarDialog()
{
}

void CSampleTabbedDockBarDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoTabbedDockBarDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CLICKME_BUTTON, m_clickme_button);
}

BEGIN_MESSAGE_MAP(CSampleTabbedDockBarDialog, CRhinoTabbedDockBarDialog)
  ON_BN_CLICKED(IDC_CLICKME_BUTTON, &CSampleTabbedDockBarDialog::OnClickedClickMeButton)
END_MESSAGE_MAP()

const wchar_t* CSampleTabbedDockBarDialog::Caption() const
{
  return L"SampleTab";
}

ON_UUID CSampleTabbedDockBarDialog::TabId() const
{
  return ID();
}

ON_UUID CSampleTabbedDockBarDialog::ID()
{
  // {47940C29-67D1-4F13-97D0-583BCE95C537}
  static const GUID SampleTabbedDockBarDialog_UUID =
  { 0x47940C29, 0x67D1, 0x4F13, { 0x97, 0xD0, 0x58, 0x3B, 0xCE, 0x95, 0xC5, 0x37 } };
  return SampleTabbedDockBarDialog_UUID;
}

ON_UUID CSampleTabbedDockBarDialog::PlugInId() const
{
  return SampleUserInterfacePlugIn().PlugInID();
}

HICON CSampleTabbedDockBarDialog::Icon(const CSize& sizeInPixels) const
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return CRhinoDpi::LoadIcon(AfxGetInstanceHandle(), IDI_DOCKBAR_ICON, sizeInPixels.cx, sizeInPixels.cy);
}

void CSampleTabbedDockBarDialog::OnShowTab(bool bShowTab, const ON_UUID& tabId)
{
  UNREFERENCED_PARAMETER(bShowTab);
  UNREFERENCED_PARAMETER(tabId);

  // TODO: Add "On Show" or "On Hide" tab code here
}

BOOL CSampleTabbedDockBarDialog::OnInitDialog()
{
  m_Resize.Add(IDC_CLICKME_BUTTON, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);

  CRhinoTabbedDockBarDialog::OnInitDialog();

  return TRUE;
}

void CSampleTabbedDockBarDialog::OnClickedClickMeButton()
{
  // TODO: Add your control notification handler code here
}
