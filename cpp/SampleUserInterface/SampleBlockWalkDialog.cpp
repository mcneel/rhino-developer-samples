#include "stdafx.h"
#include "SampleBlockWalkDialog.h"

IMPLEMENT_DYNAMIC(CSampleBlockWalkDialog, CRhinoDialog)

CSampleBlockWalkDialog::CSampleBlockWalkDialog(CWnd* pParent, CRhinoDoc& doc)
  : CRhinoDialog(CSampleBlockWalkDialog::IDD, pParent)
  , m_doc(doc)
  , m_idef(0)
{
}

CSampleBlockWalkDialog::~CSampleBlockWalkDialog()
{
}

void CSampleBlockWalkDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BLOCK_TREE, m_tree);
  DDX_Control(pDX, IDC_BLOCK_PREVIEW, m_preview);
  DDX_Control(pDX, IDC_COMBO_VIEW, m_view_combo);
  DDX_Control(pDX, IDC_COMBO_DISPLAY, m_display_combo);
}

BEGIN_MESSAGE_MAP(CSampleBlockWalkDialog, CRhinoDialog)
  ON_NOTIFY(TVN_SELCHANGED, IDC_BLOCK_TREE, &CSampleBlockWalkDialog::OnSelChangeBlockTree)
  ON_CBN_SELCHANGE(IDC_COMBO_VIEW, &CSampleBlockWalkDialog::OnSelChangeComboView)
  ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY, &CSampleBlockWalkDialog::OnSelChangeComboDisplay)
END_MESSAGE_MAP()

BOOL CSampleBlockWalkDialog::OnInitDialog()
{
  AddResizeControl(true);
  EnableSaveDialogPositionAndSize(true);
  SetMinSizeToCurrentSize();

  m_Resize.Add(IDC_BLOCK_TREE, CRhinoUiDialogItemResizer::resize_locktopleft | CRhinoUiDialogItemResizer::resize_lockbottomleft);
  m_Resize.Add(IDC_BLOCK_PREVIEW, CRhinoUiDialogItemResizer::resize_lockall);
  m_Resize.Add(IDC_COMBO_VIEW, CRhinoUiDialogItemResizer::resize_lockbottomleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_COMBO_DISPLAY, CRhinoUiDialogItemResizer::resize_lockbottomleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDOK, CRhinoUiDialogItemResizer::resize_lockbottomleft);

  CRhinoDialog::OnInitDialog();

  // For fun, lets draw the world axes in the preview control....
  m_preview.SetDrawDecorations(CRhinoUiBlockPreview::draw_world_axes);

  m_view_combo.AddString(L"Top");
  m_view_combo.AddString(L"Bottom");
  m_view_combo.AddString(L"Left");
  m_view_combo.AddString(L"Right");
  m_view_combo.AddString(L"Front");
  m_view_combo.AddString(L"Back");
  m_view_combo.AddString(L"Perspective");
  m_view_combo.SetCurSel(0); // Top

  m_display_combo.AddString(L"Wireframe");
  m_display_combo.AddString(L"Shaded");
  m_display_combo.AddString(L"Rendered");
  m_display_combo.SetCurSel(0); // Wireframe

  // Populate the tree control with the instance definition table
  const CRhinoInstanceDefinitionTable& idef_table = m_doc.m_instance_definition_table;
  for (int i = 0; i < idef_table.InstanceDefinitionCount(); i++)
  {
    const CRhinoInstanceDefinition* idef = idef_table[i];
    if (idef && !idef->IsDeleted())
      AddInstanceDefinitionToTreeCtrl(idef, TVI_ROOT, TVI_SORT, true);
  }

  return TRUE;
}

