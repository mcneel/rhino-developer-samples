
#include "stdafx.h"
#include "MarmaladeMaterialSection.h"
#include "MarmaladeMaterial.h"

static UINT uColorChangedMsg    = CRhRdkColorButton::ChangedMessageNumber();
static UINT uSubNodeChangedMsg  = CRhRdkSubNodeCtrl::ChangedMessageNumber();
static UINT uSubNodeChangingMsg = CRhRdkSubNodeCtrl::ChangingMessageNumber();

static const ON_wString sChildSlotName = "marmalade-texture-slot";

BEGIN_MESSAGE_MAP(CIOREdit, CEdit)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CIOREdit::SetSection(CMarmaladeMaterialSection& section)
{
	m_pSection = &section;
}

void CIOREdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	double dIOR = 0.0;
	CRhRdkIORContextMenu m;
	if (m.TrackMenu(pWnd, point, dIOR, &m_sName))
	{
		CString s;
		s.Format(_T("%.2f"), dIOR);
		SetWindowText(s);

		SetFocus();
		SetSel(0, -1);

		if (NULL != m_pSection)
		{
			m_pSection->OnIORChanged();
		}
	}
}

CMarmaladeMaterialSection::CMarmaladeMaterialSection()
	:
	CRhRdkMaterialUISection(IDD)
{
	m_editIOR.SetSection(*this);

	m_bChildSlotInUse = false;
}

CMarmaladeMaterialSection::~CMarmaladeMaterialSection()
{
}

UUID CMarmaladeMaterialSection::Uuid(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {8B3BA6FE-84B8-418a-A1E5-CD0A6CEC7AE7}
		0x8b3ba6fe, 0x84b8, 0x418a, { 0xa1, 0xe5, 0xcd, 0xa, 0x6c, 0xec, 0x7a, 0xe7 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

void CMarmaladeMaterialSection::DoDataExchange(CDataExchange* pDX)
{
	CRhRdkExpandingSection::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_MATERIAL_COLOR, m_btColor);
	DDX_Control(pDX, IDC_CHECK_TEXTURE_ON, m_checkTextureOn);
	DDX_Control(pDX, IDC_EDIT_TEXTURE_AMOUNT, m_editTextureAmount);
	DDX_Control(pDX, IDC_EDIT_TEXTURE_PLACEHOLDER, m_editTexturePlaceholder);
	DDX_Control(pDX, IDC_EDIT_TRANSPARENCY, m_editTransparency);
	DDX_Control(pDX, IDC_EDIT_IOR, m_editIOR);
	DDX_Control(pDX, IDC_STATIC_IOR_NAME, m_staticIORName);
}

BEGIN_MESSAGE_MAP(CMarmaladeMaterialSection, CRhRdkMaterialUISection)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_EN_KILLFOCUS(IDC_EDIT_TEXTURE_AMOUNT, OnKillfocusEditTextureAmount)
	ON_EN_KILLFOCUS(IDC_EDIT_TRANSPARENCY, OnKillfocusEditTransparency)
	ON_EN_KILLFOCUS(IDC_EDIT_IOR, OnKillfocusEditIOR)
	ON_BN_CLICKED(IDC_CHECK_TEXTURE_ON, OnClickCheckTextureOn)
	ON_REGISTERED_MESSAGE(uColorChangedMsg, OnColorChanged)
	ON_REGISTERED_MESSAGE(uSubNodeChangingMsg, OnSubNodeChanging)
	ON_REGISTERED_MESSAGE(uSubNodeChangedMsg,  OnSubNodeChanged)
END_MESSAGE_MAP()

void CMarmaladeMaterialSection::OnDropFiles(HDROP hDropInfo)
{
	RhinoMessageBox( L"OnDrop", L"Material section", MB_OK);
}

BOOL CMarmaladeMaterialSection::OnInitDialog()
{
	CRhRdkMaterialUISection::OnInitDialog();

	if (NULL == Material())
		return FALSE;

	const UUID uuid = Material()->InstanceId();
	m_SubNode.SetParentInstanceId(uuid);
	m_SubNode.SetChildSlotName(sChildSlotName);
	VERIFY(m_SubNode.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1234));

	DisplayData();

	return FALSE;
}

