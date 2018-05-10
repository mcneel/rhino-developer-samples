
#include "stdafx.h"
#include "MarmaladeMaterialSection.h"
#include "MarmaladeMaterial.h"
#include "MarmaladeUtilities.h"

static UINT uColorChangedMsg = CRhRdkColorButton::ChangedMessageNumber();

BEGIN_MESSAGE_MAP(CIOREdit, CEdit)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CIOREdit::OnContextMenu(CWnd* pWnd, CPoint pt)
{
	double dIOR = 0.0;
	CRhRdkIORContextMenu m;
	if (m.TrackMenu(pWnd->GetSafeHwnd(), ON_2iPoint(pt.x, pt.y), dIOR))
	{
		CString s;
		s.Format(_T("%.2f"), dIOR);
		SetWindowText(s);

		SetFocus();
		SetSel(0, -1);

		m_Section.OnChangeAnything();
	}
}

CMarmaladeMaterialSection::CMarmaladeMaterialSection()
	:
	m_editIOR(*this),
	CRhRdkMaterialUISection_MFC(IDD)
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
	CRhRdkMaterialUISection_MFC::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_MATERIAL_COLOR, m_btColor);
	DDX_Control(pDX, IDC_CHECK_TEXTURE_ON, m_checkTextureOn);
	DDX_Control(pDX, IDC_EDIT_TEXTURE_AMOUNT, m_editTextureAmount);
	DDX_Control(pDX, IDC_EDIT_TEXTURE_PLACEHOLDER, m_editTexturePlaceholder);
	DDX_Control(pDX, IDC_EDIT_TRANSPARENCY, m_editTransparency);
	DDX_Control(pDX, IDC_EDIT_IOR, m_editIOR);
}

BEGIN_MESSAGE_MAP(CMarmaladeMaterialSection, CRhRdkMaterialUISection_MFC)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_EN_KILLFOCUS(IDC_EDIT_TEXTURE_AMOUNT, OnKillfocusEditTextureAmount)
	ON_EN_KILLFOCUS(IDC_EDIT_TRANSPARENCY, OnKillfocusEditTransparency)
	ON_EN_KILLFOCUS(IDC_EDIT_IOR, OnKillfocusEditIOR)
	ON_BN_CLICKED(IDC_CHECK_TEXTURE_ON, OnClickCheckTextureOn)
	ON_REGISTERED_MESSAGE(uColorChangedMsg, OnColorChanged)
END_MESSAGE_MAP()

void CMarmaladeMaterialSection::OnDropFiles(HDROP hDropInfo)
{
	RhinoMessageBox( L"OnDrop", L"Material section", MB_OK);
}

BOOL CMarmaladeMaterialSection::OnInitDialog()
{
	CRhRdkMaterialUISection_MFC::OnInitDialog();

	const auto con = Controller();

	// The color button in V6 onwards supports texture slots. The button
	// can display a texture icon and has texturing items on the menu.
	// To use this feature, set the child slot name to the button before
	// setting the controller.
	m_btColor.SetChildSlotName(wszColorChildSlot);
	m_btColor.SetController(con); // Must be after setting the child slot name.

	// This old-style dialog only uses the child slot functionality of the sub-node control.
	// It implements the 'on' and 'amount' functionality by using a discrete check box and edit box.
	// The new dialog (CMarmaladeNewMaterialSection) uses the sub-node control's built-in
	// 'on' check box and 'amount' edit box. 
	m_SubNode.SetChildSlotName(wszColorChildSlot);
	VERIFY(m_SubNode.CreateWnd(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1234));
	m_SubNode.SetController(con); // Must be after setting the child slot name.

	DisplayData();

	return FALSE;
}

ON_wString CMarmaladeMaterialSection::Caption(bool) const
{
	return L"Marmalade Material";
}

bool CMarmaladeMaterialSection::SupportsVaries(const CRhRdkContentArray& aContent) const
{
	return RhRdkSupportsVariesHelperEx<CMarmaladeMaterial>(aContent);
}

