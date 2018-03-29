
#pragma once

#include "Resource.h"

class CMarmaladeAutoMaterial;

class CMarmaladeColorSection : public CRhRdkMaterialUISection
{
public:
	CMarmaladeColorSection();
	virtual ~CMarmaladeColorSection();

private:
	enum { IDD = IDD_COLOR_SECTION };
	CRhRdkColorButton m_buttonColor;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual UUID Uuid(void) const;
	virtual void DisplayData(void);
	virtual ON_wString Caption(void) const { return L"Marmalade color"; }
	virtual AFX_MODULE_STATE* GetModuleState(void) const { return AfxGetStaticModuleState(); }

protected:
	afx_msg LRESULT OnColorButton(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};
