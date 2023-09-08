
#include "stdafx.h"
#include "SampleRdkEffectsPanelDialog.h"
#include "Resource.h"

static inline const CRect R2R(const ON_4iRect& r) { return CRect(r.left, r.top, r.right, r.bottom); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All tabs must have unique ids.
//
// DO NOT REUSE THIS UUID!
// DO NOT REUSE THIS UUID!
// DO NOT REUSE THIS UUID!
// DO NOT REUSE THIS UUID!
static const UUID uuidTab = { 0xb72d4832, 0xf10d, 0x4700, { 0xb1, 0x92, 0xae, 0x9b, 0xe7, 0x89, 0x5d, 0x5f } };
// DO NOT REUSE THIS UUID!
// DO NOT REUSE THIS UUID!
// DO NOT REUSE THIS UUID!
// DO NOT REUSE THIS UUID!
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const auto uColorButtonChangedMsg = CRhRdkColorButton::ChangedMessageNumber();

BEGIN_MESSAGE_MAP(CSampleRdkEffectsPanelDialog, CRhRdkResizingDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnEdit)
	ON_REGISTERED_MESSAGE(uColorButtonChangedMsg, OnColorChanged)
END_MESSAGE_MAP()

// CSampleRdkEffectsPanelDialog

CSampleRdkEffectsPanelDialog::CSampleRdkEffectsPanelDialog(IRhRdkRenderWindow& rw)
	:
	m_RW(rw),
	CRhRdkResizingDialog(IDD)
{
}

bool CSampleRdkEffectsPanelDialog::CreateWnd(CWnd* pParent)
{
	if (!__super::Create(IDD, pParent))
		return false;

	return true;
}

void CSampleRdkEffectsPanelDialog::DoDataExchange(CDataExchange* pDX)
{
	CRhinoDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_CHECK1, m_check);
	DDX_Control(pDX, IDC_BUTTON1, m_button);
}

BOOL CSampleRdkEffectsPanelDialog::OnInitDialog()
{
	__super::OnInitDialog();

	m_edit.SetBounds(0.0, 1.0);
	m_edit.SetDecimalPlaces(3);
	m_edit.SetFixedSpinnerDelta(0.1);
	m_edit.SetLinearRollingDelta(0.01);
	m_edit.SetValue(0.5);

	EnableDisableControls();

	return TRUE;
}

void CSampleRdkEffectsPanelDialog::EnableDisableControls(void)
{
	const bool b = (m_check.GetCheck() == BST_CHECKED);
	m_edit.EnableWindow(b);
	m_button.EnableWindow(b);

	m_button.Invalidate();
}

void CSampleRdkEffectsPanelDialog::OnEdit()
{
	UpdateRendering();
}

void CSampleRdkEffectsPanelDialog::OnCheck()
{
	UpdateRendering();
	EnableDisableControls();
}

LRESULT CSampleRdkEffectsPanelDialog::OnColorChanged(WPARAM, LPARAM)
{
	UpdateRendering();

	return 1;
}

