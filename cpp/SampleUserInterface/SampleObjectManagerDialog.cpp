#include "stdafx.h"
#include "SampleObjectManagerDialog.h"
#include "SampleUserInterfacePlugIn.h"

IMPLEMENT_DYNCREATE(CSampleObjectManagerDialog, CRhinoTabbedDockBarDialog)

CSampleObjectManagerDialog::CSampleObjectManagerDialog()
  : CRhinoTabbedDockBarDialog()
{
  CRhinoEventWatcher::Register();
  CRhinoEventWatcher::Enable(FALSE);
}

CSampleObjectManagerDialog::~CSampleObjectManagerDialog()
{
  CRhinoEventWatcher::Enable(FALSE);
  CRhinoEventWatcher::UnRegister();
}

const wchar_t* CSampleObjectManagerDialog::Caption() const
{
  return L"Objects";
}

ON_UUID CSampleObjectManagerDialog::TabId() const
{
  return ID();
}

ON_UUID CSampleObjectManagerDialog::ID()
{
  // {BAB358D5-D9CF-42B5-90AE-064853BACC5B}
  static const GUID SampleObjectManagerDialog_UUID =
  { 0xBAB358D5, 0xD9CF, 0x42B5, { 0x90, 0xAE, 0x06, 0x48, 0x53, 0xBA, 0xCC, 0x5B } };
  return SampleObjectManagerDialog_UUID;
}

ON_UUID CSampleObjectManagerDialog::PlugInId() const
{
  return SampleUserInterfacePlugIn().PlugInID();
}

HICON CSampleObjectManagerDialog::Icon(const CSize& sizeInPixels) const
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return CRhinoDpi::LoadIcon(AfxGetInstanceHandle(), IDI_OBJECT_MANAGER_ICON, sizeInPixels.cx, sizeInPixels.cy);
}

void CSampleObjectManagerDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoTabbedDockBarDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LABEL, m_label);
  DDX_Control(pDX, IDC_LISTBOX, m_listbox);
}

void CSampleObjectManagerDialog::OnShowDockBar(CRhinoUiDockBar::ShowEventArgs args)
{
  if (args == CRhinoUiDockBar::ShowEventArgs::Show)
  {
    CRhinoEventWatcher::Enable(TRUE);
    FillListBox();
  }
  else if (args == CRhinoUiDockBar::ShowEventArgs::Hide)
  {
    CRhinoEventWatcher::Enable(FALSE);
    m_listbox.ResetContent();
  }
}

BEGIN_MESSAGE_MAP(CSampleObjectManagerDialog, CRhinoTabbedDockBarDialog)
  ON_WM_TIMER()
  ON_LBN_SELCHANGE(IDC_LISTBOX, &CSampleObjectManagerDialog::OSelChangeListBox)
END_MESSAGE_MAP()

BOOL CSampleObjectManagerDialog::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg && pMsg->message == WM_KEYDOWN)
  {
    if (pMsg->wParam == VK_F5)
    {
      FillListBox();
      return TRUE;
    }
  }
  return CRhinoTabbedDockBarDialog::PreTranslateMessage(pMsg);
}

BOOL CSampleObjectManagerDialog::OnInitDialog()
{
  m_Resize.Add(IDC_LABEL, CRhinoUiDialogItemResizer::resize_lockleft | CRhinoUiDialogItemResizer::resize_lockright);
  m_Resize.Add(IDC_LISTBOX, CRhinoUiDialogItemResizer::resize_lockall);

  CRhinoTabbedDockBarDialog::OnInitDialog();

  return TRUE;
}

bool CSampleObjectManagerDialog::IsValid() const
{
  if (::IsWindow(m_hWnd) && ::IsWindowVisible(m_hWnd))
    return true;
  return false;
}

void CSampleObjectManagerDialog::CreateTimer(timer_type timer)
{
  if (m_timer_ids[timer])
  {
    KillTimer(m_timer_ids[timer]);
    m_timer_ids[timer] = 0;
  }

  UINT_PTR timer_id = SetTimer(WM_USER + timer, 100, 0);
  if (timer_id)
    m_timer_ids[timer] = timer_id;
  else
    OnTimer(WM_USER + timer);
}

void CSampleObjectManagerDialog::OnTimer(UINT_PTR nIDEvent)
{
  UINT_PTR timer = nIDEvent - WM_USER;
  if (add_timer <= timer && timer <= deselectall_timer)
  {
    KillTimer(m_timer_ids[timer]);
    m_timer_ids[timer] = 0;
  }

  switch (timer)
  {
  case add_timer:
    AddHelper();
    break;

  case delete_timer:
    DeleteHelper();
    break;

  case select_timer:
    SelectHelper();
    break;

  case deselect_timer:
    DeselectHelper();
    break;

  case deselectall_timer:
    DeselectAllHelper();
    break;
  }

  CRhinoTabbedDockBarDialog::OnTimer(nIDEvent);
}

