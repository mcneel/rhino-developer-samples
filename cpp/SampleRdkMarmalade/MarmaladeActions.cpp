
#include "stdafx.h"
#include "MarmaladeActions.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeRdkPlugIn.h"
#include "MarmaladeAutoMaterial.h"
#include "MarmaladeActualShaders.h"
#include "Resource.h"

CMarmaladeExtraPeelAction::CMarmaladeExtraPeelAction(const UUID& uuidClient)
	:
	CRhRdkAction(uuidClient)
{
}

UUID CMarmaladeExtraPeelAction::Ident(void)
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {1DAA386E-0568-4cbf-B499-5DF7143BAD01}
		0x1daa386e, 0x568, 0x4cbf, { 0xb4, 0x99, 0x5d, 0xf7, 0x14, 0x3b, 0xad, 0x1 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

bool CMarmaladeExtraPeelAction::Execute(void)
{
	SetChecked(!Checked());

	return true;
}

CMarmaladeFlavorAction::CMarmaladeFlavorAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa)
	:
	CRhRdkAction(uuidClient),
	m_ExtraPeelAction(mpa)
{
}

void CMarmaladeFlavorAction::AddNewMaterial(COLORREF col1, COLORREF col2) const
{
	CMarmaladeOrangeShader os;
	CRhRdkContent* pContent = ::RhRdkCreateContentByType(os.Uuid());
	CMarmaladeAutoMaterial* pMaterial = static_cast<CMarmaladeAutoMaterial*>(pContent);

	const ON_Color col = m_ExtraPeelAction.Checked() ? col2 : col1;
	pMaterial->SetParameter(L"color", col, CRhRdkContent::ccUI);
}

CMarmaladeOrangeAction::CMarmaladeOrangeAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa)
	:
	CMarmaladeFlavorAction(uuidClient, mpa)
{
}

UUID CMarmaladeOrangeAction::Ident(void)
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {22721C3E-FE2E-4227-AD3E-6E6CD4E3DF88}
		0x22721c3e, 0xfe2e, 0x4227, { 0xad, 0x3e, 0x6e, 0x6c, 0xd4, 0xe3, 0xdf, 0x88 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

HICON CMarmaladeOrangeAction::Icon(void) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
}

bool CMarmaladeOrangeAction::Execute(void)
{
	AddNewMaterial(RGB(255, 190, 90), RGB(255, 140, 0));

	return true;
}

CMarmaladeLemonAction::CMarmaladeLemonAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa)
	:
	CMarmaladeFlavorAction(uuidClient, mpa)
{
}

UUID CMarmaladeLemonAction::Ident(void)
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {F1B5A725-566D-468e-AB59-F5440AC21640}
		0xf1b5a725, 0x566d, 0x468e, { 0xab, 0x59, 0xf5, 0x44, 0xa, 0xc2, 0x16, 0x40 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

HICON CMarmaladeLemonAction::Icon(void) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
}

bool CMarmaladeLemonAction::Execute(void)
{
	AddNewMaterial(RGB(250, 250, 160), RGB(255, 255, 0));

	return true;
}

CMarmaladeLimeAction::CMarmaladeLimeAction(const UUID& uuidClient, const CMarmaladeExtraPeelAction& mpa)
	:
	CMarmaladeFlavorAction(uuidClient, mpa)
{
}

UUID CMarmaladeLimeAction::Ident(void)
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {156BE56A-FF90-4238-87A7-C29832FCFA61}
		0x156be56a, 0xff90, 0x4238, { 0x87, 0xa7, 0xc2, 0x98, 0x32, 0xfc, 0xfa, 0x61 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

HICON CMarmaladeLimeAction::Icon(void) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
}

bool CMarmaladeLimeAction::Execute(void)
{
	AddNewMaterial(RGB(160, 250, 160), RGB(0, 255, 0));

	return true;
}

CMarmaladeBreadAction::CMarmaladeBreadAction(const UUID& uuidClient)
	:
	CRhRdkAction(uuidClient)
{
	SetChecked(true);
}

UUID CMarmaladeBreadAction::Ident(void)
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {54172957-38FE-4b81-A951-E0D6AE91740B}
		0x54172957, 0x38fe, 0x4b81, { 0xa9, 0x51, 0xe0, 0xd6, 0xae, 0x91, 0x74, 0x0b }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

CMarmaladeToastAction::CMarmaladeToastAction(const UUID& uuidClient)
	:
	CRhRdkAction(uuidClient)
{
}

UUID CMarmaladeToastAction::Ident(void)
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {5375EFBA-766B-49cb-8B10-3E6291274C8B}
		0x5375efba, 0x766b, 0x49cb, { 0x8b, 0x10, 0x3e, 0x62, 0x91, 0x27, 0x4c, 0x8b }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

CMarmaladeSconeAction::CMarmaladeSconeAction(const UUID& uuidClient)
	:
	CRhRdkAction(uuidClient)
{
}

UUID CMarmaladeSconeAction::Ident(void)
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {E2CB73A3-A5D0-4e17-9CCD-E60AE14E08FF}
		0xe2cb73a3, 0xa5d0, 0x4e17, { 0x9c, 0xcd, 0xe6, 0xa, 0xe1, 0x4e, 0x8, 0xff }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}