void CSampleRdkEffectsPanelDialog::UpdateRendering(void)
{
	// This is a very simple example of modifying the rendered image.

	const UUID uuidChannel = m_RW.ShowingChannel();
	const bool bRGB  = (uuidChannel == IRhRdkRenderWindow::chanRGB);
	const bool bRGBA = (uuidChannel == IRhRdkRenderWindow::chanRGBA);
	if (!bRGB && !bRGBA)
		return; // Only modify RGB or RGBA images.

	// TODO: ANDY -- I'm stuck on this. CRhRdkImageAdjust is now just a forward reference.
	// To get the settings that Adjust() used to provide, we need a document (to get render settings).
	// We don't have a document here.

	const auto gamma = 1.0f / m_RW.Adjust().Gamma();

	auto* pDib = m_RW.LockDib();
	if (nullptr != pDib)
	{
		auto* pChan = m_RW.OpenChannel(IRhRdkRenderWindow::chanRGBA);
		if (nullptr != pChan)
		{
			const int width  = pDib->Width();
			const int height = pDib->Height();

			const bool bOn = (m_check.GetCheck() == BST_CHECKED);
			const float amount = float(bOn ? m_edit.GetValue() : 0.0);

			const auto col = m_button.GetColor();
			const float cr = col.FRed();
			const float cg = col.FGreen();
			const float cb = col.FBlue();

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					const auto* p = static_cast<const float*>(pChan->GetValue(x, y));
					if (nullptr != p)
					{
						float r = p[0], g = p[1], b = p[2], a = bRGBA ? p[3] : 255;

						r = powf(r, gamma);
						g = powf(g, gamma);
						b = powf(b, gamma);

						r += amount * (cr - r);
						g += amount * (cg - g);
						b += amount * (cb - b);

#ifdef ON_RUNTIME_WIN
						pDib->SetPixel(x, height-y-1, r, g, b, a); // Windows dib is upside-down.
#else
						pDib->SetPixel(x, y, r, g, b, a);
#endif
					}
				}
			}

			pChan->Close();
		}

		m_RW.UnlockDib();
		m_RW.Invalidate();
	}
}

// CSampleRdkEffectsPanelTab

void CSampleRdkEffectsPanelTab::MoveWnd(const ON_4iRect& rect)
{
	if (::IsWindow(m_pDialog->GetSafeHwnd()))
	{
		m_pDialog->MoveWindow(R2R(rect), true);
	}
}

void CSampleRdkEffectsPanelTab::ShowWnd(UINT uState)
{
	if (::IsWindow(m_pDialog->GetSafeHwnd()))
	{
		m_pDialog->ShowWindow(uState);
	}
}

void CSampleRdkEffectsPanelTab::DestroyWnd(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (::IsWindow(m_pDialog->GetSafeHwnd()))
	{
		m_pDialog->DestroyWindow();
	}

	delete m_pDialog;
	m_pDialog = nullptr;
}

bool CSampleRdkEffectsPanelTab::IsCreated(void) const
{
	if (::IsWindow(m_pDialog->GetSafeHwnd()))
		return true;

	return false;
}

void CSampleRdkEffectsPanelTab::SwitchDockBar()
{
	const auto hWndParent = PanelHost();
	if (!::IsWindow(hWndParent))
		return;

	const auto hWndDlg = m_pDialog->GetSafeHwnd();
	if (!::IsWindow(hWndDlg))
		return;

	if (::GetParent(hWndDlg) != hWndParent)
	{
		::SetParent(hWndDlg, hWndParent);
	}
}

UUID CSampleRdkEffectsPanelTab::TabId(void) const
{
	return uuidTab;
}

HICON CSampleRdkEffectsPanelTab::Icon(const ON_2iSize& sizeInPixels) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON));
}

HWND CSampleRdkEffectsPanelTab::PanelHost() const
{
	// TODO: ANDY -- I'm stuck on this. This is all obsolete. This was marked as "Internal expert methods
	// for the RDK UI" so I don't understand why we made a sample explaining it to the public.
	//
	// In fact, the way this panel stuff works now is all Eto and it's all completely different.

	const auto* pUI = m_RW.UI();
	if (nullptr == pUI)
		return NULL;

	return pUI->PostEffectPanelHost();
}

bool CSampleRdkEffectsPanelTab::CreateWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto* pTab = CWnd::FromHandle(PanelHost());
	m_pDialog = new CSampleRdkEffectsPanelDialog(m_RW);
	m_pDialog->CreateWnd(pTab);

	return true;
}

// CSampleRdkEffectsPanelTabFactory

CRhinoUiDockBarTab* CSampleRdkEffectsPanelTabFactory::NewTab(IRhRdkRenderWindow& rw) const
{
	return new CSampleRdkEffectsPanelTab(rw);
}

UUID CSampleRdkEffectsPanelTabFactory::TabId(void) const
{
	return uuidTab;
}
