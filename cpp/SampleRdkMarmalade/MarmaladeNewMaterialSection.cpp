
#include "stdafx.h"
#include "MarmaladeNewMaterialSection.h"
#include "MarmaladeNewMaterial.h"
#include "MarmaladeUtilities.h"

static UINT uColorChangedMsg = CRhRdkColorButton::ChangedMessageNumber();

BEGIN_MESSAGE_MAP(CMarmaladeNewMaterialSection::CIOREdit, CRhRdkSdkRealNumberEdit)
	ON_MESSAGE(WM_CONTEXTMENU, OnContextMenu)
END_MESSAGE_MAP()

CMarmaladeNewMaterialSection::CIOREdit::CIOREdit(CMarmaladeNewMaterialSection& s)
	:
	m_Section(s)
{
	SetDecimalPlaces(2);
}

LRESULT CMarmaladeNewMaterialSection::CIOREdit::OnContextMenu(WPARAM w, LPARAM l)
{
	auto* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(w));
	if (nullptr != pWnd)
	{
		double dIOR = 1.0;
		CRhRdkIORContextMenu m;
		const CPoint pt(l);
		if (m.TrackMenu(pWnd->GetSafeHwnd(), ON_2iPoint(pt.x, pt.y), dIOR))
		{
			if (GetValue() != dIOR)
			{
				SetValue(dIOR);
				m_Section.OnChangeAnything();
			}

			SetFocus();
			SetSel(0, -1);
		}
	}

	return 1;
}

CMarmaladeNewMaterialSection::CMarmaladeNewMaterialSection()
	:
	m_editIOR(*this),
	m_SubNode(*this),
	CRhRdkMaterialUISection_MFC(IDD)
{
	m_editTransparency.SetDecimalPlaces(1);
}

UUID CMarmaladeNewMaterialSection::Uuid(void) const
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

void CMarmaladeNewMaterialSection::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BT_MATERIAL_COLOR, m_btColor);
	DDX_Control(pDX, IDC_EDIT_TEXTURE_PLACEHOLDER, m_editTexturePlaceholder);
	DDX_Control(pDX, IDC_EDIT_TRANSPARENCY, m_editTransparency);
	DDX_Control(pDX, IDC_EDIT_IOR, m_editIOR);
}

BEGIN_MESSAGE_MAP(CMarmaladeNewMaterialSection, CRhRdkMaterialUISection_MFC)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_EN_KILLFOCUS(IDC_EDIT_TRANSPARENCY, OnKillfocusEditTransparency)
	ON_REGISTERED_MESSAGE(uColorChangedMsg, OnColorChanged)
END_MESSAGE_MAP()

void CMarmaladeNewMaterialSection::OnDropFiles(HDROP hDropInfo)
{
	RhinoMessageBox( L"OnDrop", L"Material section", MB_OK);
}

BOOL CMarmaladeNewMaterialSection::OnInitDialog()
{
	__super::OnInitDialog();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const auto con = Controller();

	// The color button in V6 onwards supports texture slots. The button
	// can display a texture icon and has texturing items on the menu.
	// To use this feature, set the child slot name to the button before
	// setting the controller.
	m_btColor.SetChildSlotName(MARM_SHADER_COLOR_CSN);
	m_btColor.SetController(con); // Must be after setting the child slot name.

	m_SubNode.SetChildSlotName(MARM_SHADER_COLOR_CSN);
	VERIFY(m_SubNode.CreateWnd(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1234));
	m_SubNode.SetController(con); // Must be after setting the child slot name.

	// This new-style dialog uses the sub-node control's built-in 'on' check box and 'amount' edit box. 
	m_SubNode.ShowCheck(true);
	m_SubNode.ShowAmount(true);

	DisplayData();

	return FALSE;
}

ON_wString CMarmaladeNewMaterialSection::Caption(bool) const
{
	return L"Marmalade Material";
}

bool CMarmaladeNewMaterialSection::SupportsVaries(const CRhRdkContentArray& aContent) const
{
	return RhRdkSupportsVariesHelperEx<CMarmaladeNewMaterial>(aContent);
}

