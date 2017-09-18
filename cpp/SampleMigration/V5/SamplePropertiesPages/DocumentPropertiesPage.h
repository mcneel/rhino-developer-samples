#pragma once

#include "resource.h"
#include "ColorButton.h"

// CDocumentPropertiesPage dialog

class CDocumentPropertiesPage : public CRhinoOptionsDialogPage
{
	DECLARE_DYNAMIC(CDocumentPropertiesPage)

public:
	CDocumentPropertiesPage(CRhinoDoc& doc);   // standard constructor
	virtual ~CDocumentPropertiesPage();

  /////////////////////////////////////////////////////////////////////////////
  // CRhinoOptionsDialogPage required overrides
  const wchar_t* EnglishPageTitle() override;
  const wchar_t* LocalPageTitle() override;
  CRhinoCommand::result RunScript( CRhinoDoc& rhino_doc) override;
  /////////////////////////////////////////////////////////////////////////////
  // CRhinoStackedDialogPage overrides
  int OnActivate( BOOL bActive) override;
  int OnApply() override;
  bool ShowDefaultsButton(void) override { return true; }
  void OnDefaults(void) override;
  /////////////////////////////////////////////////////////////////////////////

// Dialog Data
	enum { IDD = IDD_DOC_PROPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  CColorButton m_color_button;
  CRhinoDoc& m_doc;
};
