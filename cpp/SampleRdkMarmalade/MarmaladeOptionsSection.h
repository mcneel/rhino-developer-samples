
#pragma once

#include "resource.h"

// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
static const GUID uuid_options_1 = { 0x5e3b9261, 0x881f, 0x46fe, { 0xb2, 0xba, 0xb8, 0x38, 0xef, 0xd2, 0x67, 0x14 } };
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
static const GUID uuid_options_2 = { 0x5e3b9261, 0x881f, 0x46fe, { 0xb2, 0xba, 0xb8, 0x38, 0xef, 0xd2, 0x67, 0x14 } };
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!

class CMarmaladeSettingsSection : public CRhRdkExpandingSection
{
public:
	CMarmaladeSettingsSection(int iIDD, CWnd* pParent = NULL) : CRhRdkExpandingSection(iIDD, pParent) {}
	virtual bool IsDetailed(void) const { return false; }
};

class CMarmaladeOptionsSection1 : public CMarmaladeSettingsSection
{
	enum { IDD = IDD_OPTIONS_SECTION_1 };
	DECLARE_DYNAMIC(CMarmaladeOptionsSection1)

public:
	CMarmaladeOptionsSection1(CWnd* pParent=NULL);
	virtual ~CMarmaladeOptionsSection1();

	virtual UUID Uuid(void) const { return uuid_options_1; }
	virtual ON_wString Caption(void) const { return L"Marmalade Options 1"; }
	virtual AFX_MODULE_STATE* GetModuleState(void) const { return AfxGetStaticModuleState(); }
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

class CMarmaladeOptionsSection1_Detailed : public CMarmaladeOptionsSection1
{
	virtual ON_wString Caption(void) const { return L"DETAILED Marmalade Options 1"; }
	bool IsDetailed(void) const { return true; }
};

class CMarmaladeOptionsSection2 : public CMarmaladeSettingsSection
{
	enum { IDD = IDD_OPTIONS_SECTION_2 };
	DECLARE_DYNAMIC(CMarmaladeOptionsSection2)

public:
	CMarmaladeOptionsSection2(CWnd* pParent = NULL);
	virtual ~CMarmaladeOptionsSection2();

	virtual UUID Uuid(void) const { return uuid_options_2; }
	virtual ON_wString Caption(void) const { return L"Marmalade Options 2"; }
	virtual AFX_MODULE_STATE* GetModuleState(void) const { return AfxGetStaticModuleState(); }

	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

class CMarmaladeOptionsSection2_Detailed : public CMarmaladeOptionsSection2
{
	virtual ON_wString Caption(void) const { return L"DETAILED Marmalade Options 2"; }
	bool IsDetailed(void) const { return true; }
};

// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
static const GUID uuid_view_dock_bar = { 0xae3a9261, 0xa81f, 0xa6fe, { 0xb2, 0xba, 0xb8, 0x38, 0xef, 0xd2, 0x67, 0x14 } };
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!

class CMarmaladeViewDockBarSection : public CRhRdkViewDockBarCustomSection
{
	enum { IDD = IDD_OPTIONS_SECTION_2 };

public:
	DECLARE_DYNAMIC(CMarmaladeViewDockBarSection)
	CMarmaladeViewDockBarSection() : CRhRdkViewDockBarCustomSection(IDD) {}

	virtual UUID Uuid(void) const { return uuid_view_dock_bar; }
	virtual ON_wString Caption(void) const { return L"Marmalade View Dock Bar Panel"; }
	virtual AFX_MODULE_STATE* GetModuleState(void) const { return AfxGetStaticModuleState(); }
};
