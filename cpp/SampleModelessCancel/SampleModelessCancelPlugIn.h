#pragma once

// CSampleModelessCancelPlugIn

class CSampleModelessCancelPlugIn : public CRhinoUtilityPlugIn
{
public:
  CSampleModelessCancelPlugIn();
  ~CSampleModelessCancelPlugIn() = default;

  // Required overrides
  const wchar_t* PlugInName() const override;
  const wchar_t* PlugInVersion() const override;
  GUID PlugInID() const override;

  // Additional overrides
  BOOL OnLoadPlugIn() override;
  void OnUnloadPlugIn() override;

  unsigned int WindowMessage();

private:
  ON_wString m_plugin_version;
  unsigned int m_window_message;
};

// Return a reference to the one and only CSampleModelessCancelPlugIn object
CSampleModelessCancelPlugIn& SampleModelessCancelPlugIn();

// CRhinoModelessDialogCommand
class CRhinoModelessDialogCommand : public CRhinoCommand
{
public:
  CRhinoModelessDialogCommand(
    bool bTransparent = false,
    bool bDoNotRepeat = false,
    const class CRhinoPlugIn* pPlugIn = nullptr,
    bool bTestCommand = false
  );

  virtual ~CRhinoModelessDialogCommand();

  virtual void DisposeDialog() = 0;
};
