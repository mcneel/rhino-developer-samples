#pragma once

#include "Resource.h"

class CSampleOptionsPage : public CRhinoOptionsDialogPage
{
  DECLARE_DYNAMIC(CSampleOptionsPage)

public:
  CSampleOptionsPage(CWnd* pParent = NULL);
  virtual ~CSampleOptionsPage();

  // Dialog Data
  enum { IDD = IDD_OPTIONS_PAGE };
  CRhinoUiSeparator m_sep1;
  CButton m_button;

public:
  int OnApply();
  const wchar_t* EnglishPageTitle();
  const wchar_t* LocalPageTitle();
  CRhinoCommand::result RunScript(const unsigned int rhino_doc_sn);

  /*
  DO NOT DELETE THIS PAGE IN PostNcDestroy()!
  When V4 first shipped, where was a lot of confusion when and where to
  delete stacked dialog pages that were created in plug-ins. The reason
  being that if the page had never been created (you never clicked on it),
  then its PostNcDestroy() member was never called. This confusion led to
  may reported crashes. Thus in later releases, Rhino always calls the
  RhinoDeleteThisPage() member. Thus the only place you need to delete
  the page is here.
  */
  void RhinoDeleteThisPage();

public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton1();

protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  DECLARE_MESSAGE_MAP()
};
