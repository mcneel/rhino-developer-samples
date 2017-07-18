#pragma once

class CSampleLayerContextMenuExtension : public CRhinoContextMenuExtension
{
public:
  CSampleLayerContextMenuExtension(CRhinoPlugIn& thePlugIn);
  ~CSampleLayerContextMenuExtension();

  bool ShouldExtendContextMenu(CRhinoContextMenuContext& context) override;
  void ExtendContextMenu(CRhinoContextMenuContext& context, CRhinoContextMenu& context_menu) override;
  void OnCommand(CRhinoContextMenuContext& context, int iAddItemID, UINT nID, CRhinoContextMenu& context_menu) override;
  void OnInitPopupMenu(CRhinoContextMenuContext& context, HWND hWnd, HMENU hMenuOriginal, HMENU hMenuRuntime, CRhinoContextMenu& context_menu) override;

private:
  CMenu m_menu;
};