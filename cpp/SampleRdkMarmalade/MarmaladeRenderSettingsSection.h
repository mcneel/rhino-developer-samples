
#pragma once

#include "MarmaladePlugIn.h"
#include "Resource.h"

// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
static const UUID uuid_options_1  = { 0x862814b2, 0x3f3d, 0x41e9, { 0xaf, 0x91, 0x8b, 0x0f, 0xe9, 0x11, 0x5d, 0xd8 } };
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
static const UUID uuid_options_2  = { 0x1e098c18, 0x7082, 0x4ff5, { 0x94, 0x8c, 0x78, 0xd8, 0x4c, 0xe5, 0x3c, 0xfc } };
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
static const UUID uuid_options_1D = { 0xfb9c5008, 0x0152, 0x4fc7, { 0x99, 0xfe, 0x9d, 0xf3, 0xd7, 0x09, 0x60, 0x4d } };
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!
static const UUID uuid_options_2D = { 0x607bea87, 0x6ae7, 0x4d32, { 0x8b, 0xc9, 0xc9, 0x89, 0x9e, 0x19, 0x51, 0x4d } };
// !!!! DO NOT REUSE THIS UUID !!!!
// !!!! DO NOT REUSE THIS UUID !!!!

class CMarmaladeRenderSettingsSection : public CRhRdkRenderSettingsSection_MFC
{
public:
	CMarmaladeRenderSettingsSection(RhRdkUiModalities m, int idd);
	virtual ~CMarmaladeRenderSettingsSection();

protected:
	virtual bool IsDetailed(void) const { return false; }

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DisplayData(void) override;
	virtual bool IsHidden(void) const;
	virtual UUID PlugInId(void) const override { return MarmaladePlugIn().PlugInID(); }
	virtual AFX_MODULE_STATE* GetModuleState(void) const override { return AfxGetStaticModuleState(); }
	virtual void OnEvent(IRhinoUiController& con, const UUID& uuidData, IRhinoUiController::EventPriority ep, const IRhinoUiEventInfo* pInfo) override;
	virtual CRhinoCommandOptionName CommandOptionName(void) const override { return CRhinoCommandOptionName(L"Marmalade", L"Marmalade"); }
	virtual CRhinoCommand::result RunScript(CRhRdkControllerPtr) override { return CRhinoCommand::result::success; }

protected:
	RhRdkUiModalities m_Modality;
	bool m_bExpertMode = false;
	bool m_bIsHidden = true;
};

class CMarmaladeRenderSettingsSection1 : public CMarmaladeRenderSettingsSection
{
private:
	enum { IDD = IDD_OPTIONS_SECTION_1 };

public:
	CMarmaladeRenderSettingsSection1(RhRdkUiModalities m) : CMarmaladeRenderSettingsSection(m, IDD) { }

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void DisplayData(void) override;
	virtual unsigned int GetIDD(void) const override { return IDD; }
	virtual UUID Uuid(void) const override { return uuid_options_1; }
	virtual ON_wString Caption(bool bAlwaysEnglish) const override { return L"Marmalade Options 1"; }

protected:
	afx_msg void OnCheckExpertMode();
	DECLARE_MESSAGE_MAP()

private:
	CButton m_check_expert_mode;
};

class CMarmaladeRenderSettingsSection1_Detailed : public CMarmaladeRenderSettingsSection1
{
public:
	CMarmaladeRenderSettingsSection1_Detailed(RhRdkUiModalities m) : CMarmaladeRenderSettingsSection1(m) { }

protected:
	virtual UUID Uuid(void) const override { return uuid_options_1D; }
	virtual ON_wString Caption(bool bAlwaysEnglish) const override { return L"DETAILED Marmalade Options 1"; }
	virtual bool IsDetailed(void) const override { return true; }
};

class CMarmaladeRenderSettingsSection2 : public CMarmaladeRenderSettingsSection
{
private:
	enum { IDD = IDD_OPTIONS_SECTION_2 };

public:
	CMarmaladeRenderSettingsSection2(RhRdkUiModalities m) : CMarmaladeRenderSettingsSection(m, IDD) { }

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual UUID Uuid(void) const override { return uuid_options_2; }
	virtual unsigned int GetIDD(void) const override { return IDD; }
	virtual ON_wString Caption(bool bAlwaysEnglish) const override { return L"Marmalade Options 2"; }
	DECLARE_MESSAGE_MAP()
};

class CMarmaladeRenderSettingsSection2_Detailed : public CMarmaladeRenderSettingsSection2
{
public:
	CMarmaladeRenderSettingsSection2_Detailed(RhRdkUiModalities m) : CMarmaladeRenderSettingsSection2(m) { }

protected:
	virtual UUID Uuid(void) const { return uuid_options_2D; }
	virtual ON_wString Caption(bool bAlwaysEnglish) const { return L"DETAILED Marmalade Options 2"; }
	virtual bool IsDetailed(void) const override { return true; }
};
