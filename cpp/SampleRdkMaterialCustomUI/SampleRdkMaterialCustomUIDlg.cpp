
#include "stdafx.h"
#include "SampleRdkMaterialCustomUIDlg.h"
#include "SampleRdkMaterialCustomUIMaterial.h"

inline const CRect R2R(const ON_4iRect& r) { return CRect(r.left, r.top, r.right, r.bottom); }

static UINT g_uColorButtonMessage = CRhRdkColorButton::ChangedMessageNumber();

BEGIN_MESSAGE_MAP(CSampleRdkMaterialCustomUIDlg, CRhinoDialog)
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, OnKillfocusEditName)
	ON_EN_KILLFOCUS(IDC_EDIT_NOTES, OnKillfocusEditNotes)
	ON_REGISTERED_MESSAGE(g_uColorButtonMessage, OnColorChanged)
END_MESSAGE_MAP()

CSampleRdkMaterialCustomUIDlg::CSampleRdkMaterialCustomUIDlg(const UUID& uuidEditor, const UUID& uuidUI, CWnd* pParent)
	:
	m_iInternalCall(0),
	CRhRdkCustomContentUI(uuidEditor, uuidUI),
	CRhinoDialog(IDD, pParent)
{
}

CSampleRdkMaterialCustomUIDlg::~CSampleRdkMaterialCustomUIDlg()
{
	for (int i = 0; i < m_aDS.Count(); i++)
	{
		m_aDS[i]->DeleteThis();
	}
}

void CSampleRdkMaterialCustomUIDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COLOR_BUTTON, m_color_button);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edit_name);
	DDX_Control(pDX, IDC_EDIT_NOTES, m_edit_notes);
}

void CSampleRdkMaterialCustomUIDlg::CreateDlg(CWnd* pParent)
{
	Create(IDD, pParent);

	// Because it will host child controls, set the dialog as
	// a control parent. This is critical to avoid the risk of
	// an infinite loop caused by a bug in Windows dialog code.
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	// Make sure the automatic UI is placed correctly.
	SetControlPositionAndSize();
}

void CSampleRdkMaterialCustomUIDlg::Destroy(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}
}

BOOL CSampleRdkMaterialCustomUIDlg::OnInitDialog()
{
	__super::OnInitDialog();

	return FALSE; // Prevent focus being set to a control; this would interfere with the RDK UI.
}

void CSampleRdkMaterialCustomUIDlg::SetPositionAndSize(const ON_4iRect& rect)
{
	if (::IsWindow(GetSafeHwnd()))
	{
		MoveWindow(R2R(rect));
	}
}

void CSampleRdkMaterialCustomUIDlg::DisplayData(void)
{
	if (m_iInternalCall > 0)
		return;

	const auto con = Controller();
	if (!con)
		return;

	// This demonstrates how to support 'varies' for multiple content selection.
	// This only displays the non-automatic fields.

	CRhRdkEditableContentArray aContent(*con, false);

	int count = 0;
	for (int i = 0; i < aContent.Count(); i++)
	{
		const auto& c = *aContent[i];

		const CRhRdkColor col = c.GetParameter(L"color").AsRdkColor();

		CString sName = c.InstanceName();
		sName.Trim();

		CString sNotes = c.Notes();
		sNotes.Trim();

		if (0 == count++)
		{
			// First content; set the values to the UI.
			m_color_button.SetColor(col, false);
			m_edit_name.SetWindowText(sName);
			m_edit_notes.SetWindowText(sNotes);
		}
		else
		{
			// Subsequent contents; check the values and if different, set the UI to 'varies'.
			if (m_color_button.GetColor() != col)
				m_color_button.SetVaries(true);

			CString s;
			m_edit_name.GetWindowText(s);
			if (sName != s)
				m_edit_name.SetWindowText(L"");

			m_edit_notes.GetWindowText(s);
			if (sNotes != s)
				m_edit_notes.SetWindowText(L"");
		}
	}
}

