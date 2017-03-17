#pragma once

#include "resource.h"

// CSampleTabDialogPage1 dialog

class CSampleTabDialogPage1 : public CRhinoUiTabCtrlPage
{
	DECLARE_DYNAMIC(CSampleTabDialogPage1)

public:
	CSampleTabDialogPage1();
	virtual ~CSampleTabDialogPage1();

// Dialog Data
	enum { IDD = IDD_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
