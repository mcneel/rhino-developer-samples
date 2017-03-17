#pragma once

class CSampleRenderer;


// CSampleViewportRendererDisplayModeChanged event watcher
class CSampleViewportRendererDisplayModeChanged : public CRhinoDisplayModeChanged
{
public:
  CSampleViewportRendererDisplayModeChanged();
  virtual void Notify(const class CRhinoDisplayModeChanged::CParameters& params);
};


// CSampleViewportRendererPlugIn plug-in
class CSampleViewportRendererPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleViewportRendererPlugIn();
  ~CSampleViewportRendererPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  // Online help overrides
  BOOL AddToPlugInHelpMenu() const;
  BOOL OnDisplayPlugInHelp(HWND hWnd) const;

  // Load time override
  CRhinoPlugIn::plugin_load_time PlugInLoadTime();

  // Custom renderer members
  GUID DisplayModeID() const;
  void StartRenderer();
  void StopRenderer();

  // Rendered image access
  CRhinoDib* LockRenderImage();
  void UnlockRenderImage(CRhinoDib* pRenderImage);

  // Viewport updating
  void RequestRedraw();
  void AcknowledgeRedraw();
  void OnRedrawTimer();

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here

  // Critical section for rendered image access
  CCriticalSection m_cs;

  // Dib to contain the rendered image
  CRhinoDib m_renderImageDib;

  // Renderer
  CSampleRenderer* m_pRenderer;

  // Viewport update request flag
  bool m_bRedrawRequested;

  // Viewport update timer id
  UINT_PTR m_uTimerProcId;

  // Viewport update timer event
  static UINT s_uTimerProcEvent;

  // Viewport update timer callback
  static void CALLBACK RedrawTimerProc(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);

  // Watches for display mode changes
  CSampleViewportRendererDisplayModeChanged m_display_mode_watcher;
};

CSampleViewportRendererPlugIn& SampleViewportRendererPlugIn();