void CSampleObjectManagerDialog::OnNewDocument(CRhinoDoc& doc)
{
  UNREFERENCED_PARAMETER(doc);
}

void CSampleObjectManagerDialog::OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(filename);
  UNREFERENCED_PARAMETER(bMerge);
  UNREFERENCED_PARAMETER(bReference);
}

void CSampleObjectManagerDialog::OnCloseDocument(CRhinoDoc& doc)
{
  UNREFERENCED_PARAMETER(doc);
  m_listbox.ResetContent();
}

void CSampleObjectManagerDialog::OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(command);
  UNREFERENCED_PARAMETER(context);
}

void CSampleObjectManagerDialog::OnEndCommand(const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc)
{
  UNREFERENCED_PARAMETER(command);
  UNREFERENCED_PARAMETER(context);
  UNREFERENCED_PARAMETER(rc);
}

void CSampleObjectManagerDialog::OnAddObject(CRhinoDoc& doc, CRhinoObject& object)
{
  UNREFERENCED_PARAMETER(doc);
  if (IsValid())
  {
    m_add_ids.AddUuid(object.Attributes().m_uuid);
    CreateTimer(add_timer);
  }
}

void CSampleObjectManagerDialog::OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object)
{
  UNREFERENCED_PARAMETER(doc);
  if (IsValid())
  {
    m_delete_ids.AddUuid(object.Attributes().m_uuid);
    CreateTimer(delete_timer);
  }
}

void CSampleObjectManagerDialog::OnSelectObject(CRhinoDoc& doc, const CRhinoObject& object)
{
  UNREFERENCED_PARAMETER(doc);
  if (IsValid())
  {
    m_select_ids.AddUuid(object.Attributes().m_uuid);
    CreateTimer(select_timer);
  }
}

void CSampleObjectManagerDialog::OnSelectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects)
{
  UNREFERENCED_PARAMETER(doc);
  if (IsValid())
  {
    for (int i = 0; i < objects.Count(); i++)
    {
      const CRhinoObject* object = objects[i];
      if (object)
        m_select_ids.AddUuid(object->Attributes().m_uuid);
    }
    CreateTimer(select_timer);
  }
}

void CSampleObjectManagerDialog::OnDeselectObject(CRhinoDoc& doc, const CRhinoObject& object)
{
  UNREFERENCED_PARAMETER(doc);
  if (IsValid())
  {
    m_deselect_ids.AddUuid(object.Attributes().m_uuid);
    CreateTimer(deselect_timer);
  }
}

void CSampleObjectManagerDialog::OnDeselectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects)
{
  UNREFERENCED_PARAMETER(doc);
  if (IsValid())
  {
    for (int i = 0; i < objects.Count(); i++)
    {
      const CRhinoObject* object = objects[i];
      if (object)
        m_deselect_ids.AddUuid(object->Attributes().m_uuid);
    }
    CreateTimer(deselect_timer);
  }
}

void CSampleObjectManagerDialog::OnDeselectAllObjects(CRhinoDoc& doc, int count)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(count);
  if (IsValid())
    CreateTimer(deselectall_timer);
}

void CSampleObjectManagerDialog::AddHelper()
{
  const ON_UUID* pids = m_add_ids.Array();
  if (pids)
  {
    int i, index = -1;
    ON_wString str;
    for (i = 0; i < m_add_ids.Count(); i++)
    {
      ON_UuidToString(pids[i], str);
      str.MakeLower();
      index = m_listbox.AddString(str);
    }
  }
  m_add_ids.Empty();
}

void CSampleObjectManagerDialog::DeleteHelper()
{
  const ON_UUID* pids = m_delete_ids.Array();
  if (pids)
  {
    int i, index = -1;
    ON_wString str;
    for (i = 0; i < m_delete_ids.Count(); i++)
    {
      ON_UuidToString(pids[i], str);
      str.MakeLower();
      index = m_listbox.FindStringExact(index, str);
      if (index != LB_ERR)
        m_listbox.DeleteString(index);
      else
        break;
    }
  }
  m_delete_ids.Empty();
}

void CSampleObjectManagerDialog::SelectHelper()
{
  const ON_UUID* pids = m_select_ids.Array();
  if (pids)
  {
    int i, index = -1;
    ON_wString str;
    for (i = 0; i < m_select_ids.Count(); i++)
    {
      ON_UuidToString(pids[i], str);
      str.MakeLower();
      index = m_listbox.FindStringExact(index, str);
      if (index != LB_ERR)
        m_listbox.SetSel(index, TRUE);
      else
        break;
    }
  }
  m_select_ids.Empty();
}

