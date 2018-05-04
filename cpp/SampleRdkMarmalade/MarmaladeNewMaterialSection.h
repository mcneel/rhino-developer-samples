
#pragma once

#include "Resource.h"

class CMarmaladeNewMaterialSection : public CRhRdkMaterialUISection_MFC
{
public:
	CMarmaladeNewMaterialSection();

private:
	enum { IDD = IDD_NEW_MATERIAL_SECTION };
	CRhRdkColorButton m_btColor;
	CRhRdkSdkRealNumberEdit m_editTransparency;
	CEdit m_editTexturePlaceholder;

	class CIOREdit : public CRhRdkSdkRealNumberEdit 
	{
	public:
		CIOREdit(CMarmaladeNewMaterialSection& s);

	protected:
		virtual void OnUserChangedValue(void) override { m_Section.OnChangeAnything(); }

	protected:
		afx_msg LRESULT OnContextMenu(WPARAM w, LPARAM l);
		DECLARE_MESSAGE_MAP()

	private:
		CMarmaladeNewMaterialSection& m_Section;
	}
	m_editIOR;

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
	afx_msg void OnKillfocusEditTextureAmount();
	afx_msg void OnClickCheckTextureOn();
	afx_msg LRESULT OnColorChanged(WPARAM, LPARAM);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()

	void OnChangeAnything(void);

private:
	class CSNC : public CRhRdkSubNodeCtrl
	{
	public:
		CSNC(CMarmaladeNewMaterialSection& s) : m_Section(s) { }

	protected:
		virtual void OnCheckChanged(void)  override { m_Section.OnChangeAnything(); }
		virtual void OnAmountChanged(void) override { m_Section.OnChangeAnything(); }

	private:
		CMarmaladeNewMaterialSection& m_Section;
	}
	m_SubNode;

	int m_iInternalCall = 0;
};
