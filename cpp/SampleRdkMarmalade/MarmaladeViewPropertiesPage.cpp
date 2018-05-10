
#include "stdafx.h"
#include "MarmaladeViewPropertiesPage.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeUtilities.h"

// This replaces the obsolete CRhRdkViewDockBarCustomSectionManager and CRhRdkViewDockBarCustomSection.
// This is an example of how custom view properties pages are now implemented.
// As a demonstration, it shows how to allow the user to set and get the view target.

BEGIN_MESSAGE_MAP(CMarmaladeViewPropertiesPage, TRhinoPropertiesPanelPage<CRhinoDialog>)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_X, OnKillfocusEditTarget)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_Y, OnKillfocusEditTarget)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_Z, OnKillfocusEditTarget)
	// TODO: Add your message handlers here.
END_MESSAGE_MAP()

void CMarmaladeViewPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TARGET_X, m_edit_target_x);
	DDX_Control(pDX, IDC_EDIT_TARGET_Y, m_edit_target_y);
	DDX_Control(pDX, IDC_EDIT_TARGET_Z, m_edit_target_z);
	// TODO: Add your controls here.
}

BOOL CMarmaladeViewPropertiesPage::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO: Initialize your controls here.

	return TRUE;
}

void CMarmaladeViewPropertiesPage::UpdatePage(IRhinoPropertiesPanelPageEventArgs& ea)
{
	const auto* pDoc = ea.Document();
	if (nullptr == pDoc)
		return;

	m_bInit = true;

	const auto vsn = pDoc->ActiveRhinoViewSerialNumber();
	const auto* pView = CRhinoView::FromRuntimeSerialNumber(vsn);
	const auto pt = pView->ActiveViewport().Target();
	ON_wString s;
	s.Format(L"%.2f", pt.x);
	m_edit_target_x.SetWindowText(s);
	s.Format(L"%.2f", pt.y);
	m_edit_target_y.SetWindowText(s);
	s.Format(L"%.2f", pt.z);
	m_edit_target_z.SetWindowText(s);

	// TODO: Update your controls here.
}

void CMarmaladeViewPropertiesPage::OnKillfocusEditTarget()
{
	if (!m_bInit || (nullptr == PropertiesPanelPageHost()))
		return;

	const auto dsn = PropertiesPanelPageHost()->DocumentRuntimeSerialNumber();
	auto* pDoc = CRhinoDoc::FromRuntimeSerialNumber(dsn);
	if (nullptr == pDoc)
		return;

	const auto vsn = pDoc->ActiveRhinoViewSerialNumber();
	const auto* pView = CRhinoView::FromRuntimeSerialNumber(vsn);

	CString s;
	ON_3dVector vec;
	m_edit_target_x.GetWindowText(s);
	vec.x = StringToFloat(s);
	m_edit_target_y.GetWindowText(s);
	vec.y = StringToFloat(s);
	m_edit_target_z.GetWindowText(s);
	vec.z = StringToFloat(s);
	pView->ActiveViewport().SetTarget(vec);

	pDoc->DeferredRedraw();
}

bool CMarmaladeViewPropertiesPage::IncludeInNavigationControl(IRhinoPropertiesPanelPageEventArgs&) const
{
	// Only include this page when Marmalade is the current renderer.
	return RhinoApp().GetDefaultRenderApp() == MarmaladePlugIn().PlugInID();
}

CRhinoCommand::result CMarmaladeViewPropertiesPage::RunScript(IRhinoPropertiesPanelPageEventArgs& e)
{
	return CRhinoCommand::failure;
}
