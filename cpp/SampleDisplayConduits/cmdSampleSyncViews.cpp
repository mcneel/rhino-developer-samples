/////////////////////////////////////////////////////////////////////////////
// cmdSampleSyncViews.cpp : command file
//

#include "stdafx.h"
#include "SampleDisplayConduitsPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSyncViews command
//

static void SyncVP( CRhinoView* from, CRhinoView* to )
{
  if( from && to )
  {
    const ON_3dmView& v0 = from->Viewport().View();
    const ON_Viewport& vp0 = from->Viewport().VP();

    ON_3dmView& v1 = to->Viewport().m_v;
    ON_Viewport& vp1 = to->Viewport().m_v.m_vp;

    vp1.SetProjection( vp0.Projection() );
    vp1.SetCameraLocation( vp0.CameraLocation() );
    vp1.SetCameraDirection( vp0.CameraDirection() );
    vp1.SetCameraUp( vp0.CameraUp() );
    v1.SetTargetPoint( v0.TargetPoint() );

    double fl, fr, ft, fb, fn, ff, fa;
    vp1.GetFrustumAspect( fa );
    vp0.GetFrustum( &fl, &fr, &fb, &ft, &fn, &ff );
    vp1.SetFrustum( fl, fr, fb, ft, fn, ff );
    vp1.SetFrustumAspect( fa );

    to->Viewport().SetTarget( from->Viewport().Target() );
  }
}

class CSampleSyncViewsConduit: public CRhinoDisplayConduit
{
public:
  CSampleSyncViewsConduit();
              
  bool ExecConduit(
    CRhinoDisplayPipeline&, // pipeline executing this conduit
    UINT,                   // current channel within the pipeline
    bool&                   // channel termination flag
    );  

  void NotifyConduit(
    EConduitNotifiers,      // event reported by the display pipeline
    CRhinoDisplayPipeline&  // display pipeline calling this conduit
    );
  
public:
  CRhinoView* m_pView1;
  CRhinoView* m_pView2;
  HWND m_hWnd1;
  HWND m_hWnd2;
  bool m_bDirty1;
  bool m_bDirty2;
};

CSampleSyncViewsConduit::CSampleSyncViewsConduit()
: CRhinoDisplayConduit( CSupportChannels::SC_INITFRAMEBUFFER )
{
  m_pView1 = 0;
  m_pView2 = 0;
  m_hWnd1 = 0;
  m_hWnd2 = 0;
  m_bDirty1 = false;
  m_bDirty2 = false;
}

void CSampleSyncViewsConduit::NotifyConduit( EConduitNotifiers Notify, CRhinoDisplayPipeline& dp )
{
  if( (m_pView1 && m_pView1->DisplayPipeline() == 0 ) ||
      (m_pView2 && m_pView2->DisplayPipeline() == 0 ) )
  {
    m_pView1 = 0;
    m_pView2 = 0;
    Disable();
    return;
  }

  switch( Notify )
  {
    case CN_PROJECTIONCHANGED:
    {
      CRhinoView* pActiveView = ::RhinoApp().ActiveView();
      if( pActiveView )
      {
        if( (pActiveView == m_pView1) && (m_pView == m_pView1) )
        {
          m_hWnd1 = m_pView1->m_hWnd;
          SyncVP( m_pView1, m_pView2 );
          m_bDirty1 = true;
          m_bDirty2 = false;
        }
        else if( (pActiveView == m_pView2) && (m_pView == m_pView2) )
        {
          m_hWnd2 = m_pView2->m_hWnd;
          SyncVP( m_pView2, m_pView1 );
          m_bDirty2 = true;
          m_bDirty1 = false;
        }
      }
      break;
    }
    
    case CN_PIPELINECLOSED:
    {
      if( m_bDirty1 && !m_bDirty2 )
      {
        m_bDirty1 = false;
        CClientDC dc( m_pView2 );
        if( m_pView2->DisplayPipeline()->DrawFrameBuffer(m_pView2->DisplayAttributes()) )
          m_pView2->DisplayPipeline()->ShowFrameBuffer( &dc );
      }
      else if( !m_bDirty1 && m_bDirty2 )
      {
        m_bDirty2 = false;
        CClientDC dc( m_pView1 );
        if( m_pView1->DisplayPipeline()->DrawFrameBuffer(m_pView1->DisplayAttributes()) )
          m_pView1->DisplayPipeline()->ShowFrameBuffer( &dc );
      }
      else
      {
        m_bDirty1 = m_bDirty2 = false;
      }
      break;
    }
  }  
}

bool CSampleSyncViewsConduit::ExecConduit( CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate )
{
  switch( nChannel )
  {
    case CSupportChannels::SC_INITFRAMEBUFFER:
      break;
  }
  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleSyncViews : public CRhinoCommand
{
public:
	CCommandSampleSyncViews() {}
	~CCommandSampleSyncViews() {}
	UUID CommandUUID()
	{
		// {84350CBB-24F2-40F6-95B0-C0656F90BCE8}
		static const GUID SampleSyncViewsCommand_UUID =
		{ 0x84350CBB, 0x24F2, 0x40F6, { 0x95, 0xB0, 0xC0, 0x65, 0x6F, 0x90, 0xBC, 0xE8 } };
		return SampleSyncViewsCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleSyncViews"; }
	const wchar_t* LocalCommandName() { return L"SampleSyncViews"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

public:
  CSampleSyncViewsConduit m_conduit;
};

// The one and only CCommandSampleSyncViews object
static class CCommandSampleSyncViews theSampleSyncViewsCommand;

CRhinoCommand::result CCommandSampleSyncViews::RunCommand( const CRhinoCommandContext& context )
{
  if( m_conduit.IsEnabled() 
      && ::IsWindow( m_conduit.m_hWnd1 )
      && ::IsWindow( m_conduit.m_hWnd2 ) )
  {
    m_conduit.m_pView1 = 0;
    m_conduit.m_pView2 = 0;
    m_conduit.Disable();
  }
  else
  {
    CRhinoGetView gv;
    gv.SetCommandPrompt( L"Select primary view" );
    gv.GetView();
    if( gv.CommandResult() != success )
      return gv.CommandResult();

    CRhinoView* pPrimaryView = gv.View();
    
    gv.SetCommandPrompt( L"Select secondary view" );
    gv.GetView();
    if( gv.CommandResult() != success )
      return gv.CommandResult();

    CRhinoView* pSecondaryView = gv.View();
    
    if( pPrimaryView == pSecondaryView )
    {
      RhinoApp().Print( L"\nPrimary and secondary views can not be the same view.\n" );
      return failure;
    }

    m_conduit.m_pView1 = pPrimaryView;
    m_conduit.m_pView2 = pSecondaryView;
    m_conduit.m_hWnd1 = m_conduit.m_pView1->m_hWnd;
    m_conduit.m_hWnd2 = m_conduit.m_pView2->m_hWnd;
    
    SyncVP( pPrimaryView, pSecondaryView );
    
    m_conduit.Bind( *pPrimaryView );
    m_conduit.Bind( *pSecondaryView );
    
    pPrimaryView->Redraw();
    pSecondaryView->Redraw();
    
    m_conduit.Enable();
  }
    
	return CRhinoCommand::success;
}

//
// END SampleSyncViews command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
