#include "stdafx.h"
#include "SampleModelessCancelPlugIn.h"
#include "SampleDialog2.h"

IMPLEMENT_DYNAMIC(CSampleDialog2, CRhinoDialog)

CSampleDialog2::CSampleDialog2(CWnd* pParentWnd, CRhinoModelessDialogCommand* pCommand)
  : CRhinoDialog(CSampleDialog2::IDD, pParentWnd)
  , m_pCommand(pCommand)
{
  SetAllowEscapeAndEnter(false);
}

CSampleDialog2::~CSampleDialog2()
{
}

void CSampleDialog2::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleDialog2, CRhinoDialog)
  ON_BN_CLICKED(IDC_BTN_CLOSE, &CSampleDialog2::OnCloseButton)
  ON_BN_CLICKED(IDC_BTN_GET, &CSampleDialog2::OnGetButton)
  ON_BN_CLICKED(IDC_BTN_CANCEL1, &CSampleDialog2::OnCancel1Button)
  ON_BN_CLICKED(IDC_BTN_CANCEL2, &CSampleDialog2::OnCancel2Button)
  ON_BN_CLICKED(IDC_BTN_CUSTOM, &CSampleDialog2::OnCustomButton)
  ON_BN_CLICKED(IDC_BTN_SELECT, &CSampleDialog2::OnSelectButton)
  ON_BN_CLICKED(IDC_BTN_CANCEL3, &CSampleDialog2::OnCancel3Button)
END_MESSAGE_MAP()

BOOL CSampleDialog2::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  return TRUE;
}

void CSampleDialog2::OnCloseButton()
{
  DestroyWindow();
}

void CSampleDialog2::DisposeDialog()
{
  CRhinoDialog::OnCancel();
}

void CSampleDialog2::PostNcDestroy()
{
  CRhinoDialog::PostNcDestroy();

  if (m_pCommand)
    m_pCommand->DisposeDialog();

  delete this;
}


/////////////////////////////////////////////////////////////////////////////
// CSampleGetCircle class

class CSampleGetCircle : public CRhinoGetObject
{
public:
  CSampleGetCircle() = default;

  bool CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
  ) const override;

  CRhinoGet::result GetCircles(
    int minimum_number = 1,
    int maximum_number = 1
  );
};

bool CSampleGetCircle::CustomGeometryFilter(
  const CRhinoObject* object,
  const ON_Geometry* geometry,
  ON_COMPONENT_INDEX component_index
) const
{
  const ON_ArcCurve* arc_curve = ON_ArcCurve::Cast(geometry);
  if (arc_curve && arc_curve->IsCircle())
    return true;

  const ON_NurbsCurve* nurbs_curve = ON_NurbsCurve::Cast(geometry);
  if (nurbs_curve)
  {
    ON_Arc arc;
    if (nurbs_curve->IsArc(nullptr, &arc) && arc.IsCircle())
      return true;
  }

  return false;
}

CRhinoGet::result CSampleGetCircle::GetCircles(int minimum_number, int maximum_number)
{
  SetCommandPrompt(L"Select circles");

  // Expert user tool that allows GetPoint, GetObject, GetString, etc., 
  // to return if a registered Windows message is posted by the static function
  // CRhinoGet::PostCustomWindowsMessage.
  AcceptCustomWindowsMessage(SampleModelessCancelPlugIn().WindowMessage());

  return GetObjects(minimum_number, maximum_number);
}

// CSampleGetCircle class
/////////////////////////////////////////////////////////////////////////////


void CSampleDialog2::OnGetButton()
{
  // Standard GetObject
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects");
  CRhinoGet::result res = go.GetObjects(1, 0);
  if (res == CRhinoGet::object)
  {
    for (int i = 0; i < go.ObjectCount(); i++)
    {
      const CRhinoObject* object = go.Object(i).Object();
      if (object)
      {
        ON_wString str;
        ON_UuidToString(object->ModelObjectId(), str);
        RhinoApp().Print(L"%s\n", static_cast<const wchar_t*>(str));
      }
    }
  }
  else if (res == CRhinoGet::cancel)
  {
    RhinoApp().Print(L"CRhinoGet::cancel\n");
  }
}

void CSampleDialog2::OnCustomButton()
{
  // Custom GetObject
  CSampleGetCircle go;
  CRhinoGet::result res = go.GetCircles(1, 0);
  if (res == CRhinoGet::object)
  {
    for (int i = 0; i < go.ObjectCount(); i++)
    {
      const CRhinoObject* object = go.Object(i).Object();
      if (object)
      {
        ON_wString str;
        ON_UuidToString(object->ModelObjectId(), str);
        RhinoApp().Print(L"%s\n", static_cast<const wchar_t*>(str));
      }
    }
  }
  // Windows posted a message id that was in the list passed to CRhinoGet::AcceptWindowsMessage().  
  // Call CRhinoGet::WndMsg() to get the message.
  else if (res == CRhinoGet::winmsg)
  {
    RhinoApp().Print(L"CRhinoGet::winmsg\n");
  }
  else if (res == CRhinoGet::cancel)
  {
    RhinoApp().Print(L"CRhinoGet::cancel\n");
  }
}

void CSampleDialog2::OnSelectButton()
{
  // Rhino command that selects objects
  CRhinoDoc* pDoc = RhinoApp().ActiveDoc();
  if (pDoc)
    RhinoApp().RunScript(pDoc->RuntimeSerialNumber(), L"! _Select", 0);
}

void CSampleDialog2::OnCancel1Button()
{
  // Script a bang character
  CRhinoDoc* pDoc = RhinoApp().ActiveDoc();
  if (pDoc)
    RhinoApp().RunScript(pDoc->RuntimeSerialNumber(), L"!", 0);
}

void CSampleDialog2::OnCancel2Button()
{
  // Post an escape key via WM_CHAR
  ::PostMessage(RhinoApp().MainWnd(), WM_CHAR, (WPARAM)VK_ESCAPE, (LPARAM)0);
}

void CSampleDialog2::OnCancel3Button()
{
  // Post a custom Window message to the getter
  CRhinoGet::PostCustomWindowsMessage(SampleModelessCancelPlugIn().WindowMessage(), 0, 0);
}
