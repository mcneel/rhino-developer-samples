#include "stdafx.h"
#include "SampleModelessCancelPlugIn.h"
#include "SampleDialog1.h"

IMPLEMENT_DYNAMIC(CSampleDialog1, CRhinoDialog)

CSampleDialog1::CSampleDialog1(CWnd* pParentWnd, CRhinoModelessDialogCommand* pCommand)
  : CRhinoDialog(CSampleDialog1::IDD, pParentWnd)
  , m_pCommand(pCommand)
{
  SetAllowEscapeAndEnter(false);
}

CSampleDialog1::~CSampleDialog1()
{
}

void CSampleDialog1::DoDataExchange(CDataExchange* pDX)
{
	CRhinoDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleDialog1, CRhinoDialog)
  ON_BN_CLICKED(IDC_BTN_GET, &CSampleDialog1::OnGetButton)
  ON_BN_CLICKED(IDC_BTN_CUSTOM, &CSampleDialog1::OnCustomButton)
  ON_BN_CLICKED(IDC_BTN_POINTS, &CSampleDialog1::OnPointsButton)
  ON_BN_CLICKED(IDC_BTN_CANCEL1, &CSampleDialog1::OnCancel1Button)
  ON_BN_CLICKED(IDC_BTN_CANCEL2, &CSampleDialog1::OnCancel2Button)
  ON_BN_CLICKED(IDC_BTN_CANCEL3, &CSampleDialog1::OnCancel3Button)
  ON_BN_CLICKED(IDC_BTN_CLOSE, &CSampleDialog1::OnCloseButton)
END_MESSAGE_MAP()

BOOL CSampleDialog1::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  return TRUE;
}

void CSampleDialog1::OnCloseButton()
{
  DestroyWindow();
}

void CSampleDialog1::DisposeDialog()
{
  CRhinoDialog::OnCancel();
}

void CSampleDialog1::PostNcDestroy()
{
  CRhinoDialog::PostNcDestroy();

  if (m_pCommand)
    m_pCommand->DisposeDialog();

  delete this;
}


/////////////////////////////////////////////////////////////////////////////
// CSampleGetPoints class

class CSampleGetPoints : public CRhinoGetPoint
{
public:
  CSampleGetPoints() = default;
  
  void DynamicDraw(
    CRhinoDisplayPipeline& dp, 
    const ON_3dPoint& pt
  ) override;

  int GetPoints();

public:
  ON_3dPointArray m_point_array;
};

int CSampleGetPoints::GetPoints()
{
  m_point_array.Empty();

  SetCommandPrompt(L"Start of polyline");

  // Expert user tool that allows GetPoint, GetObject, GetString, etc., 
  // to return if a registered Windows message is posted by the static function
  // CRhinoGet::PostCustomWindowsMessage.
  AcceptCustomWindowsMessage(SampleModelessCancelPlugIn().WindowMessage());

  CRhinoGet::result res = GetPoint();

  if (res == CRhinoGet::point)
  {
    m_point_array.Append(Point());

    SetCommandPrompt(L"Next point of polyline");
    AcceptNothing();

    for (;; )
    {
      SetBasePoint(Point());

      res = GetPoint();

      if (res == CRhinoGet::point)
      {
        ON_3dPoint pt = Point();
        if (pt.DistanceTo(m_point_array[m_point_array.Count() - 1]) > ON_ZERO_TOLERANCE)
          m_point_array.Append(Point());
        continue;
      }

      // Windows posted a message id that was in the list passed to CRhinoGet::AcceptWindowsMessage().  
      // Call CRhinoGet::WndMsg() to get the message.
      if (res == CRhinoGet::winmsg)
      {
        RhinoApp().Print(L"CRhinoGet::winmsg\n");
        return 0;
      }

      if (res == CRhinoGet::cancel)
      {
        RhinoApp().Print(L"CRhinoGet::cancel\n");
        return 0;
      }

      if (res == CRhinoGet::nothing)
        break;

      return 0;
    }
  }

  // Windows posted a message id that was in the list passed to CRhinoGet::AcceptWindowsMessage().  
  // Call CRhinoGet::WndMsg() to get the message.
  if (res == CRhinoGet::winmsg)
  {
    RhinoApp().Print(L"CRhinoGet::winmsg\n");
    return 0;
  }

  if (res == CRhinoGet::cancel)
  {
    RhinoApp().Print(L"CRhinoGet::cancel\n");
    return 0;
  }

  return m_point_array.Count();
}

void CSampleGetPoints::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
  if (m_point_array.Count() > 0)
  {
    int i = 0;
    for (i = 1; i < m_point_array.Count(); i++)
    {
      dp.DrawPoint(m_point_array[i - 1]);
      dp.DrawLine(m_point_array[i - 1], m_point_array[i]);
    }
    dp.DrawPoint(m_point_array[i - 1]);
    dp.DrawPoint(pt);
    dp.DrawLine(m_point_array[i - 1], pt);
  }
  CRhinoGetPoint::DynamicDraw(dp, pt);
}

// CSampleGetPoints class
/////////////////////////////////////////////////////////////////////////////


void CSampleDialog1::OnGetButton()
{
  // Standard GetPoint
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Pick a point location");
  for (;;)
  {
    CRhinoGet::result res = gp.GetPoint();
    if (res == CRhinoGet::point)
    {
      ON_wString str;
      RhinoFormatPoint(gp.Point(), str);
      RhinoApp().Print(L"%s\n", static_cast<const wchar_t*>(str));
      gp.SetCommandPrompt(L"Pick another point location. Press Enter when done");
      gp.AcceptNothing();
      continue;
    }

    if (res == CRhinoGet::cancel)
      RhinoApp().Print(L"CRhinoGet::cancel\n");
    else if (res == CRhinoGet::nothing)
      RhinoApp().Print(L"CRhinoGet::nothing\n");
    break;
  }
}

void CSampleDialog1::OnCustomButton()
{
  // Custom GetPoint
  CSampleGetPoints gp;
  int count = gp.GetPoints();
  if (count > 1)
    RhinoApp().Print(L"Point count = %d\n", count);
}

void CSampleDialog1::OnPointsButton()
{
  // Rhino command that picks points
  CRhinoDoc* pDoc = RhinoApp().ActiveDoc();
  if (pDoc)
    RhinoApp().RunScript(pDoc->RuntimeSerialNumber(), L"! _Points", 0);
}

void CSampleDialog1::OnCancel1Button()
{
  // Script a bang character
  CRhinoDoc* pDoc = RhinoApp().ActiveDoc();
  if (pDoc)
    RhinoApp().RunScript(pDoc->RuntimeSerialNumber(), L"!", 0);
}

void CSampleDialog1::OnCancel2Button()
{
  // Post an escape key via WM_CHAR
  ::PostMessage(RhinoApp().MainWnd(), WM_CHAR, (WPARAM)VK_ESCAPE, (LPARAM)0);
}

void CSampleDialog1::OnCancel3Button()
{
  // Post a custom Window message to the getter
  CRhinoGet::PostCustomWindowsMessage(SampleModelessCancelPlugIn().WindowMessage(), 0, 0);
}
