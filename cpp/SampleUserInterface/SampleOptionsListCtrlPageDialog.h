#pragma once

#include "resource.h"

class CSampleOptionsControl : public CRhinoUiOptionsListCtrl
{
public:
  CSampleOptionsControl();
  ~CSampleOptionsControl();

  void OnEndEditItem(CRhinoUiOptionsListCtrlItem& item);
  bool OnItemButtonClicked(CRhinoUiOptionsListCtrlPushButton& item);
  bool OnItemButtonClicked(CRhinoUiOptionsListCtrlCheckBox& item);
  bool PopulateItemComboBox(CRhinoUiOptionsListCtrlComboBox& item);
  bool OnComboBoxPopulated(CRhinoUiOptionsListCtrlComboBox& item);
  void OnItemComboBoxSelChanged(CRhinoUiOptionsListCtrlComboBox& item);

  CRhinoUiOptionsListCtrlCheckBox m_HasIsoCurves;
  CRhinoUiOptionsListCtrlCheckBox m_ShowIsoCurves;
  CRhinoUiOptionsListCtrlEditBox m_IsoCurveDensity;
  CRhinoUiOptionsListCtrlCheckBox m_VerticalCheckBox;
  CRhinoUiOptionsListCtrlCheckBox m_HideTabCheckBox;
  CRhinoUiOptionsListCtrlRadioButtonColumn m_RadioColumn;
  CRhinoUiOptionsListCtrlPointEditBox m_PointEditBox;
  CRhinoUiOptionsListCtrlCheckBox m_ExpandCheckBox1;
  CRhinoUiOptionsListCtrlCheckBox m_ExpandCheckBox2;
  CRhinoUiOptionsListCtrlCheckBox m_ExpandCheckBox3;
  CRhinoUiOptionsListCtrlCheckBox m_ExpandCheckBox4;
  CRhinoUiOptionsListCtrlGroupComboBox m_GroupCombo;
  CRhinoUiOptionsListCtrlGroupComboBox m_NestedGroupCombo;
};


class CSampleOptionsListCtrlPageDialog : public CRhinoTabbedDockBarDialog
{
  DECLARE_DYNCREATE(CSampleOptionsListCtrlPageDialog)

public:
  CSampleOptionsListCtrlPageDialog();

  // Required CRhinoTabbedDockBarDialog overrides
  const wchar_t* Caption() const override;
  ON_UUID TabId() const override;
  static ON_UUID ID();
  ON_UUID PlugInId() const override;
  HICON Icon(const CSize& sizeInPixels) const;

  // Optional CRhinoTabbedDockBarDialog overrides
  void OnShowTab(const class CRhinoUiPanel& db, bool bShowTab, const ON_UUID& dockBarId) override;
  void OnShowDockBar(IDockBarEventWatcher::ShowEventArgs args) override;

  // Dialog Data
  enum { IDD = IDD_LISTCTRL_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg LRESULT OnRegisteredMessage(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

protected:
  friend class CSampleOptionsControl;
  CRhinoUiOptionsListCtrlComboBox* m_pLayerItem;
  CRhinoUiOptionsListCtrlComboBox* m_pColorItem;
  CRhinoUiOptionsListCtrlComboBox m_ComboBoxItem;
  CSampleOptionsControl m_options_list;

  void CreateGroup(
    CRhinoUiOptionsListCtrlGroupComboBox& combo,
    const wchar_t* lpsItem,
    CRhinoUiOptionsListCtrlGroupComboBox* pNestedCombo = NULL
  );
};