void CMarmaladeNewMaterialSection::DisplayData(void)
{
	__super::DisplayData();

	if (m_iInternalCall > 0)
		return;

	const auto con = Controller();
	if (!con)
		return;

	CRhRdkVariant vCol, vIOR, vTrans, vOn, vAmount;

	CRhRdkEditableContentArray aContent(*con, false);
	if (0 == aContent.Count())
		return;

	m_btColor.SetUndoString(::RhRdkSmartUndoString(*this, MARM_SHADER_COLOR));

	int count = 0;

	for (int i = 0; i < aContent.Count(); i++)
	{
		const auto pMaterial = dynamic_cast<const CMarmaladeNewMaterial*>(aContent[i]);
		if (nullptr == pMaterial)
			continue;

		const auto vColThis    = pMaterial->GetParameter(MARM_SHADER_COLOR);
		const auto vTransThis  = pMaterial->GetParameter(MARM_SHADER_TRANSPARENCY);
		const auto vIORThis    = pMaterial->GetParameter(MARM_SHADER_IOR);
		const auto vOnThis     = pMaterial->GetParameter(MARM_SHADER_TEXTURE_ON);
		const auto vAmountThis = pMaterial->GetParameter(MARM_SHADER_TEXTURE_AMOUNT);

		if (0 == count++)
		{
			vCol    = vColThis;
			vTrans  = vTransThis;
			vIOR    = vIORThis;
			vOn     = vOnThis;
			vAmount = vAmountThis;
		}
		else
		{
			if (vCol != vColThis)
				vCol.SetVaries();

			if (vTrans != vTransThis)
				vTrans.SetVaries();

			if (vIOR != vIORThis)
				vIOR.SetVaries();

			if (vOn != vOnThis)
				vOn.SetVaries();

			if (vAmount != vAmountThis)
				vAmount.SetVaries();
		}
	}

	if (!vCol.Varies())
	{
		m_btColor.SetColor(vCol, false);
	}

	m_btColor.SetVaries(vCol.Varies());

	if (vTrans.Varies())
	{
		m_editTransparency.SetBlank();
	}
	else
	{
		m_editTransparency.SetValue(vTrans, true);
	}

	if (vIOR.Varies())
	{
		m_editIOR.SetBlank();
	}
	else
	{
		m_editIOR.SetValue(vIOR, true);
	}

	m_SubNode.SetCheck(vOn.Varies() ? BST_INDETERMINATE : (vOn.AsBool() ? BST_CHECKED : BST_UNCHECKED));

	if (!vAmount.Varies())
		m_SubNode.SetAmount(vAmount.AsDouble() * 0.01);

	m_SubNode.SetAmountVaries(vAmount.Varies());

	m_SubNode.DisplayData();
}

void CMarmaladeNewMaterialSection::OnChangeAnything(void)
{
	const auto con = Controller();
	if (!con)
		return;

	m_iInternalCall++;

	CRhRdkNewUndoableEvent e(*con, ::RhRdkSmartUndoString(*this, MARM_SHADER_COLOR));

	CRhRdkEditableContentArray aContent(*con, true);
	for (int i = 0; i < aContent.Count(); i++)
	{
		const CRhRdkContent::Change<CRhRdkContent> m(*aContent[i], RhRdkChangeContext::UI);

		if (!m_btColor.Varies())
		{
			const auto col = m_btColor.GetColor();
			m().SetParameter(MARM_SHADER_COLOR, col);
		}

		if (!m_editIOR.IsBlank())
			m().SetParameter(MARM_SHADER_IOR, m_editIOR.GetValue());

		if (!m_editTransparency.IsBlank())
			m().SetParameter(MARM_SHADER_TRANSPARENCY, m_editTransparency.GetValue());

		const auto uCheck = m_SubNode.GetCheck();
		if (BST_INDETERMINATE != uCheck)
			m().SetParameter(MARM_SHADER_TEXTURE_ON, BST_CHECKED == uCheck);

		if (!m_SubNode.AmountVaries())
			m().SetParameter(MARM_SHADER_TEXTURE_AMOUNT, int(m_SubNode.GetAmount() * 100.0));
	}

	m_iInternalCall--;
}

LRESULT CMarmaladeNewMaterialSection::OnColorChanged(WPARAM w, LPARAM l)
{
	OnChangeAnything();
	return 1;
}

void CMarmaladeNewMaterialSection::OnClickCheckTextureOn()
{
	OnChangeAnything();
}

void CMarmaladeNewMaterialSection::OnKillfocusEditTransparency()
{
	OnChangeAnything();
}

void CMarmaladeNewMaterialSection::OnKillfocusEditTextureAmount()
{
	OnChangeAnything();
}

void CMarmaladeNewMaterialSection::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (!::IsWindow(m_editTexturePlaceholder.GetSafeHwnd()))
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rect;
	m_editTexturePlaceholder.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rectClient.right;
	m_SubNode.MoveWindow(rect, true);
}

void CMarmaladeNewMaterialSection::OnEvent(IRhinoUiController& con, const UUID& uuidData, IRhinoUiController::EventPriority ep, const IRhinoUiEventInfo* pInfo)
{
	__super::OnEvent(con, uuidData, ep, pInfo);

	DisplayData();
	EnableDisableControls();
}