LRESULT CSampleRdkMaterialCustomUIDlg::OnColorChanged(WPARAM w, LPARAM l)
{
	ASSERT(Controller());
	if (!Controller())
		return 0;

	const auto col = m_color_button.GetColor();

	CRhRdkNewUndoableEvent e(*Controller(), L"SampleRdkMaterialCustomUI material color");
	CRhRdkEditableContentArray aContent(*Controller(), true);

	for (int i = 0; i < aContent.Count(); i++)
	{
		const CRhRdkContent::Change<CRhRdkContent> c(*aContent[i], RhRdkChangeContext::UI);

		c().SetParameter(L"color", col);
	}

	return 1;
}

void CSampleRdkMaterialCustomUIDlg::OnKillfocusEditName()
{
	ASSERT(Controller());
	if (!Controller())
		return;

	CString sName;
	m_edit_name.GetWindowText(sName);
	sName.Trim();

	m_iInternalCall++;

	CRhRdkNewUndoableEvent e(*Controller(), L"SampleRdkMaterialCustomUI material name");
	CRhRdkEditableContentArray aContent(*Controller(), true);

	for (int i = 0; i < aContent.Count(); i++)
	{
		const CRhRdkContent::Change<CRhRdkContent> c(*aContent[i], RhRdkChangeContext::UI);
		c().SetInstanceName(sName);
	}

	m_iInternalCall--;
}

void CSampleRdkMaterialCustomUIDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	SetControlPositionAndSize();
}

void CSampleRdkMaterialCustomUIDlg::SetControlPositionAndSize(void)
{
}

void CSampleRdkMaterialCustomUIDlg::OnKillfocusEditNotes()
{
	ASSERT(Controller());
	if (!Controller())
		return;

	CString sNotes;
	m_edit_notes.GetWindowTextW(sNotes);
	sNotes.Trim();

	m_iInternalCall++;

	CRhRdkNewUndoableEvent e(*Controller(), L"SampleRdkMaterialCustomUI material notes");
	CRhRdkEditableContentArray aContent(*Controller(), true);

	for (int i = 0; i < aContent.Count(); i++)
	{
		const CRhRdkContent::Change<CRhRdkContent> c(*aContent[i], RhRdkChangeContext::UI);
		c().SetNotes(sNotes);
	}

	m_iInternalCall--;
}

void CSampleRdkMaterialCustomUIDlg::OnContentRenamed(const CRhRdkContent& c)
{
	if (m_iInternalCall > 0)
		return;

	//const CRhRdkDocument* pRdkDoc = RdkDocument();
	//if (nullptr == pRdkDoc)
	//	return;
	//
	//if (m_RdkDocRef != c.RdkDocumentRegistered())
	//	return; // Wrong document.

	CRhRdkContentArray aContent;
	GetSelection(aContent);

	for (int i = 0; i < aContent.Count(); i++)
	{
		if (&c == aContent[i])
		{
			DisplayData();
			break;
		}
	}
}

void CSampleRdkMaterialCustomUIDlg::OnContentNotesChanged(const CRhRdkContent& c)
{
	OnContentRenamed(c);
}

bool CSampleRdkMaterialCustomUIDlg::IsCreated(void) const
{
	return IsWindow(GetSafeHwnd()) ? true : false;
}

bool CSampleRdkMaterialCustomUIDlg::IsShown(void) const
{
	return IsWindowVisible() ? true : false;
}

void CSampleRdkMaterialCustomUIDlg::Show(bool bShow)
{
	if (::IsWindow(GetSafeHwnd()))
	{
		ShowWindow(bShow ? SW_SHOWNA : SW_HIDE);
	}
}

void CSampleRdkMaterialCustomUIDlg::Enable(bool bEnable)
{
	if (::IsWindow(GetSafeHwnd()))
	{
		EnableWindow(bEnable); // TODO: gray out the controls.
	}
}
