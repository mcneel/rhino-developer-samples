#include "stdafx.h"
#include "SampleDrawAnnotationDialog.h"

// CSampleDrawAnnotationConduit conduit

CSampleDrawAnnotationConduit::CSampleDrawAnnotationConduit() 
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY, false) 
{
  m_dim_style = ON_DimStyle::Default;
}

bool CSampleDrawAnnotationConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  if (m_text.IsValid())
  {
    if (nChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
    {
      ON_BoundingBox bbox = m_text.BoundingBox();
      m_pChannelAttrs->m_BoundingBox.Union(bbox);
    }
    else if (nChannel == CSupportChannels::SC_DRAWOVERLAY)
    {
      dp.DrawAnnotation(m_text, 1.0, m_dim_style, ON_UNSET_COLOR, nullptr, nullptr);
    }
  }
  return true;
}

void CSampleDrawAnnotationConduit::SetDimStyle(const ON_DimStyle& dim_style)
{
  m_dim_style = dim_style;
}


void CSampleDrawAnnotationConduit::SetTextFont(const ON_Font& font)
{
  m_dim_style.SetFont(font);
  const ON_wString facename(m_dim_style.Font().FontFaceName());
  m_text.Create(facename, &m_dim_style, ON_Plane::World_xy);
}

// CSampleDrawAnnotationDialog dialog

IMPLEMENT_DYNAMIC(CSampleDrawAnnotationDialog, CRhinoDialog)

CSampleDrawAnnotationDialog::CSampleDrawAnnotationDialog(CWnd* pParent, CRhinoDoc& doc)
	: CRhinoDialog(IDD_FONT_DIALOG, pParent)
  , m_doc(doc)
{
  SetEnableDisplayCommands(true, m_doc.RuntimeSerialNumber());
  EnableSaveDialogPositionAndSize(true);
}

void CSampleDrawAnnotationDialog::DoDataExchange(CDataExchange* pDX)
{
	CRhinoDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO1, m_font_combo_box);
}

BEGIN_MESSAGE_MAP(CSampleDrawAnnotationDialog, CRhinoDialog)
  ON_BN_CLICKED(IDOK, &CSampleDrawAnnotationDialog::OnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &CSampleDrawAnnotationDialog::OnClickedCancel)
  ON_CBN_SELCHANGE(IDC_COMBO1, &CSampleDrawAnnotationDialog::OnSelChangeComboFont)
END_MESSAGE_MAP()

BOOL CSampleDrawAnnotationDialog::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  m_conduit.Enable(m_doc.RuntimeSerialNumber());

  m_font_combo_box.SetIgnoreZeroTypeFonts(true);
  m_font_combo_box.FillFonts();
  m_font_combo_box.SetCurSel(0);
  OnSelChangeComboFont();

  return TRUE;
}

void CSampleDrawAnnotationDialog::OnClickedOk()
{
  m_conduit.Disable();

  CRhinoDialog::OnOK();
}

void CSampleDrawAnnotationDialog::OnClickedCancel()
{
  m_conduit.Disable();

  CRhinoDialog::OnCancel();
}

void CSampleDrawAnnotationDialog::OnSelChangeComboFont()
{
  LOGFONT lf;
  memset(&lf, 0, sizeof(lf));
  int index = m_font_combo_box.GetSelFont(lf);
  if (index == CB_ERR)
    return;

  ON_Font font;
  if (!font.SetFontCharacteristics(lf.lfFaceName, false, false, false, false))
    return;

  m_conduit.SetTextFont(font);
  m_doc.Redraw();
}
