
#include "stdafx.h"
#include "MarmaladeColorSection.h"
#include "MarmaladeAutoMaterial.h"

static UINT uChangedMessageNumber = 0;

BEGIN_MESSAGE_MAP(CMarmaladeColorSection, CRhRdkMaterialUISection_MFC)
	ON_REGISTERED_MESSAGE(uChangedMessageNumber, OnColorButton)
END_MESSAGE_MAP()

CMarmaladeColorSection::CMarmaladeColorSection()
	:
	CRhRdkMaterialUISection_MFC(IDD)
{
	uChangedMessageNumber = m_buttonColor.ChangedMessageNumber();
}

CMarmaladeColorSection::~CMarmaladeColorSection()
{
}

void CMarmaladeColorSection::DoDataExchange(CDataExchange* pDX)
{
	CRhRdkMaterialUISection_MFC::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_MATERIAL_COLOR, m_buttonColor);
}

BOOL CMarmaladeColorSection::OnInitDialog()
{
	CRhRdkMaterialUISection_MFC::OnInitDialog();

	DisplayData();

	return FALSE;
}

static const wchar_t* wszColor = L"color";

void CMarmaladeColorSection::DisplayData(void)
{
	if (NULL == Material())
		return;

	const CRhRdkColor col = Material()->GetParameter(wszColor).AsRdkColor();
	m_buttonColor.SetColor(col);
}

LRESULT CMarmaladeColorSection::OnColorButton(WPARAM, LPARAM)
{
	if (NULL == Material())
		return 0;

	const CRhRdkColor colOld = Material()->GetParameter(wszColor).AsRdkColor();
	const CRhRdkColor colNew = m_buttonColor.GetColor();
	if (colNew != colOld)
	{
		Material()->SetParameter(wszColor, colNew.OnColor(), CRhRdkContent::ccUI);
	}

	return 1;
}

UUID CMarmaladeColorSection::Uuid(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {9CF98E8B-407D-4852-975F-AC3720E20C86}
		0x9cf98e8b, 0x407d, 0x4852, { 0x97, 0x5f, 0xac, 0x37, 0x20, 0xe2, 0xc, 0x86 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}
