
#include "stdafx.h"
#include "MarmaladeRenderSettingsSection.h"

static const UUID uuidData_MarmRS = { 0xc793fe5c, 0xf46e, 0x4b88, { 0x96, 0x18, 0xff, 0x84, 0xf, 0x93, 0xf4, 0x36 } };

class CDataSource : public CRhRdkDataSource
{
public:
	bool ExpertMode(void) const { return m_bExpertMode; }
	void SetExpertMode(bool b) { if (m_bExpertMode != b) { m_bExpertMode = b; SetChanged(); } }

protected:
	virtual void* GetData(const UUID& uuidData, bool bForWrite, bool bAutoChangeBracket=true) const override
	{
		if (!PrepareGetData(uuidData, bForWrite, bAutoChangeBracket))
			return nullptr;

		return (void*)this;
	}

	virtual void SupportedUuidData(OUT ON_SimpleArray<UUID>& aUuidData) const override
	{
		aUuidData.Append(uuidData_MarmRS);
	}

private:
	bool m_bExpertMode = false;
};

IRhinoUiDataSource* CMarmaladeRenderSettingsSection::m_pDS = nullptr;
int CMarmaladeRenderSettingsSection::m_iCount = 0;

CMarmaladeRenderSettingsSection::CMarmaladeRenderSettingsSection(RhRdkUiModalities m, int idd)
	:
	m_Modality(m),
	CRhRdkRenderSettingsSection_MFC(m, idd)
{
}

CMarmaladeRenderSettingsSection::~CMarmaladeRenderSettingsSection()
{
	if (0 == --m_iCount)
	{
		m_pDS->DeleteThis();
		m_pDS = nullptr;
	}
}

BOOL CMarmaladeRenderSettingsSection::OnInitDialog()
{
	CRhRdkRenderSettingsSection_MFC::OnInitDialog();

	if (0 == m_iCount++)
	{
		m_pDS = new CDataSource;

		if (Controller())
		{
			Controller()->AddDataSource(*m_pDS);
		}
	}

	return TRUE;
}

bool CMarmaladeRenderSettingsSection::IsHidden(void) const
{
	const auto* pDS = static_cast<CDataSource*>(GetData(uuidData_MarmRS, false));
	if (nullptr == pDS)
		return true;

	return pDS->ExpertMode() ^ IsDetailed() ? true : false;
}

void CMarmaladeRenderSettingsSection::OnEvent(IRhinoUiController& con, const UUID& uuidData, IRhinoUiController::EventPriority ep, const IRhinoUiEventInfo* pInfo)
{
	if (uuidData_MarmRS == uuidData)
	{
		DisplayData();
	}
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

	const auto con = Controller();
	if (!con)
		return;

	const auto* pDS = static_cast<CDataSource*>(con->GetData(uuidData_MarmRS, false));
	if (nullptr != pDS)
	{
		m_check_expert_mode.SetCheck(pDS->ExpertMode() ? BST_CHECKED : BST_UNCHECKED);
	}
}

void CMarmaladeRenderSettingsSection1::OnCheckExpertMode()
{
	const auto con = Controller();
	if (!con)
		return;

	auto* pDS = static_cast<CDataSource*>(con->GetData(uuidData_MarmRS, true));
	if (nullptr != pDS)
	{
		pDS->SetExpertMode(m_check_expert_mode.GetCheck() == BST_CHECKED);

		con->Commit(uuidData_MarmRS);
	}
}

BEGIN_MESSAGE_MAP(CMarmaladeRenderSettingsSection2, CMarmaladeRenderSettingsSection)
END_MESSAGE_MAP()

void CMarmaladeRenderSettingsSection2::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}
