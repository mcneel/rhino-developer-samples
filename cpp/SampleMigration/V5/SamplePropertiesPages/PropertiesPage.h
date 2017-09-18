#pragma once

#include "resource.h"
#include "ColorSwatch.h"

// CPropertiesPage dialog

class CPropertiesPage : public CRhinoObjectPropertiesDialogPageEx
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
  void InitControls( const CRhinoObject* new_obj = NULL) override;
  BOOL AddPageToControlBar( const CRhinoObject* obj = NULL) const override;
  CRhinoCommand::result RunScript( ON_SimpleArray<const CRhinoObject*>& objects) override;
  // Optional overrides
  page_type PageType() const override;
  /////////////////////////////////////////////////////////////////////////////
  // CRhinoObjectPropertiesDialogPageEx required overrides
  HICON Icon(void) const override;
  /////////////////////////////////////////////////////////////////////////////
  // CRhinoStackedDialogPag required overrides
  const wchar_t* EnglishPageTitle() override;
  const wchar_t* LocalPageTitle() override;
  /////////////////////////////////////////////////////////////////////////////

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
  mutable HICON m_icon;

private:
  void RunModifyCommand(bool byLayer);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
