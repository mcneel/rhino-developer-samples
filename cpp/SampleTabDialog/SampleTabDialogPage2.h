#pragma once

#include "resource.h"

// CSampleTabDialogPage2 dialog

class CSampleTabDialogPage2 : public CRhinoUiTabCtrlPage
{
	DECLARE_DYNAMIC(CSampleTabDialogPage2)

public:
	CSampleTabDialogPage2();
	virtual ~CSampleTabDialogPage2();

// Dialog Data
	enum { IDD = IDD_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
