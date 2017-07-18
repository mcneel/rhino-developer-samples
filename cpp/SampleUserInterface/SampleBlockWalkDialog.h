#pragma once

#include "Resource.h"

class CSampleBlockWalkDialog : public CRhinoDialog
{
  DECLARE_DYNAMIC(CSampleBlockWalkDialog)

public:
  CSampleBlockWalkDialog(CWnd* pParent, CRhinoDoc& doc);

  // Dialog Data
  enum { IDD = IDD_BLOCKWALK_DIALOG };
  CTreeCtrl m_tree;
  CRhinoUiBlockPreview m_preview;
  CComboBox m_view_combo;
  CComboBox m_display_combo;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnSelChangeBlockTree(NMHDR *pNMHDR, LRESULT* pResult);
  afx_msg void OnSelChangeComboView();
  afx_msg void OnSelChangeComboDisplay();
  DECLARE_MESSAGE_MAP()

private:
  void AddInstanceDefinitionToTreeCtrl(const CRhinoInstanceDefinition* idef, HTREEITEM hParent, HTREEITEM hInsertAfter, bool bAddDef);

private:
  CRhinoDoc& m_doc;
  const CRhinoInstanceDefinition* m_idef;
};