void CSampleObjectManagerDialog::DeselectHelper()
{
  const ON_UUID* pids = m_deselect_ids.Array();
  if (pids)
  {
    int i, index = -1;
    ON_wString str;
    for (i = 0; i < m_deselect_ids.Count(); i++)
    {
      ON_UuidToString(pids[i], str);
      str.MakeLower();
      index = m_listbox.FindStringExact(index, str);
      if (index != LB_ERR)
        m_listbox.SetSel(index, FALSE);
      else
        break;
    }
  }
  m_deselect_ids.Empty();
}

void CSampleObjectManagerDialog::DeselectAllHelper()
{
  m_listbox.SetSel(-1, FALSE);
  m_deselect_ids.Empty();
}

void CSampleObjectManagerDialog::FillListBox()
{
  m_listbox.SetRedraw(FALSE);
  m_listbox.ResetContent();

  CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_and_reference_objects);
  it.IncludeLights(true);
  it.IncludeGrips(false);

  const CRhinoObject* obj = 0;
  ON_wString str;
  for (obj = it.First(); obj; obj = it.Next())
  {
    ON_UuidToString(obj->Attributes().m_uuid, str);
    str.MakeLower();
    int index = m_listbox.AddString(str);
    if (obj->IsSelected())
      m_listbox.SetSel(index, TRUE);
  }

  m_listbox.SetRedraw(TRUE);
  m_listbox.Invalidate();
  m_listbox.UpdateWindow();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectManagerSelect command
//

#pragma region SampleObjectManagerSelect command

class CCommandSampleObjectManagerSelect : public CRhinoSelCommand
{
public:
  CCommandSampleObjectManagerSelect();
  ~CCommandSampleObjectManagerSelect() {}
  UUID CommandUUID()
  {
    // {6815BA85-F701-4A7F-8D2C-F6251EEB5C5E}
    static const GUID SampleObjectManagerSelectCommand_UUID =
    { 0x6815BA85, 0xF701, 0x4A7F, { 0x8D, 0x2C, 0xF6, 0x25, 0x1E, 0xEB, 0x5C, 0x5E } };
    return SampleObjectManagerSelectCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleObjectManagerSelect"; }
  const wchar_t* LocalCommandName() const { return L"SampleObjectManagerSelect"; }
  bool SelFilter(const CRhinoObject* object);

  void SetModelObjectId(ON_UUID model_object_id);
  void SetModelObjectIds(const ON_UuidList& model_object_ids);

private:
  ON_UuidList m_model_object_ids;
};

// The one and only CCommandSampleObjectManagerSelect object
static class CCommandSampleObjectManagerSelect theSampleObjectManagerSelectCommand;

CCommandSampleObjectManagerSelect::CCommandSampleObjectManagerSelect()
  : CRhinoSelCommand(0, true) // Make this a test command so it does not auto-complete (e.g. hidden)
{
}

bool CCommandSampleObjectManagerSelect::SelFilter(const CRhinoObject* object)
{
  if (object && m_model_object_ids.FindUuid(object->ModelObjectId()))
    return true;
  return false;
}

void CCommandSampleObjectManagerSelect::SetModelObjectId(ON_UUID model_object_id)
{
  m_model_object_ids.Empty();
  m_model_object_ids.AddUuid(model_object_id);
  CRhinoDoc* doc = RhinoApp().ActiveDoc();
  if (0 != doc)
    doc->UnselectAll();
}
void CCommandSampleObjectManagerSelect::SetModelObjectIds(const ON_UuidList& model_object_ids)
{
  m_model_object_ids.Empty();
  m_model_object_ids = model_object_ids;
  CRhinoDoc* doc = RhinoApp().ActiveDoc();
  if (0 != doc)
    doc->UnselectAll();
}

#pragma endregion

//
// END SampleObjectManagerSelect command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


void CSampleObjectManagerDialog::OSelChangeListBox()
{
  const int count = m_listbox.GetSelCount();
  if (count)
  {
    CRhinoEventWatcher::Enable(FALSE);

    ON_SimpleArray<int> items(count);
    items.SetCount(count);
    m_listbox.GetSelItems(count, items.Array());

    ON_UuidList uuid_list;
    for (int i = 0; i < count; i++)
    {
      int length = m_listbox.GetTextLen(items[i]);
      ON_wString text;
      text.ReserveArray(length + 2);
      text.SetLength(length);
      m_listbox.GetText(items[i], text.Array());

      ON_UUID uuid = ON_UuidFromString(text);
      if (ON_UuidIsNotNil(uuid))
        uuid_list.AddUuid(uuid);
    }

    if (uuid_list.Count())
    {
      theSampleObjectManagerSelectCommand.SetModelObjectIds(uuid_list);
      RhinoApp().RunScript(L"_SampleObjectManagerSelect", 0);
    }

    CRhinoEventWatcher::Enable(TRUE);
  }
}
