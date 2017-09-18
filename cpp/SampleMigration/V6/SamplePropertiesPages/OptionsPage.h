#pragma once

#include "resource.h"
#include "ColorButton.h"
#include "rhinoSdkTMfcPages.h"

// COptionsPage dialog

class COptionsPage : public TRhinoOptionsPage<CDialog>
{
	DECLARE_DYNAMIC(COptionsPage)

public:
	COptionsPage();   // standard constructor
	virtual ~COptionsPage();

  /////////////////////////////////////////////////////////////////////////////
  // CRhinoOptionsDialogPage required overrides
  const wchar_t* EnglishTitle() const override;
  const wchar_t* LocalTitle() const override;
  CRhinoCommand::result RunScript(CRhinoOptionsPageEventArgs& e) override;
  /////////////////////////////////////////////////////////////////////////////
  // CRhinoStackedDialogPage overrides
  void UpdatePage(CRhinoOptionsPageEventArgs& e) override;
  bool Apply(CRhinoOptionsPageEventArgs& e) override;
  RhinoOptionPageButtons ButtonsToDisplay() const override
  {
    // You may use the following to display both the "Restore Defaults" and "Apply" buttons:
    // RhinoOptionPageButtons::DefaultButton | RhinoOptionPageButtons::ApplyButton
    return RhinoOptionPageButtons::DefaultButton;
  }
  void OnRestoreDefaultsClick(CRhinoOptionsPageEventArgs& e) override;
  /////////////////////////////////////////////////////////////////////////////

// Dialog Data
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CColorButton m_obj_color_button;
};
