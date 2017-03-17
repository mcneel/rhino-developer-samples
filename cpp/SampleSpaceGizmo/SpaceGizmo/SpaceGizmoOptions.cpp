/////////////////////////////////////////////////////////////////////////////
// SpaceGizmoOptions.cpp

#include "stdafx.h"
#include "SpaceGizmoOptions.h"
#include "SpaceGizmoPlugIn.h"

IMPLEMENT_DYNAMIC( CSpaceGizmoOptions, CRhinoOptionsDialogPage )

CSpaceGizmoOptions::CSpaceGizmoOptions( CWnd* pParent )
	: CRhinoOptionsDialogPage(CSpaceGizmoOptions::IDD, pParent)
{
}

CSpaceGizmoOptions::~CSpaceGizmoOptions()
{
}

void CSpaceGizmoOptions::DoDataExchange( CDataExchange* pDX )
{
	CRhinoOptionsDialogPage::DoDataExchange( pDX );
  DDX_Control( pDX, IDC_SEP1, m_sep1 );
	DDX_Check( pDX, IDC_CHK_ROTATE_ABOUT_CAMERA, m_settings.m_rotate_about_camera );
	DDX_Check( pDX, IDC_CHK_DOLLY_ZOOM, m_settings.m_swap_zoom_with_dolly );
	DDX_Check( pDX, IDC_CHK_ALLOW_TILT, m_settings.m_allow_tilt );
	DDX_Check( pDX, IDC_CHK_ROTATE_PLAN_VIEWS, m_settings.m_rotate_parallel_view );
	DDX_Check( pDX, IDC_CHK_SWAP_PAN_AND_ZOOM, m_settings.m_swap_pan_and_zoom );
	DDX_Control( pDX, IDC_ED_ROTATE_SCALE, m_RotationScale );
	DDX_Control( pDX, IDC_ED_TRANS_SCALE, m_TranslationScale );
	m_RotationScale.DDX_Text( pDX, IDC_ED_ROTATE_SCALE, m_settings.m_rotation_scale );
	m_TranslationScale.DDX_Text( pDX, IDC_ED_TRANS_SCALE, m_settings.m_translation_scale );
}

BEGIN_MESSAGE_MAP( CSpaceGizmoOptions, CRhinoOptionsDialogPage )
END_MESSAGE_MAP()

// CSpaceGizmoOptions message handlers
BOOL CSpaceGizmoOptions::OnInitDialog() 
{
  m_settings = SpaceGizmoPlugIn().SpaceGizmoSettings();

  CRhinoOptionsDialogPage::OnInitDialog();
	
  m_RotationScale.SetMin( true, -1000000);
  m_TranslationScale.SetMin( true, -1000000);
	
	return TRUE;
}

void CSpaceGizmoOptions::PostNcDestroy() 
{
	CRhinoOptionsDialogPage::PostNcDestroy();
}

BOOL CSpaceGizmoOptions::OnHelpInfo( HELPINFO* pHelpInfo ) 
{
  // TODO...
  return TRUE;
}

const wchar_t* CSpaceGizmoOptions::EnglishPageTitle()
{
  return L"SpaceGizmo";
}

const wchar_t* CSpaceGizmoOptions::LocalPageTitle()
{
  return L"SpaceGizmo";
}

BOOL CSpaceGizmoOptions::OnApply() 
{
  if( !UpdateData(TRUE) )
    return FALSE;

	SpaceGizmoPlugIn().SetSpaceGizmoSettings( m_settings );

  return TRUE;
}

void CSpaceGizmoOptions::OnCancel()
{
}

void CSpaceGizmoOptions::RhinoDeleteThisPage()
{
  delete this;
}

CRhinoCommand::result CSpaceGizmoOptions::RunScript( CRhinoDoc& rhino_doc)
{
  m_settings = SpaceGizmoPlugIn().SpaceGizmoSettings();

  CRhinoCommandOptionValue no = RHCMDOPTVALUE( L"No" );
  CRhinoCommandOptionValue yes = RHCMDOPTVALUE( L"Yes" );

  CRhinoGetOption go;
  go.SetCommandPrompt( L"Choose Space Gizmo option" );
  go.AcceptNothing();
  go.AddCommandOptionInteger( RHCMDOPTNAME(L"RotationScale"), &m_settings.m_rotation_scale, L"Rotation scale", -1000000 );
  go.AddCommandOptionInteger( RHCMDOPTNAME(L"TranslationScale"), &m_settings.m_translation_scale, L"Translation scale", -1000000 );
  go.AddCommandOptionToggle( RHCMDOPTNAME(L"RotatePlanViews"), no, yes, m_settings.m_rotate_parallel_view, &m_settings.m_rotate_parallel_view );
  go.AddCommandOptionToggle( RHCMDOPTNAME(L"SwapPanAndZoom"), no, yes, m_settings.m_swap_pan_and_zoom, &m_settings.m_swap_pan_and_zoom );
  go.AddCommandOptionToggle( RHCMDOPTNAME(L"AllowTilt"), no, yes, m_settings.m_allow_tilt, &m_settings.m_allow_tilt );
  go.AddCommandOptionToggle( RHCMDOPTNAME(L"DollyZoom"), no, yes, m_settings.m_swap_zoom_with_dolly, &m_settings.m_swap_zoom_with_dolly);
  go.AddCommandOptionToggle( RHCMDOPTNAME(L"RotateAboutCamera"), no, yes, m_settings.m_rotate_about_camera, &m_settings.m_rotate_about_camera );

  while( true )
  {
    go.GetOption();

    switch( go.Result() )
    {
    case CRhinoGet::option:
      continue;
    case CRhinoGet::nothing:
      return CRhinoCommand::success;
    case CRhinoGet::exit_rhino:
      return CRhinoCommand::exit_rhino;
    case CRhinoGet::cancel:
    default:
      return CRhinoCommand::cancel;
    }
  }

	SpaceGizmoPlugIn().SetSpaceGizmoSettings( m_settings );

  return CRhinoCommand::success;
}
