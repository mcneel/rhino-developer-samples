#include "stdafx.h"
#include "SampleTabDialog.h"
#include "SampleTabDialogPage3.h"

// CSampleTabDialogPage3 dialog

IMPLEMENT_DYNAMIC(CSampleTabDialogPage3, CRhinoUiTabCtrlPage)

CSampleTabDialogPage3::CSampleTabDialogPage3()
	: CRhinoUiTabCtrlPage(CSampleTabDialogPage3::IDD)
{
}

CSampleTabDialogPage3::~CSampleTabDialogPage3()
{
}

void CSampleTabDialogPage3::DoDataExchange(CDataExchange* pDX)
{
	CRhinoUiTabCtrlPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleTabDialogPage3, CRhinoUiTabCtrlPage)
END_MESSAGE_MAP()

// CSampleTabDialogPage3 message handlers
