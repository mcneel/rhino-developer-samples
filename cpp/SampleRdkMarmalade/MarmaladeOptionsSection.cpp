
#include "stdafx.h"
#include "MarmaladeOptionsSection.h"

IMPLEMENT_DYNAMIC(CMarmaladeOptionsSection1, CRhRdkExpandingSection)

CMarmaladeOptionsSection1::CMarmaladeOptionsSection1(CWnd* pParent /*=NULL*/)
	: CMarmaladeSettingsSection(CMarmaladeOptionsSection1::IDD, pParent)
{
}

CMarmaladeOptionsSection1::~CMarmaladeOptionsSection1()
{
}

void CMarmaladeOptionsSection1::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMarmaladeOptionsSection1, CMarmaladeSettingsSection)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CMarmaladeOptionsSection2, CMarmaladeSettingsSection)

CMarmaladeOptionsSection2::CMarmaladeOptionsSection2(CWnd* pParent /*=NULL*/)
	:
	CMarmaladeSettingsSection(IDD, pParent)
{
}

CMarmaladeOptionsSection2::~CMarmaladeOptionsSection2()
{
}

void CMarmaladeOptionsSection2::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMarmaladeOptionsSection2, CMarmaladeSettingsSection)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CMarmaladeViewDockBarSection, CRhRdkViewDockBarCustomSection);