void CMarmaladeMaterialSection::OnIORChanged(void)
{
	m_staticIORName.SetWindowTextW(m_editIOR.ChosenName());
}

LRESULT CMarmaladeMaterialSection::OnColorChanged(WPARAM w, LPARAM l)
{
	if (NULL == Material())
		return 0;

	Material()->SetColor(m_btColor.GetColor().OnColor(), CRhRdkContent::ccUI);

	return 1;
}

LRESULT CMarmaladeMaterialSection::OnSubNodeChanging(WPARAM, LPARAM ctrlID)
{
	if (NULL == Material())
		return 0;

	m_bChildSlotInUse = (NULL != Material()->FindChild(sChildSlotName));

	return 1;
}

LRESULT CMarmaladeMaterialSection::OnSubNodeChanged(WPARAM, LPARAM ctrlID)
{
	if (NULL == Material())
		return 0;

	CRhRdkContent* pChild = Material()->FindChild(sChildSlotName);
	if (NULL == pChild)
		return 0;

	if (m_bChildSlotInUse)
		return 0;

	Material()->SetTextureOn(true, CRhRdkContent::ccProgram);

	DisplayData();

	return 1;
}

ON_wString CMarmaladeMaterialSection::Caption(void) const
{
	return L"Marmalade Material";
}

void CMarmaladeMaterialSection::DisplayData(void)
{
	if (NULL == Material())
		return;

	m_btColor.SetColor(Material()->Color(), false);

	m_SubNode.DisplayData();

	CString s;
	s.Format(_T("%.1f"), Material()->Transparency());
	m_editTransparency.SetWindowText(s);

	s.Format(_T("%.2f"), Material()->IOR());
	m_editIOR.SetWindowText(s);

	s.Format(_T("%.1f"), Material()->TextureAmount());
	m_editTextureAmount.SetWindowText(s);

	m_checkTextureOn.SetCheck(Material()->TextureOn() ? BST_CHECKED : BST_UNCHECKED);
}

void CMarmaladeMaterialSection::OnClickCheckTextureOn()
{
	if (NULL != Material())
	{
		const bool bChecked = (BST_CHECKED == m_checkTextureOn.GetCheck());
		Material()->SetTextureOn(bChecked, CRhRdkContent::ccUI);
	}
}

void CMarmaladeMaterialSection::OnKillfocusEditTransparency()
{
	if (NULL != Material())
	{
		CString s;
		m_editTransparency.GetWindowText(s);
		Material()->SetTransparency(_tstof(s), CRhRdkContent::ccUI);
	}
}

void CMarmaladeMaterialSection::OnKillfocusEditIOR()
{
	if (NULL != Material())
	{
		CString s;
		m_editIOR.GetWindowText(s);
		Material()->SetIOR(_tstof(s), CRhRdkContent::ccUI);

		OnIORChanged();
	}
}

void CMarmaladeMaterialSection::OnKillfocusEditTextureAmount()
{
	if (NULL != Material())
	{
		CString s;
		m_editTextureAmount.GetWindowText(s);
		Material()->SetTextureAmount(_tstof(s), CRhRdkContent::ccUI);
	}
}

void CMarmaladeMaterialSection::OnSize(UINT nType, int cx, int cy)
{
	CRhRdkMaterialUISection::OnSize(nType, cx, cy);

	if (!IsWindow(m_editTexturePlaceholder.GetSafeHwnd()))
		return;

	CRect rect;
	m_editTexturePlaceholder.GetWindowRect(rect);
	ScreenToClient(rect);
	m_SubNode.MoveWindow(rect, true);
}

CMarmaladeMaterial* CMarmaladeMaterialSection::Material(void) const
{
	return static_cast<CMarmaladeMaterial*>(CRhRdkMaterialUISection::Material());
}
