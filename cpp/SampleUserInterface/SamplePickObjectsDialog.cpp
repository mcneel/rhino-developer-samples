#include "stdafx.h"
#include "SamplePickObjectsDialog.h"

IMPLEMENT_DYNAMIC(CSamplePickObjectsDialog, CRhinoDialog)

CSamplePickObjectsDialog::CSamplePickObjectsDialog(CWnd* pParent, unsigned int document_sn)
  : CRhinoDialog(CSamplePickObjectsDialog::IDD, pParent)
{
  SetEnableDisplayCommands(TRUE, document_sn);
}

CSamplePickObjectsDialog::~CSamplePickObjectsDialog()
{
}

void CSamplePickObjectsDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_MESSAGE, m_message);
}

BEGIN_MESSAGE_MAP(CSamplePickObjectsDialog, CRhinoDialog)
  ON_BN_CLICKED(IDC_PICK, &CSamplePickObjectsDialog::OnBnClickedPick)
END_MESSAGE_MAP()


BOOL CSamplePickObjectsDialog::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  m_message.SetWindowText(L"0 objects picked.");

  return TRUE;
}

void CSamplePickObjectsDialog::OnBnClickedPick()
{
  // If our dialog contains controls that had values we cared about,
  // you will want to call UpdateData() before hiding the dialog.
  //UpdateData(TRUE);

  // Hide this dialog
  PushPickButton(TRUE);

  // Pick some objects
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Pick objects");
  go.GetObjects(1, 0);
  if (go.CommandResult() == CRhinoCommand::success)
  {
    int count = go.ObjectCount();
    ON_wString str;
    if (1 == count)
      str = L"1 object picked.";
    else
      str.Format(L"%d objects picked.", count);
    m_message.SetWindowText(str);
  }

  // Hide this dialog
  PushPickButton(FALSE);

  // If our dialog contains controls that had values we cared about,
  // you will want to call UpdateData() before showing the dialog.
  //UpdateData(FALSE);
}
