#include "stdafx.h"
#include "SampleTabDialog.h"
#include "SampleTabDialogPage1.h"

// CSampleTabDialogPage1 dialog

IMPLEMENT_DYNAMIC(CSampleTabDialogPage1, CRhinoUiTabCtrlPage)

CSampleTabDialogPage1::CSampleTabDialogPage1()
	: CRhinoUiTabCtrlPage(CSampleTabDialogPage1::IDD)
{
}

CSampleTabDialogPage1::~CSampleTabDialogPage1()
{
}

void CSampleTabDialogPage1::DoDataExchange(CDataExchange* pDX)
{
	CRhinoUiTabCtrlPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleTabDialogPage1, CRhinoUiTabCtrlPage)
END_MESSAGE_MAP()

// CSampleTabDialogPage1 message handlers
