#include "stdafx.h"
#include "SampleLayerContextMenuExtension.h"
#include "SampleUserInterfacePlugIn.h"
#include "resource.h"

CSampleLayerContextMenuExtension::CSampleLayerContextMenuExtension(CRhinoPlugIn& thePlugIn)
  : CRhinoContextMenuExtension(thePlugIn, *(thePlugIn.PlugInModuleState()), true)
{
}

CSampleLayerContextMenuExtension::~CSampleLayerContextMenuExtension()
{
}

bool CSampleLayerContextMenuExtension::ShouldExtendContextMenu(CRhinoContextMenuContext& context)
{
  // Extend the layer list's context menu
  return (0 == ON_UuidCompare(context.m_uuid, CRhinoContextMenuExtension::UUIDLayerList()));
}

void CSampleLayerContextMenuExtension::ExtendContextMenu(CRhinoContextMenuContext& context, CRhinoContextMenu& context_menu)
{
  if (0 != ON_UuidCompare(context.m_uuid, CRhinoContextMenuExtension::UUIDLayerList()))
    return;

  // Required to access our plug-in's resources
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if (0 == m_menu.GetSafeHmenu())
    m_menu.LoadMenu(IDR_SAMPLE_MENU);

  if (0 != m_menu.GetSafeHmenu())
  {
    // A menu can have many sub-menus. We only want the first one...
    CMenu* sub_menu = m_menu.GetSubMenu(0);
    if (0 != sub_menu)
      context_menu.AddItem(L"SampleMenu", sub_menu->GetSafeHmenu());
  }
}

void CSampleLayerContextMenuExtension::OnCommand(CRhinoContextMenuContext& context, int iAddItemID, UINT nID, CRhinoContextMenu& context_menu)
{
  UNREFERENCED_PARAMETER(iAddItemID);
  UNREFERENCED_PARAMETER(context_menu);

  if (0 != ON_UuidCompare(context.m_uuid, CRhinoContextMenuExtension::UUIDLayerList()))
    return;

  if (nullptr != context.m_doc)
  {
    // Get selected layers...
    for (int i = 0; i < context.m_uuids.Count(); i++)
    {
      int layer_index = context.m_doc->m_layer_table.FindLayerFromId(context.m_uuids[i], true, false, -1);
      if (layer_index >= 0 && layer_index < context.m_doc->m_layer_table.LayerCount())
      {
        ON_wString layer_path_name;
        context.m_doc->m_layer_table.GetLayerPathName(layer_index, layer_path_name);
        RhinoApp().Print(L"%s\n", (const wchar_t*)layer_path_name);
      }
    }
  }

  SampleUserInterfacePlugIn().OnPlugInMenuCommand((WPARAM)nID);
}

void CSampleLayerContextMenuExtension::OnInitPopupMenu(CRhinoContextMenuContext& context, HWND hWnd, HMENU hMenuOriginal, HMENU hMenuRuntime, CRhinoContextMenu& context_menu)
{
  UNREFERENCED_PARAMETER(hWnd);

  if (0 != ON_UuidCompare(context.m_uuid, CRhinoContextMenuExtension::UUIDLayerList()))
    return;

  if (0 != hMenuOriginal && 0 != hMenuRuntime)
  {
    UINT uCheck = (SampleUserInterfacePlugIn().ScriptMode()) ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED;
    UINT uIDCheckItem = context_menu.RuntimePopupMenuItemID(hMenuOriginal, ID_SAMPLEMENU_SCRIPTMODE);
    ::CheckMenuItem(hMenuRuntime, uIDCheckItem, uCheck);
  }

}
