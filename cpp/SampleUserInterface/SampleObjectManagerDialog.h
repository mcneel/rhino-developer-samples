#pragma once

#include "Resource.h"

class CSampleObjectManagerDialog : public CRhinoTabbedDockBarDialog, public CRhinoEventWatcher
{
  DECLARE_DYNCREATE(CSampleObjectManagerDialog)

public:
  CSampleObjectManagerDialog();
  virtual ~CSampleObjectManagerDialog();

  // Required CRhinoTabbedDockBarDialog overrides
  const wchar_t* Caption() const override;
  ON_UUID TabId() const override;
  static ON_UUID ID();
  ON_UUID PlugInId() const override;

  // Optional CRhinoTabbedDockBarDialog overrides
  void OnShowDockBar(IDockBarEventWatcher::ShowEventArgs args) override;

  // Optional CRhinoEventWatcher overrides
  void OnNewDocument(CRhinoDoc& doc) override;
  void OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference) override;
  void OnCloseDocument(CRhinoDoc& doc) override;
  void OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context) override;
  void OnEndCommand(const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc) override;
  void OnAddObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnSelectObject(CRhinoDoc& doc, const CRhinoObject& object) override;
  void OnSelectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects) override;
  void OnDeselectObject(CRhinoDoc& doc, const CRhinoObject& object) override;
  void OnDeselectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects) override;
  void OnDeselectAllObjects(CRhinoDoc& doc, int count) override;

  // Dialog Data
  enum { IDD = IDD_OBJECT_MANAGER_DIALOG };
  CStatic m_label;
  CListBox m_listbox;

  ON_UuidList m_add_ids;
  ON_UuidList m_delete_ids;
  ON_UuidList m_select_ids;
  ON_UuidList m_deselect_ids;

  UINT_PTR m_timer_ids[5];
  enum timer_type
  {
    add_timer = 0,
    delete_timer,
    select_timer,
    deselect_timer,
    deselectall_timer,
  };
  void CreateTimer(timer_type timer);

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OSelChangeListBox();
  DECLARE_MESSAGE_MAP()

private:
  bool IsValid() const;
  void AddHelper();
  void DeleteHelper();
  void SelectHelper();
  void DeselectHelper();
  void DeselectAllHelper();
  void FillListBox();
};