void CMarmaladeMaterialSection::DisplayData(void)
{
	__super::DisplayData();

	if (m_iInternalCall > 0)
		return;

	const auto con = Controller();
	if (!con)
		return;

	CRhRdkColor col;
	bool bTextureOn = false;
	double dIOR = 0.0, dTrans = 0.0, dTextureAmount = 0.0;
	bool bColVaries = false, bIORVaries = false, bTransVaries = false, bTextureOnVaries = false, bTextureAmountVaries = false;

	int count = 0;
	CRhRdkEditableContentArray aContent(*con, false);
	for (int i = 0; i < aContent.Count(); i++)
	{
		const auto pMaterial = dynamic_cast<const CMarmaladeMaterial*>(aContent[i]);
		if (nullptr == pMaterial)
			continue;

		if (0 == count++)
		{
			col = pMaterial->Color();
			dIOR = pMaterial->IOR();
			dTrans = pMaterial->Transparency();
			dTextureAmount = pMaterial->TextureAmount();
			bTextureOn = pMaterial->TextureOn();
		}
		else
		{
			if (col != pMaterial->Color())
				bColVaries = true;

			if (dIOR != pMaterial->IOR())
				bIORVaries = true;

			if (dTrans != pMaterial->Transparency())
				bTransVaries = true;

			if (bTextureOn != pMaterial->TextureOn())
				bTextureOnVaries = true;

			if (dTextureAmount != pMaterial->TextureAmount())
				bTextureAmountVaries = true;
		}
	}

	m_btColor.SetColor(col, false);
	m_btColor.SetVaries(bColVaries);

	ON_wString s1;
	if (!bTransVaries)
		s1.Format(_T("%.1f"), dTrans);
	m_editTransparency.SetWindowText(s1);

	ON_wString s2;
	if (!bIORVaries)
		s2.Format(_T("%.2f"), dIOR);
	m_editIOR.SetWindowText(s2);

	ON_wString s3;
	if (!bTextureAmountVaries)
		s3.Format(_T("%g%%"), dTextureAmount * 100.0);
	m_editTextureAmount.SetWindowText(s3);

	m_checkTextureOn.SetCheck(bTextureOnVaries ? BST_INDETERMINATE : (bTextureOn ? BST_CHECKED : BST_UNCHECKED));

	m_SubNode.DisplayData();
}

void CMarmaladeMaterialSection::OnChangeAnything(void)
{
	const auto con = Controller();
	if (!con)
		return;

	CString s;

	m_iInternalCall++;

	CRhRdkEditableContentArray aContent(*con, true);
	for (int i = 0; i < aContent.Count(); i++)
	{
		const auto pMaterial = dynamic_cast<const CMarmaladeMaterial*>(aContent[i]);
		if (nullptr == pMaterial)
			continue;

		const CRhRdkContent::Change<CMarmaladeMaterial> m(*pMaterial, RhRdkChangeContext::UI);

		if (!m_btColor.Varies())
			m().SetColor(m_btColor.GetColor());

		const auto uCheck = m_checkTextureOn.GetCheck();
		if (BST_INDETERMINATE != uCheck)
			m().SetTextureOn(BST_CHECKED == uCheck);

		m_editTextureAmount.GetWindowText(s);
		if (!s.IsEmpty())
			m().SetTextureAmount(_tstof(s) * 0.01);

		m_editIOR.GetWindowText(s);
		if (!s.IsEmpty())
			m().SetIOR(_tstof(s));

		m_editTransparency.GetWindowText(s);
		if (!s.IsEmpty())
			m().SetTransparency(_tstof(s));
	}

	m_iInternalCall--;
}

LRESULT CMarmaladeMaterialSection::OnColorChanged(WPARAM w, LPARAM l)
{
	OnChangeAnything();
	return 1;
}

void CMarmaladeMaterialSection::OnClickCheckTextureOn()
{
	OnChangeAnything();
}

void CMarmaladeMaterialSection::OnKillfocusEditTransparency()
{
	OnChangeAnything();
}

void CMarmaladeMaterialSection::OnKillfocusEditIOR()
{
	OnChangeAnything();
}

void CMarmaladeMaterialSection::OnKillfocusEditTextureAmount()
{
	OnChangeAnything();
}

void CMarmaladeMaterialSection::OnSize(UINT nType, int cx, int cy)
{
	CRhRdkMaterialUISection_MFC::OnSize(nType, cx, cy);

	if (!IsWindow(m_editTexturePlaceholder.GetSafeHwnd()))
		return;

	CRect rect;
	m_editTexturePlaceholder.GetWindowRect(rect);
	ScreenToClient(rect);
	m_SubNode.MoveWindow(rect, true);
}

void CMarmaladeMaterialSection::OnEvent(IRhinoUiController& con, const UUID& uuidData, IRhinoUiController::EventPriority ep, const IRhinoUiEventInfo* pInfo)
{
	CRhRdkMaterialUISection_MFC::OnEvent(con, uuidData, ep, pInfo);

	DisplayData();
	EnableDisableControls();
}
