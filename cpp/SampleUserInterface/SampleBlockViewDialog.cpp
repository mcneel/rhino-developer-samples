#include "StdAfx.h"
#include "SampleBlockViewDialog.h"

IMPLEMENT_DYNAMIC(CSampleBlockViewDialog, CRhinoDialog)

CSampleBlockViewDialog::CSampleBlockViewDialog(CWnd* pParent, CRhinoDoc& doc)
  : CRhinoDialog(CSampleBlockViewDialog::IDD, pParent),
  m_doc(doc)
{
}

CSampleBlockViewDialog::~CSampleBlockViewDialog()
{
}

void CSampleBlockViewDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LISTBOX, m_listbox);
  DDX_Control(pDX, IDC_PREVIEW, m_preview);
}

BEGIN_MESSAGE_MAP(CSampleBlockViewDialog, CRhinoDialog)
  ON_LBN_SELCHANGE(IDC_LISTBOX, OnSelChangeListBox)
END_MESSAGE_MAP()

BOOL CSampleBlockViewDialog::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  // For fun, lets draw the world axes in the preview control....
  m_preview.SetDrawDecorations(CRhinoUiBlockPreview::draw_world_axes);

  const CRhinoInstanceDefinitionTable& idef_table = m_doc.m_instance_definition_table;
  int i;
  for (i = 0; i < idef_table.InstanceDefinitionCount(); i++)
  {
    const CRhinoInstanceDefinition* idef = idef_table[i];
    if (idef && !idef->IsDeleted())
      m_listbox.AddString(idef->Name());
  }

  m_listbox.SetCurSel(0);

  OnSelChangeListBox();

  return TRUE;
}

void CSampleBlockViewDialog::OnSelChangeListBox()
{
  int index = m_listbox.GetCurSel();
  if (index != LB_ERR)
  {
    CString str;
    m_listbox.GetText(index, str);
    const CRhinoInstanceDefinitionTable& idef_table = m_doc.m_instance_definition_table;
    int i;
    for (i = 0; i < idef_table.InstanceDefinitionCount(); i++)
    {
      const CRhinoInstanceDefinition* idef = idef_table[i];
      if (idef && str.CompareNoCase(idef->Name()) == 0)
      {
        m_preview.SetInstanceDefinition(idef);
        return;
      }
    }
  }
}
