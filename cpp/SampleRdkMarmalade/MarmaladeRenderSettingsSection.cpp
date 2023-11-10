
#include "stdafx.h"
#include "MarmaladeRenderSettingsSection.h"
#include "MarmaladePlugIn.h"

CMarmaladeRenderSettingsSection::CMarmaladeRenderSettingsSection(RhRdkUiModalities m, int idd)
	:
	m_Modality(m),
	CRhRdkRenderSettingsSection_MFC(m, idd)
{
}

CMarmaladeRenderSettingsSection::~CMarmaladeRenderSettingsSection()
{
}

BOOL CMarmaladeRenderSettingsSection::OnInitDialog()
{
	CRhRdkRenderSettingsSection_MFC::OnInitDialog();

	return TRUE;
}

void CMarmaladeRenderSettingsSection::DisplayData(void)
{
	CRhRdkRenderSettingsSection_MFC::DisplayData();

	m_bIsHidden = true;

	if (RhinoApp().GetDefaultRenderApp() == MarmaladePlugIn().PlugInID())
	{
		m_bIsHidden = (m_bExpertMode ^ IsDetailed()) ? true : false;
	}
}

bool CMarmaladeRenderSettingsSection::IsHidden(void) const
{
	return m_bIsHidden;
}

void CMarmaladeRenderSettingsSection::OnEvent(IRhinoUiController& con, const UUID& uuidData, IRhinoUiController::EventPriority ep, const IRhinoUiEventInfo* pInfo)
{
	// Not called; not implemented.
}

BEGIN_MESSAGE_MAP(CMarmaladeRenderSettingsSection1, CMarmaladeRenderSettingsSection)
	ON_BN_CLICKED(IDC_CHECK_EXPERT_MODE, OnCheckExpertMode)
END_MESSAGE_MAP()

void CMarmaladeRenderSettingsSection1::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_CHECK_EXPERT_MODE, m_check_expert_mode);
	__super::DoDataExchange(pDX);
}

void CMarmaladeRenderSettingsSection1::DisplayData(void)
{
	CMarmaladeRenderSettingsSection::DisplayData();

	m_check_expert_mode.SetCheck(m_bExpertMode ? BST_CHECKED : BST_UNCHECKED);
}

void CMarmaladeRenderSettingsSection1::OnCheckExpertMode()
{
	m_bExpertMode = m_check_expert_mode.GetCheck() == BST_CHECKED;
}

BEGIN_MESSAGE_MAP(CMarmaladeRenderSettingsSection2, CMarmaladeRenderSettingsSection)
END_MESSAGE_MAP()

void CMarmaladeRenderSettingsSection2::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}
