#pragma once

#include "Resource.h"

class CSampleObjectPropertiesPageDialog : public CRhinoObjectPropertiesDialogPage
{
public:
  CSampleObjectPropertiesPageDialog(UINT nID, CWnd* pParent = NULL);

  // Dialog Data
  enum { IDD = IDD_OBJPROPS_DIALOG };
  CRhinoUiSeparator m_separator;
  CEdit	m_edit;
  CButton	m_button;

  ON_wString m_hyperlink;
  bool m_bVaries;

  // Required CRhinoObjectPropertiesDialogPageEx overrides
  HICON Icon(const CSize& sizeInPixels) const;

  // Required CRhinoObjectPropertiesDialogPage overrides
  void InitControls(const CRhinoObject* new_obj = 0);
  bool SupportsSubObjectSelection() const;
  BOOL AddPageToControlBar(const CRhinoObject* obj = 0) const;
  CRhinoCommand::result RunScript(ON_SimpleArray<const CRhinoObject*>& objects);

  // Required CRhinoStackedDialogPage overrides
  const wchar_t* EnglishPageTitle() { return L"Hyperlink"; }
  const wchar_t* LocalPageTitle() { return L"Hyperlink"; }

  // Optional CRhinoObjectPropertiesDialogPage overrides
  CRhinoObjectPropertiesDialogPage::page_type PageType() const;
  void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes);

  // CRITICAL CRhinoStackedDialogPage override
  void RhinoDeleteThisPage();

  void InitControls(ON_SimpleArray<const CRhinoObject*>& objects);
  void ModifyObjectList(const wchar_t* hyperlink);
  void ModifyObjectList(ON_SimpleArray<const CRhinoObject*>& objects, const wchar_t* hyperlink);

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();
  afx_msg void OnEditChange();
  afx_msg void OnEditKillFocus();
  afx_msg void OnButton();
  virtual void PostNcDestroy();
  DECLARE_MESSAGE_MAP()
};
