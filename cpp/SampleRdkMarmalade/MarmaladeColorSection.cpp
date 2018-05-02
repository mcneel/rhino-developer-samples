
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
	const auto con = Controller();
	if (!con)
		return;

	CRhRdkColor col;
	bool bColVaries = false;

	CRhRdkEditableContentArray aContent(*con, false);
	for (int i = 0; i < aContent.Count(); i++)
	{
		const auto colThis = aContent[i]->GetParameter(wszColor).AsRdkColor();

		if (0 == i)
		{
			col = colThis;
		}
		else
		{
			if (col != colThis)
				bColVaries = true;
		}
	}

	m_buttonColor.SetColor(col);
	m_buttonColor.SetVaries(bColVaries);
}

LRESULT CMarmaladeColorSection::OnColorButton(WPARAM, LPARAM)
{
	const auto con = Controller();
	if (!con)
		return 0;

	const auto colNew = m_buttonColor.GetColor();

	CRhRdkEditableContentArray aContent(*con, true);
	for (int i = 0; i < aContent.Count(); i++)
	{
		const CRhRdkContent::Change<CRhRdkContent> m(*aContent[i], RhRdkChangeContext::UI);

		//const auto colOld = m().GetParameter(wszColor).AsRdkColor();
		//if (colNew != colOld)
		{
			m().SetParameter(wszColor, colNew.OnColor());
		}
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
