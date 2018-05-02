
#pragma once

#include "Resource.h"

class CMarmaladeAutoMaterial;

class CMarmaladeColorSection : public CRhRdkMaterialUISection_MFC
{
public:
	CMarmaladeColorSection();
	virtual ~CMarmaladeColorSection();

private:
	enum { IDD = IDD_COLOR_SECTION };
	CRhRdkColorButton m_buttonColor;

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual UUID Uuid(void) const override;
	virtual void DisplayData(void) override;
	virtual unsigned int GetIDD(void) const override { return IDD; }
	virtual ON_wString Caption(bool bAlwaysEnglish) const override { return L"Marmalade color"; }
	virtual AFX_MODULE_STATE* GetModuleState(void) const override { return AfxGetStaticModuleState(); }

protected:
	afx_msg LRESULT OnColorButton(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};
