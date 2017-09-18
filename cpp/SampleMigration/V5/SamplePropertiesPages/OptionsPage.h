#pragma once

#include "resource.h"
#include "ColorButton.h"

// COptionsPage dialog

class COptionsPage : public CRhinoOptionsDialogPage
{
	DECLARE_DYNAMIC(COptionsPage)

public:
	COptionsPage();   // standard constructor
	virtual ~COptionsPage();

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
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CColorButton m_obj_color_button;
};
