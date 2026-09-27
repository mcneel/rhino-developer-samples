
#pragma once

class CSampleRdkAddRdkMaterialsPlugIn : public CRhinoRenderPlugIn
{
public:
  CSampleRdkAddRdkMaterialsPlugIn();
  virtual ~CSampleRdkAddRdkMaterialsPlugIn();

  // Required overrides
  virtual const wchar_t* PlugInName() const override;
  virtual const wchar_t* PlugInVersion() const override;
  virtual GUID PlugInID() const override;
  virtual int OnLoadPlugIn() override;
  virtual void OnUnloadPlugIn() override;
  virtual CRhinoCommand::result Render( const CRhinoCommandContext& context, bool render_preview) override;
  virtual plugin_load_time PlugInLoadTime() override { return load_plugin_at_startup; }

  // Online help overrides
  virtual BOOL32 AddToPlugInHelpMenu() const override;
  virtual BOOL32 OnDisplayPlugInHelp(HWND hWnd) const override;

private:
  ON_wString m_plugin_version;
  class CSampleRdkAddRdkMaterialsRdkPlugIn* m_pRdkPlugIn = nullptr;

  // TODO: Add additional class information here
};

CSampleRdkAddRdkMaterialsPlugIn& SampleRdkAddRdkMaterialsPlugIn();
