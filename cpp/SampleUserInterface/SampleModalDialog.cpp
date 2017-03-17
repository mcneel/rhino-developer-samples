#include "stdafx.h"
#include "SampleModalDialog.h"

IMPLEMENT_DYNAMIC(CSampleModalDialog, CDialog)

CSampleModalDialog::CSampleModalDialog(CWnd* pParent /*=NULL*/)
  : CDialog(CSampleModalDialog::IDD, pParent)
{
}

CSampleModalDialog::~CSampleModalDialog()
{
}

void CSampleModalDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleModalDialog, CDialog)
END_MESSAGE_MAP()

// CSampleModalDialog message handlers
