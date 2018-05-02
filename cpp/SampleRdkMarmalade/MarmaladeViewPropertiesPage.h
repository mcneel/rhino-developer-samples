
#pragma once

#include "Resource.h"

class CMarmaladeViewPropertiesPage : public TRhinoPropertiesPanelPage<CRhinoDialog>
{
public:
	CMarmaladeViewPropertiesPage(CWnd* pParent=nullptr)
		:
		TRhinoPropertiesPanelPage<CRhinoDialog>(IDD, IDI_MARMALADE, true /* Auto-delete */)
	{
	}

private:
	enum { IDD = IDD_VIEW_PROPERTIES };
	CEdit m_edit_target_x;
	CEdit m_edit_target_y;
	CEdit m_edit_target_z;

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual const wchar_t* EnglishTitle() const override { return L"Marmalade"; }
	virtual const wchar_t* LocalTitle() const override { return EnglishTitle(); }
	virtual bool IncludeInNavigationControl(IRhinoPropertiesPanelPageEventArgs& e) const override;
	virtual CRhinoCommand::result RunScript(IRhinoPropertiesPanelPageEventArgs& e) override;
	virtual void UpdatePage(IRhinoPropertiesPanelPageEventArgs& e) override;
	virtual int Index(void) const override { return 100; }
	virtual RhinoPropertiesPanelPageType PageType() const { return RhinoPropertiesPanelPageType::View; }

protected:
	afx_msg void OnKillfocusEditTarget();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bInit = false;
};
