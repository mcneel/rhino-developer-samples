#include "stdafx.h"
#include "SampleTabDialog.h"
#include "SampleTabDialogPage2.h"

// CSampleTabDialogPage2 dialog

IMPLEMENT_DYNAMIC(CSampleTabDialogPage2, CRhinoUiTabCtrlPage)

CSampleTabDialogPage2::CSampleTabDialogPage2()
	: CRhinoUiTabCtrlPage(CSampleTabDialogPage2::IDD)
{
}

CSampleTabDialogPage2::~CSampleTabDialogPage2()
{
}

void CSampleTabDialogPage2::DoDataExchange(CDataExchange* pDX)
{
	CRhinoUiTabCtrlPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleTabDialogPage2, CRhinoUiTabCtrlPage)
END_MESSAGE_MAP()

// CSampleTabDialogPage2 message handlers
