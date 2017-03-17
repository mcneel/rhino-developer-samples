#pragma once

#include "resource.h"
#include "SampleTabDialogPage1.h"
#include "SampleTabDialogPage2.h"
#include "SampleTabDialogPage3.h"

// CSampleTabDialog dialog

class CSampleTabDialog : public CRhinoUiDialog
{
	DECLARE_DYNAMIC(CSampleTabDialog)

public:
	CSampleTabDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSampleTabDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG };
  CRhinoUiTabPageCtrl m_tab_ctrl;
  CSampleTabDialogPage1 m_page1;
  CSampleTabDialogPage2 m_page2;
  CSampleTabDialogPage3 m_page3;

protected:
  virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
