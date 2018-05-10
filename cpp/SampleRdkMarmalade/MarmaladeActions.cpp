
#include "stdafx.h"
#include "MarmaladeActions.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeRdkPlugIn.h"
#include "MarmaladeAutoMaterial.h"
#include "MarmaladeActualShaders.h"
#include "Resource.h"

#if defined RHINO_SDK_6_5_OR_NEWER

static bool ConvertIconToDib(HICON hIcon, CRhinoDib& dibOut)
{
	ICONINFO info = { 0 };
	if (!GetIconInfo(hIcon, &info))
		return false;

	// This just assumes it's a 32-bit dib with alpha in it.
	dibOut.SetBitmap(info.hbmColor);

	VERIFY(::DeleteObject(info.hbmColor));
	VERIFY(::DeleteObject(info.hbmMask));

	return true;
}

bool CMarmaladeExtraPeelTask::IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const
{
	return ::RhRdkGetMenuIcon(RhRdkMenuIcons::Check, ON_2iSize(width, height), dib);
}

UUID CMarmaladeExtraPeelTask::Ident(void)
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

void CMarmaladeExtraPeelTask::Update(IRhRdkTaskUpdate& tu) const
{
	tu.SetIsChecked(m_bChecked);
}

CRhRdkTask::Result CMarmaladeExtraPeelTask::Execute(const IRhRdkTaskOrigin& origin) const
{
	m_bChecked = !m_bChecked;

	return Result::Success;
}

CMarmaladeFlavorTask::CMarmaladeFlavorTask(const CMarmaladeExtraPeelTask& mpa)
	:
	m_ExtraPeelAction(mpa)
{
}

void CMarmaladeFlavorTask::AddNewMaterial(COLORREF col1, COLORREF col2) const
{
	// This is just a demonstration to make the task do something.
	// It is not a good idea to add custom tasks that create materials
	// because the RDK already has its own Create New Material tasks.

	const auto rhino_doc_sn = CRhinoDoc::ModelessUserInterfaceDocSerialNumber();
	const auto* pDoc = CRhinoDoc::FromRuntimeSerialNumber(rhino_doc_sn);
	if (nullptr == pDoc)
		return;

	const auto& rdkDocConst = CRhRdkDocument::Get(*pDoc);
	auto& rdkDoc = rdkDocConst.BeginChange(RhRdkChangeContext::UI);

	CMarmaladeOrangeShader os;
	const auto* pContent = rdkDoc.CreateContentByType(os.Uuid());

	{	CRhRdkContent::Change<CRhRdkContent> c(*pContent);
		const auto col = CRhRdkColor(m_ExtraPeelAction.IsChecked() ? col2 : col1);
		c().SetParameter(L"color", col);
	}

	rdkDoc.EndChange();
}

CMarmaladeOrangeTask::CMarmaladeOrangeTask(const CMarmaladeExtraPeelTask& mpa)
	:
	CMarmaladeFlavorTask(mpa)
{
}

UUID CMarmaladeOrangeTask::Ident(void)
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

bool CMarmaladeOrangeTask::IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MARMALADE));
	return ConvertIconToDib(hIcon, dib);
}

CRhRdkTask::Result CMarmaladeOrangeTask::Execute(const IRhRdkTaskOrigin& origin) const
{
	AddNewMaterial(RGB(255, 190, 90), RGB(255, 140, 0));

	return Result::Success;
}

CMarmaladeLemonTask::CMarmaladeLemonTask(const CMarmaladeExtraPeelTask& mpa)
	:
	CMarmaladeFlavorTask(mpa)
{
}

UUID CMarmaladeLemonTask::Ident(void)
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

bool CMarmaladeLemonTask::IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MARMALADE));
	return ConvertIconToDib(hIcon, dib);
}

CRhRdkTask::Result CMarmaladeLemonTask::Execute(const IRhRdkTaskOrigin& origin) const
{
	AddNewMaterial(RGB(250, 250, 160), RGB(255, 255, 0));

	return Result::Success;
}

CMarmaladeLimeTask::CMarmaladeLimeTask(const CMarmaladeExtraPeelTask& mpa)
	:
	CMarmaladeFlavorTask(mpa)
{
}

UUID CMarmaladeLimeTask::Ident(void)
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

bool CMarmaladeLimeTask::IconOut(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MARMALADE));
	return ConvertIconToDib(hIcon, dib);
}

CRhRdkTask::Result CMarmaladeLimeTask::Execute(const IRhRdkTaskOrigin& origin) const
{
	AddNewMaterial(RGB(160, 250, 160), RGB(0, 255, 0));

	return Result::Success;
}

enum class Choices { Bread, Toast, Scone };

static Choices Choice = Choices::Bread;

void CMarmaladeBreadAction::Update(IRhRdkTaskUpdate& tu) const
{
	tu.SetRadio(Choices::Bread == Choice);
}

CRhRdkTask::Result CMarmaladeBreadAction::Execute(const IRhRdkTaskOrigin&) const
{
	Choice = Choices::Bread;

	return Result::Success;
}

bool CMarmaladeBreadAction::IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const
{
	return ::RhRdkGetMenuIcon(RhRdkMenuIcons::Radio, ON_2iSize(width, height), dib);
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

bool CMarmaladeToastAction::IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const
{
	return ::RhRdkGetMenuIcon(RhRdkMenuIcons::Radio, ON_2iSize(width, height), dib);
}

void CMarmaladeToastAction::Update(IRhRdkTaskUpdate& tu) const
{
	tu.SetRadio(Choices::Toast == Choice);
}

CRhRdkTask::Result CMarmaladeToastAction::Execute(const IRhRdkTaskOrigin&) const
{
	Choice = Choices::Toast;

	return Result::Success;
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

bool CMarmaladeSconeAction::IconIn(CRhRdkContent::Kinds kind, int width, int height, OUT CRhinoDib& dib) const
{
	return ::RhRdkGetMenuIcon(RhRdkMenuIcons::Radio, ON_2iSize(width, height), dib);
}

void CMarmaladeSconeAction::Update(IRhRdkTaskUpdate& tu) const
{
	tu.SetRadio(Choices::Scone == Choice);
}

CRhRdkTask::Result CMarmaladeSconeAction::Execute(const IRhRdkTaskOrigin&) const
{
	Choice = Choices::Scone;

	return Result::Success;
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

#endif
