#include "StdAfx.h"
#include "SampleAnimatorDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleAnimatorConduit implementation

CSampleAnimatorConduit::CSampleAnimatorConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY)
{
  m_xform.Identity();
}

bool CSampleAnimatorConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  switch (nChannel)
  {
  case CSupportChannels::SC_CALCBOUNDINGBOX:
  {
    // If you are dynamically drawing objects, then we must implement the 
    // this channel to add to the overall scene bounding box. This will make
    // Rhino adjust its clipping planes to include our geometry.
    int i;
    for (i = 0; i < m_objects.Count(); i++)
    {
      const CRhinoObject* obj = m_objects[i];
      if (obj)
      {
        ON_BoundingBox bbox = obj->BoundingBox();
        bbox.Transform(m_xform);
        m_pChannelAttrs->m_BoundingBox.Union(bbox);
      }
    }
  }
  break;

  case CSupportChannels::SC_DRAWOVERLAY:
  {
    // This channel is where the drawing takes place.
    int i;
    for (i = 0; i < m_objects.Count(); i++)
    {
      const CRhinoObject* obj = m_objects[i];
      if (obj)
      {
        dp.SetObjectColor(obj->ObjectDrawColor());
        dp.DrawObject(obj, &m_xform);
      }
    }
  }
  break;
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CSampleAnimatorDialog implementation

CSampleAnimatorDialog::CSampleAnimatorDialog(CWnd* pParent, unsigned int document_sn)
  : CRhinoDialog(CSampleAnimatorDialog::IDD, pParent)
  , m_document_sn(document_sn)
{
  SetEnableDisplayCommands(true, document_sn);
  m_max_steps = 100;
}

void CSampleAnimatorDialog::DoDataExchange(CDataExchange* pDX)
{
  CRhinoDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_SLIDER, m_slider);
}

BEGIN_MESSAGE_MAP(CSampleAnimatorDialog, CRhinoDialog)
  ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CSampleAnimatorDialog::OnInitDialog()
{
  EnableSaveDialogPositionAndSize();

  CRhinoDialog::OnInitDialog();

  // Setup slider control
  m_slider.SetRange(0, m_max_steps - 1);
  m_slider.SetTicFreq(10);
  m_slider.SetLineSize(1);
  m_slider.SetPageSize(10);

  // Enable drawing conduit
  m_conduit.Enable(m_document_sn);

  // Draw first position
  ON_3dVector v = m_points[0] - m_start;
  m_conduit.m_xform.Translation(v);
  RedrawDocument();

  return TRUE;
}

void CSampleAnimatorDialog::RedrawDocument()
{
  CRhinoDoc* doc = RhinoApp().ActiveDoc();
  if (doc)
    doc->Redraw(CRhinoView::regenerate_display_hint);
}

void CSampleAnimatorDialog::OnOK()
{
  m_conduit.Disable();
  CRhinoDialog::OnOK();
}

void CSampleAnimatorDialog::OnCancel()
{
  m_conduit.Disable();
  CRhinoDialog::OnCancel();
}

void CSampleAnimatorDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  UNREFERENCED_PARAMETER(nPos);
  UNREFERENCED_PARAMETER(pScrollBar);

  switch (nSBCode)
  {
  case TB_LINEUP:
    break;
  case TB_LINEDOWN:
    break;
  case TB_PAGEUP:
    break;
  case TB_PAGEDOWN:
    break;
  case TB_THUMBPOSITION:
    return;
  case TB_THUMBTRACK:
    break;
  case TB_TOP:
    break;
  case TB_BOTTOM:
    break;
  case TB_ENDTRACK:
    return;
  }

  // Update positions
  int pos = m_slider.GetPos();
  if (pos >= m_slider.GetRangeMin() || pos <= m_slider.GetRangeMax())
  {
    ON_3dVector v = m_points[pos] - m_start;
    m_conduit.m_xform.Translation(v);
    RedrawDocument();
  }
}
