#pragma once

#include "resource.h"

// CSampleTabDialogPage3 dialog

class CSampleTabDialogPage3 : public CRhinoUiTabCtrlPage
{
	DECLARE_DYNAMIC(CSampleTabDialogPage3)

public:
	CSampleTabDialogPage3();
	virtual ~CSampleTabDialogPage3();

// Dialog Data
	enum { IDD = IDD_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
