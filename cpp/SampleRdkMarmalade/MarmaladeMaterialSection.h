
#pragma once

#include "Resource.h"

class CIOREdit : public CEdit
{
public:
	CIOREdit(class CMarmaladeMaterialSection& s) : m_Section(s) { }

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CMarmaladeMaterialSection& m_Section;
};

class CMarmaladeMaterialSection : public CRhRdkMaterialUISection_MFC
{
public:
	CMarmaladeMaterialSection();

private:
	enum { IDD = IDD_MATERIAL_SECTION };
	CRhRdkColorButton m_btColor;
	CButton m_checkTextureOn;
	CEdit m_editTextureAmount;
	CEdit m_editTransparency;
	CEdit m_editTexturePlaceholder;
	CIOREdit m_editIOR;

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void DisplayData(void) override;
	virtual ON_wString Caption(bool bAlwaysEnglish) const override;
	virtual unsigned int GetIDD(void) const override { return IDD; }
	virtual UUID Uuid(void) const override;
	virtual bool SupportsVaries(const CRhRdkContentArray& aContent) const override;
	virtual AFX_MODULE_STATE* GetModuleState(void) const override { return AfxGetStaticModuleState(); }
	virtual void OnEvent(IRhinoUiController&, const UUID& uuidData, IRhinoUiController::EventPriority, const IRhinoUiEventInfo*) override;

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillfocusEditTransparency();
	afx_msg void OnKillfocusEditIOR();
	afx_msg void OnKillfocusEditTextureAmount();
	afx_msg void OnClickCheckTextureOn();
	afx_msg LRESULT OnColorChanged(WPARAM, LPARAM);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()

	friend class CIOREdit;
	void OnChangeAnything(void);

private:
	CRhRdkSubNodeCtrl m_SubNode;
	int m_iInternalCall = 0;
};
