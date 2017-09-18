#pragma once

#include "resource.h"
#include "ColorButton.h"

// CDocumentPropertiesPage dialog

class CDocumentPropertiesPage : public TRhinoOptionsPage<CDialog>
{
	DECLARE_DYNAMIC(CDocumentPropertiesPage)

public:
	CDocumentPropertiesPage();   // standard constructor
	virtual ~CDocumentPropertiesPage();

  /////////////////////////////////////////////////////////////////////////////
  // CRhinoOptionsDialogPage required overrides
  const wchar_t* EnglishTitle() const override;
  const wchar_t* LocalTitle() const override;
  CRhinoCommand::result RunScript(CRhinoOptionsPageEventArgs& e) override;
  /////////////////////////////////////////////////////////////////////////////
  // CRhinoStackedDialogPage overrides
  void UpdatePage(CRhinoOptionsPageEventArgs& e) override;
  bool Apply(CRhinoOptionsPageEventArgs& e) override;
  RhinoOptionPageButtons ButtonsToDisplay() const override { return RhinoOptionPageButtons::DefaultButton; }
  void OnRestoreDefaultsClick(CRhinoOptionsPageEventArgs& e) override;
  /////////////////////////////////////////////////////////////////////////////

// Dialog Data
	enum { IDD = IDD_DOC_PROPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CColorButton m_color_button;
};
