/////////////////////////////////////////////////////////////////////////////
// SampleCirclePreviewDialog.cpp

#include "stdafx.h"
#include "SampleDialogsPlugIn.h"
#include "SampleCirclePreviewDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleCirclePreviewConduit

CSampleCirclePreviewConduit::CSampleCirclePreviewConduit()
: CRhinoDisplayConduit( CSupportChannels::SC_PREDRAWOBJECTS )
{
}

bool CSampleCirclePreviewConduit::ExecConduit( 
        CRhinoDisplayPipeline& dp, 
        UINT nChannel, 
        bool& bTerminate 
        )
{
  CRhinoViewport& vp = *m_pVP;
  if( m_circle.IsValid() )
  {
    ON_Color saved_color = vp.SetDrawColor( RhinoApp().AppSettings().ActiveLayerColor() );
    vp.DrawCircle( m_circle );
		vp.SetDrawColor( saved_color );
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CSampleCirclePreviewDialog

IMPLEMENT_DYNAMIC( CSampleCirclePreviewDialog, CRhinoDialog )

CSampleCirclePreviewDialog::CSampleCirclePreviewDialog(CWnd* pParent /*=NULL*/)
: CRhinoDialog(CSampleCirclePreviewDialog::IDD, pParent)
{
  // Allows dialog to lose focus
  SetEnableDisplayCommands( true );
  m_center_x = 0.0;
  m_center_y = 0.0;
  m_center_z = 0.0;
  m_radius = 1.0;
}

CSampleCirclePreviewDialog::~CSampleCirclePreviewDialog()
{
}

void CSampleCirclePreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CRhinoDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_SEP1, m_sep1);
  DDX_Control(pDX, IDC_SEP2, m_sep2);
  DDX_Control(pDX, IDC_CENTER_X, m_edit_center_x);
  DDX_Control(pDX, IDC_CENTER_Y, m_edit_center_y);
  DDX_Control(pDX, IDC_CENTER_Z, m_edit_center_z);
  DDX_Control(pDX, IDC_RADIUS, m_edit_radius);
  m_edit_center_x.DDX_Text( pDX, IDC_CENTER_X, m_center_x );
  m_edit_center_y.DDX_Text( pDX, IDC_CENTER_Y, m_center_y );
  m_edit_center_z.DDX_Text( pDX, IDC_CENTER_Z, m_center_z );
  m_edit_radius.DDX_Text( pDX, IDC_RADIUS, m_radius );
}

BEGIN_MESSAGE_MAP(CSampleCirclePreviewDialog, CRhinoDialog)
  ON_BN_CLICKED(IDC_PREVIEW, &CSampleCirclePreviewDialog::OnBnClickedPreview)
	ON_BN_CLICKED(IDCANCEL, &CSampleCirclePreviewDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CSampleCirclePreviewDialog::OnInitDialog()
{
  CRhinoDialog::OnInitDialog();

  return TRUE;
}

void CSampleCirclePreviewDialog::OnBnClickedPreview()
{
  UpdateData( TRUE );

  ON_3dPoint point( m_center_x, m_center_y, m_center_z );

  ON_Plane plane = ON_xy_plane;
  plane.SetOrigin( point );

  ON_Circle circle( plane, m_radius );
  if( circle.IsValid() )
  {
    m_preview.m_circle = circle;

    if( !m_preview.IsEnabled() )
      m_preview.Enable();

    CRhinoDoc* doc = RhinoApp().ActiveDoc();
    if( doc )
      doc->Regen();
  }
}

void CSampleCirclePreviewDialog::OnBnClickedCancel()
{
	m_preview.Disable();
  CRhinoDoc* doc = RhinoApp().ActiveDoc();
  if( doc )
    doc->Regen();

	OnCancel();
}
