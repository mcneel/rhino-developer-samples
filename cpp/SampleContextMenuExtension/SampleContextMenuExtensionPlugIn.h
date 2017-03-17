/////////////////////////////////////////////////////////////////////////////
// SampleContextMenuExtensionPlugIn.h : main header file for the SampleContextMenuExtension plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSampleLayerContextMenuExtension
// See SampleContextMenuExtensionPlugIn.cpp for the implementation of this class
//

class CSampleLayerContextMenuExtension : public CRhinoContextMenuExtension
{
public:
  CSampleLayerContextMenuExtension();
  ~CSampleLayerContextMenuExtension();

  bool ShouldExtendContextMenu( CRhinoContextMenuContext& context );
  void ExtendContextMenu( CRhinoContextMenuContext& context, CRhinoContextMenu& context_menu );
	void OnCommand( CRhinoContextMenuContext& context, int iAddItemID, UINT nID, CRhinoContextMenu& context_menu );
  void OnInitPopupMenu( CRhinoContextMenuContext& context, HWND hWnd, HMENU hMenuOriginal, HMENU hMenuRuntime, CRhinoContextMenu& context_menu);

private:
  int m_iAddItemID0;
  int m_iAddItemID1;
  int m_iAddItemID2;
};

/////////////////////////////////////////////////////////////////////////////
// CSampleContextMenuExtensionPlugIn
// See SampleContextMenuExtensionPlugIn.cpp for the implementation of this class
//

class CSampleContextMenuExtensionPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleContextMenuExtensionPlugIn();
  ~CSampleContextMenuExtensionPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
  CSampleLayerContextMenuExtension m_context_menu_extension;
};

CSampleContextMenuExtensionPlugIn& SampleContextMenuExtensionPlugIn();