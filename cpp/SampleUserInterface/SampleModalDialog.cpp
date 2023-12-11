#include "stdafx.h"
#include "SampleModalDialog.h"

IMPLEMENT_DYNAMIC(CSampleModalDialog, CRhinoDialog)

CSampleModalDialog::CSampleModalDialog(CWnd* pParent /*=NULL*/)
  : CRhinoDialog(CSampleModalDialog::IDD, pParent)
{
}

void CSampleModalDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleModalDialog, CRhinoDialog)
END_MESSAGE_MAP()

// CSampleModalDialog message handlers

BOOL CSampleModalDialog::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  return TRUE;
}