void CSampleBlockWalkDialog::AddInstanceDefinitionToTreeCtrl(const CRhinoInstanceDefinition* idef, HTREEITEM hParent, HTREEITEM hInsertAfter, bool bAddDef)
{
  // I use the tree control's "item data" to store information about the instance definition.
  // If the "item data" is >= 0, then that is the index of the instance definition (in the idef table).
  // If the "item data" is < 0, then that is the an instance definition object's runtime serial number
  // that can be used to quickly look up the object so we can draw it highlighted in the preview control.

  if (idef)
  {
    HTREEITEM hRoot = 0;
    if (bAddDef)
    {
      // Only top-level instance definitions get item data >= 0.
      hRoot = m_tree.InsertItem(idef->Name(), hParent, hInsertAfter);
      m_tree.SetItemData(hRoot, (DWORD_PTR)idef->Index());
    }
    else
      hRoot = hParent;

    ON_SimpleArray<const CRhinoObject*> objects;
    idef->GetObjects(objects);

    // First pass is to add nested blocks
    int i = 0;
    for (i = 0; i < objects.Count(); i++)
    {
      const CRhinoObject* obj = objects[i];
      if (0 == obj)
        continue;

      const CRhinoInstanceObject* p = CRhinoInstanceObject::Cast(obj);
      if (p)
      {
        // If a nested block, then we want to add the runtime serial number of the
        // instance object. Then, we will recursively call this function but set
        // the bAddRef flag to false
        const CRhinoInstanceDefinition* q = p->InstanceDefinition();
        if (q)
        {
          // Format a display name for the tree
          ON_wString s;
          ON_wString d = q->Name();
          ON_wString n = obj->Attributes().m_name;
          if (n.IsEmpty())
            s = d;
          else
            s.Format(L"%s - %s", n, d);

          HTREEITEM hItem = m_tree.InsertItem(s, hRoot, hInsertAfter);
          m_tree.SetItemData(hItem, (DWORD_PTR)-((int)obj->RuntimeSerialNumber()));

          AddInstanceDefinitionToTreeCtrl(q, hItem, hInsertAfter, false);
          objects[i] = 0;
          continue;
        }
      }
    }

    // Second pass is to add objects
    for (i = 0; i < objects.Count(); i++)
    {
      const CRhinoObject* obj = objects[i];
      if (0 == obj)
        continue;

      // Format a display name for the tree
      ON_wString s;
      ON_wString d = obj->ShortDescription(false);
      ON_wString n = obj->Attributes().m_name;
      if (n.IsEmpty())
        s = d;
      else
        s.Format(L"%s - %s", n, d);

      HTREEITEM hItem = m_tree.InsertItem(s, hRoot, hInsertAfter);
      m_tree.SetItemData(hItem, (DWORD_PTR)-((int)obj->RuntimeSerialNumber()));
    }
  }
}

void CSampleBlockWalkDialog::OnSelChangeBlockTree(NMHDR* pNMHDR, LRESULT* pResult)
{
  UNREFERENCED_PARAMETER(pNMHDR);

  // Get the selected item
  HTREEITEM hItem = m_tree.GetSelectedItem();
  if (hItem)
  {
    // Reset
    m_idef = 0;
    const CRhinoObject* idef_obj = 0;
    ON_Xform idef_xf(1);

    // Get the item data
    int idef_index = (int)m_tree.GetItemData(hItem);

    // If the item data < 0, then its a subobject
    if (idef_index < 0)
    {
      // Get subobject to draw
      idef_obj = m_doc.LookupObjectByRuntimeSerialNumber(-idef_index);

      // Walk up the tree and find the top-most parent (idef).
      // Build a tranform along the way.
      while (hItem)
      {
        HTREEITEM hParent = m_tree.GetParentItem(hItem);
        if (hParent)
        {
          idef_index = (int)m_tree.GetItemData(hParent);
          if (idef_index < 0)
          {
            const CRhinoInstanceObject* iref = CRhinoInstanceObject::Cast(m_doc.LookupObjectByRuntimeSerialNumber(-idef_index));
            if (iref)
              idef_xf = idef_xf * iref->InstanceXform();
          }
        }
        hItem = hParent;
      }
    }

    // At this point, idef_index should be >= 0.
    const CRhinoInstanceDefinitionTable& idef_table = m_doc.m_instance_definition_table;
    if (-1 < idef_index && idef_index < idef_table.InstanceDefinitionCount())
      m_idef = idef_table[idef_index];

    // Update the control, only redraw once
    m_preview.SetInstanceDefinitionObject(idef_obj, idef_xf, false);
    m_preview.SetInstanceDefinition(m_idef);
  }

  *pResult = 0;
}

void CSampleBlockWalkDialog::OnSelChangeComboView()
{
  int n = m_view_combo.GetCurSel();
  if (n != CB_ERR)
  {
    CRhinoUiBlockPreview::view_type vt = CRhinoUiBlockPreview::GetViewType(n);
    m_preview.SetViewType(vt, true);
  }
}

void CSampleBlockWalkDialog::OnSelChangeComboDisplay()
{
  int index = m_display_combo.GetCurSel();
  if (index != CB_ERR)
  {
    ON_UUID display_mode = ON_StandardDisplayModeId::Wireframe;
    switch (index)
    {
    case 0:
      display_mode = ON_StandardDisplayModeId::Wireframe;
      break;
    case 1:
      display_mode = ON_StandardDisplayModeId::Shaded;
      break;
    case 2:
      display_mode = ON_StandardDisplayModeId::Rendered;
      break;
    }
    m_preview.SetDisplayMode(display_mode, true);
  }
}
