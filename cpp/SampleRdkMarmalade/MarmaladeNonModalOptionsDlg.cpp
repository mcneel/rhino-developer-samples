
#include "stdafx.h"
#include "MarmaladeNonModalOptionsDlg.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeOptionsSection.h"

static const int iTimerId = 1023961;

BOOL CALLBACK Marm_ClipChildrenProc(HWND hwndCtrl,LPARAM lParam);
void Marm_ClipChildren(CWnd* pThis, CDC* pDC);

class CMarmaladeMainOptionsHolder : public CRhRdkExpandingSectionHolder
{
public:
	void SetExpertMode(bool bExpert);
	int  ExpertMode(void) const;
};

int CMarmaladeMainOptionsHolder::ExpertMode(void) const
{
	bool bDetailed = false;

	int count = 0;

	const int iSectionCount = GetSectionCount();

	for (int i = 0; i < iSectionCount; i++)
	{
		const SECTION_RECORD* pRec = GetSectionInfo(i);
		if (!pRec)
			continue;

		CMarmaladeSettingsSection* pSection = dynamic_cast<CMarmaladeSettingsSection*>(pRec->pSection);
		if ((NULL != pSection) && IsSectionShown(pSection->SectionIndex()))
		{
			if (0 == count++)
			{
				bDetailed = pSection->IsDetailed();
			}
			else
			{
				if (pSection->IsDetailed() != bDetailed)
					return BST_INDETERMINATE;
			}
		}
	}

	return bDetailed ? BST_CHECKED : BST_UNCHECKED;
}

void CMarmaladeMainOptionsHolder::SetExpertMode(bool bExpert)
{
	const int count = GetSectionCount();

	for (int i = 0; i < count; i++)
	{
		const SECTION_RECORD* pRec = GetSectionInfo(i);
		if (NULL == pRec)
			continue;

		CMarmaladeSettingsSection* pSection = dynamic_cast<CMarmaladeSettingsSection*>(pRec->pSection);
		if (NULL != pSection)
		{
			if (pSection->IsDetailed())
			{
				ShowSection(i, bExpert);
			}
			else
			{
				ShowSection(i, !bExpert);
			}
		}
	}
}

CMarmaladeNonModalOptionsDlg::CMarmaladeNonModalOptionsDlg()
	:
	m_bIsVisible(false),
	m_bWasVisible(false),
	CDialog(IDD)
{
	m_pHolder = new CMarmaladeMainOptionsHolder;

	m_iBigHeight = ::GetSystemMetrics(SM_CYFULLSCREEN) * 2 / 3;

	Create(IDD);
}

CMarmaladeNonModalOptionsDlg::~CMarmaladeNonModalOptionsDlg()
{
	delete m_pHolder;
	m_pHolder = NULL;
}

void CMarmaladeNonModalOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_RENDER, m_btRender);
	DDX_Control(pDX, IDC_CHECK_EXPERT, m_check_expert);
}

BEGIN_MESSAGE_MAP(CMarmaladeNonModalOptionsDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_NCLBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BT_RENDER, OnBtRender)
	ON_BN_CLICKED(IDC_CHECK_EXPERT, OnCheckExpert)
END_MESSAGE_MAP()

BOOL CMarmaladeNonModalOptionsDlg::OnEraseBkgnd(CDC* pDC)
{
	// This code solves flickering... comment it out to see the results!
	Marm_ClipChildren(this, pDC);
	const BOOL b = __super::OnEraseBkgnd(pDC);
	pDC->SelectClipRgn(NULL);

	return b;
}

static HICON Icon16x16(DWORD dwIcon)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(dwIcon), IMAGE_ICON, 16, 16, LR_SHARED);
}

BOOL CMarmaladeNonModalOptionsDlg::OnInitDialog()
{
	__super::OnInitDialog();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SetIcon(Icon16x16(IDI_MARMALADE), false);

	GetClientRect(m_rectClient);

	// Get the holder's rect and delete the frame placeholder.
	CWnd* pFrame = GetDlgItem(IDC_STATIC_FRAME);
	ASSERT(NULL != pFrame);
	pFrame->GetWindowRect(m_rectHolder);
	pFrame->DestroyWindow();
	ScreenToClient(m_rectHolder);

	if (NULL == m_pHolder)
		return FALSE;

	// Create the holder.
	m_pHolder->Create(_T(""), m_rectHolder, this);

	// Create section 1 and attach it to the holder.
	CRhRdkExpandingSection* pSection1 = new CMarmaladeOptionsSection1;
	pSection1->CreateAndAttachToHolder(m_pHolder);

	// Create section 2 and attach it to the holder.
	CRhRdkExpandingSection* pSection2 = new CMarmaladeOptionsSection2;
	pSection2->CreateAndAttachToHolder(m_pHolder);

	// Create section 3 and attach it to the holder.
	CRhRdkExpandingSection* pSection3 = new CMarmaladeOptionsSection1_Detailed;
	pSection3->CreateAndAttachToHolder(m_pHolder);

	// Create section 4 and attach it to the holder.
	CRhRdkExpandingSection* pSection4 = new CMarmaladeOptionsSection2_Detailed;
	pSection4->CreateAndAttachToHolder(m_pHolder);

	m_pHolder->SetExpertMode(false);

	const int iCheck = m_pHolder->ExpertMode();
	m_check_expert.SetCheck(iCheck);

	return FALSE;
}

void CMarmaladeNonModalOptionsDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (!::IsWindow(m_btRender.GetSafeHwnd()))
		return;

	CRect rectButton;
	m_check_expert.GetClientRect(rectButton);

	const int y = cy - rectButton.bottom - 8;

	const int iLeftExpert =  (m_rectHolder.right - rectButton.right) - 117;

	m_check_expert.SetWindowPos(NULL, iLeftExpert, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_btRender.GetClientRect(rectButton);
	const int iLeftRender = m_rectHolder.right - rectButton.right;
	m_btRender.SetWindowPos(NULL, iLeftRender, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_rectHolder.bottom = y - 8;

	if (::IsWindow(m_pHolder->GetSafeHwnd()))
	{
		m_pHolder->MoveWindow(m_rectHolder);
	}
}

void CMarmaladeNonModalOptionsDlg::OnRendererChanged(const UUID& uuidOldRenderEngine, const UUID& uuidNewRenderEngine)
{
	const UUID uuidMarmalade = MarmaladePlugIn().PlugInID();

	if (uuidOldRenderEngine == uuidMarmalade)
	{
		m_bWasVisible = IsWindowVisible() ? true : false;

		ShowWindow(SW_HIDE);
	}
	else
	if ((uuidNewRenderEngine == uuidMarmalade) && m_bWasVisible)
	{
		ShowWindow(SW_SHOWNA);
	}
}

LRESULT CMarmaladeNonModalOptionsDlg::OnNcHitTest(CPoint pt)
{
	const LRESULT ht = __super::OnNcHitTest(pt);

	switch (ht)
	{
	case HTBOTTOMLEFT:
	case HTBOTTOMRIGHT:
	case HTTOPLEFT:
	case HTTOPRIGHT:
	case HTLEFT:
	case HTRIGHT:
		return HTNOWHERE;
	}

	return ht;
}

void CMarmaladeNonModalOptionsDlg::OnClose()
{
	m_bIsVisible = false;

	ShowWindow(SW_HIDE);

	__super::OnClose();
}

void CMarmaladeNonModalOptionsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	__super::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_bIsVisible = true;
	}
}

void CMarmaladeNonModalOptionsDlg::OnBtRender()
{
	RhinoApp().RunScript(L"-_Render", 0);
}

void CMarmaladeNonModalOptionsDlg::OnCheckExpert()
{
	const bool bExpert = (BST_CHECKED != m_check_expert.GetCheck());
	m_check_expert.SetCheck(bExpert ? BST_CHECKED : BST_UNCHECKED);

	if (m_pHolder)
		m_pHolder->SetExpertMode(bExpert);
}

void CMarmaladeNonModalOptionsDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	__super::OnNcLButtonDblClk(nHitTest, point);

	const int m_iSmallHeight = ::GetSystemMetrics(SM_CYCAPTION) + 2 * ::GetSystemMetrics(SM_CYSIZEFRAME);

	CRect rectMove;
	GetWindowRect(rectMove);

	CRect rect = rectMove;

	if (rect.Height() > m_iSmallHeight)
	{
		// Change to a small bar.
		m_iBigHeight = rect.Height();
		rect.bottom = rect.top + m_iSmallHeight;
	}
	else
	{
		// Go back to normal.
		rect.bottom = rect.top + m_iBigHeight;
	}

	MoveWindow(rect);
}

// Anti-flicker code.
class CLIP_CHILDREN_PROC_DATA
{
public:
	CLIP_CHILDREN_PROC_DATA(HWND h, CDC& dc) : m_hwndDialog(h), m_dcClip(dc) { }

	inline HWND DialogHWnd(void) const { return m_hwndDialog; }
	inline CDC& ClipDC(void) const { return m_dcClip; }

private:
	HWND m_hwndDialog;
	CDC& m_dcClip;
};

BOOL CALLBACK Marm_ClipChildrenProc(HWND hwndCtrl, LPARAM lParam)
{
	if (!::IsWindow(hwndCtrl) || (0 == lParam))
		return TRUE;

	const UINT style = ::GetWindowLong(hwndCtrl, GWL_STYLE); 
	if (0 == (style & WS_VISIBLE))
		return TRUE;

	CLIP_CHILDREN_PROC_DATA& cpd = *((CLIP_CHILDREN_PROC_DATA*)lParam);

	if (::GetParent(hwndCtrl) != cpd.DialogHWnd())
		return TRUE;

	const CWnd* pDlg = CWnd::FromHandle(cpd.DialogHWnd());

	CRect rcChild;
	::GetWindowRect(hwndCtrl, rcChild);
	pDlg->ScreenToClient(rcChild);

	cpd.ClipDC().ExcludeClipRect(rcChild);

	return TRUE;
}

void Marm_ClipChildren(CWnd* pThis, CDC* pDC)
{
	if ((NULL == pThis) || (NULL == pDC))
		return;

	CLIP_CHILDREN_PROC_DATA cpd(pThis->GetSafeHwnd(), *pDC);

	if (::IsWindow(cpd.DialogHWnd()))
	{
		const UINT dwStyle = ::GetWindowLong(cpd.DialogHWnd(), GWL_STYLE);
		if (0 != (dwStyle & WS_VISIBLE))
		{
			EnumChildWindows(pThis->GetSafeHwnd(), Marm_ClipChildrenProc, (LPARAM)&cpd);
		}
	}
}
