#pragma once

#include "resource.h"

class CSampleOptionsListCtrl : public CRhinoUiOptionsListCtrl
{
public:
  CSampleOptionsListCtrl();
  ~CSampleOptionsListCtrl();

  void OnEndEditItem(CRhinoUiOptionsListCtrlItem& item);
  bool OnItemButtonClicked(CRhinoUiOptionsListCtrlPushButton& item);
  bool OnItemButtonClicked(CRhinoUiOptionsListCtrlCheckBox& item);
  bool PopulateItemComboBox(CRhinoUiOptionsListCtrlComboBox& item);
  bool OnComboBoxPopulated(CRhinoUiOptionsListCtrlComboBox& item);
  void OnItemComboBoxSelChanged(CRhinoUiOptionsListCtrlComboBox& item);

  CRhinoUiOptionsListCtrlRadioButtonColumn m_RadioColumn;
};


class CSampleOptionsListCtrlDlg : public CRhinoDialog
{
  DECLARE_DYNCREATE(CSampleOptionsListCtrlDlg)

public:
  CSampleOptionsListCtrlDlg();
  virtual ~CSampleOptionsListCtrlDlg();

  // Dialog Data
  enum { IDD = IDD_OPTIONS_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();
  afx_msg LRESULT OnRegisteredMessage(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

protected:
  friend class CSampleOptionsListCtrl;
  CSampleOptionsListCtrl m_options_list;
};
