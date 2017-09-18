#pragma once

#include "resource.h"
#include "ColorSwatch.h"
#include "rhinoSdkTMfcPages.h"

// CPropertiesPage dialog

class CPropertiesPage : public TRhinoPropertiesPanelPage<CDialog>
{
	DECLARE_DYNAMIC(CPropertiesPage)

public:
	CPropertiesPage();   // standard constructor
	virtual ~CPropertiesPage();

// Dialog Data
	enum { IDD = IDD_OBJECT_PROPS };

  BOOL OnInitDialog() override;

  /////////////////////////////////////////////////////////////////////////////
  // CRhinoObjectPropertiesDialogPage required overrides
  void UpdatePage(IRhinoPropertiesPanelPageEventArgs& e) override;
  bool IncludeInNavigationControl(IRhinoPropertiesPanelPageEventArgs& e) const override;
  CRhinoCommand::result RunScript(IRhinoPropertiesPanelPageEventArgs& e) override;
  // Optional overrides
  RhinoPropertiesPanelPageType PageType() const override;
  /////////////////////////////////////////////////////////////////////////////
  // CRhinoObjectPropertiesDialogPageEx required overrides
  // Handled by TRhinoPropertiesPanelPage
	//HICON Icon(void) const override;
  /////////////////////////////////////////////////////////////////////////////
  // CRhinoStackedDialogPag required overrides
  const wchar_t* EnglishTitle() const override;
  const wchar_t* LocalTitle() const override;
  /////////////////////////////////////////////////////////////////////////////
  void OnModifyPage(IRhinoPropertiesPanelPageEventArgs& args) override;

public:
  bool IncludeObject(const CRhinoObject* object) const;
  bool IsOurColor(const CRhinoObject* object) const;
  int ToByLayer(const CRhinoObject* object) const;
  int ToObjectColor(const CRhinoObject* object) const;
  void ModifiedMessage(bool byLayer, int modifiedCount) const;

  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnSize(UINT nType, int cx, int cy);

private:
  CColorSwatch m_color_swatch;
  CStatic m_message_text;

private:
  void RunModifyCommand(bool byLayer);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
