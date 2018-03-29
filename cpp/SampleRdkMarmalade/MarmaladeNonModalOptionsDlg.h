#pragma once


#include "Resource.h"

class CMarmaladeMainOptionsHolder;

class CMarmaladeNonModalOptionsDlg : public CDialog
{
public:
	CMarmaladeNonModalOptionsDlg();
	virtual ~CMarmaladeNonModalOptionsDlg();

protected: // Event sink overrides.
	virtual void OnRendererChanged(const UUID& uuidOldRenderEngine, const UUID& uuidNewRenderEngine);

private:
	enum { IDD = IDD_NON_MODAL_OPTIONS };

	CButton m_btRender;
	CButton m_check_expert;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	afx_msg LRESULT OnNcHitTest(CPoint pt);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtRender();
	afx_msg void OnCheckExpert();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CMarmaladeMainOptionsHolder* m_pHolder;
	CRect m_rectHolder;
	CRect m_rectClient;
	int m_iBigHeight;
	bool m_bIsVisible;
	bool m_bWasVisible;
};

