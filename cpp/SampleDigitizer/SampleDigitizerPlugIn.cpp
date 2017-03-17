/////////////////////////////////////////////////////////////////////////////
// SampleDigitizerPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleDigitizerPlugIn.h"

// The plug-in object must be constructed before any plug-in classes
// derived from CRhinoCommand. The #pragma init_seg(lib) ensures that
// this happens.

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"SampleDigitizer" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"B7A2F8E2-5C8E-44C3-8A20-34668613BCF7" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__"  "__TIME__ )

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION( L"Robert McNeel & Associates" );
RHINO_PLUG_IN_DEVELOPER_ADDRESS( L"3670 Woodland Park Avenue North\015\012Seattle WA 98103" );
RHINO_PLUG_IN_DEVELOPER_COUNTRY( L"United States" );
RHINO_PLUG_IN_DEVELOPER_PHONE( L"206-545-6877" );
RHINO_PLUG_IN_DEVELOPER_FAX( L"206-545-7321" );
RHINO_PLUG_IN_DEVELOPER_EMAIL( L"tech@mcneel.com" );
RHINO_PLUG_IN_DEVELOPER_WEBSITE( L"http://www.rhino3d.com" );
RHINO_PLUG_IN_UPDATE_URL( L"https://github.com/mcneel/" );

// The one and only CSampleDigitizerPlugIn object
static CSampleDigitizerPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleDigitizerPlugIn definition

CSampleDigitizerPlugIn& SampleDigitizerPlugIn()
{ 
  // Return a reference to the one and only CSampleDigitizerPlugIn object
  return thePlugIn; 
}

CSampleDigitizerPlugIn::CSampleDigitizerPlugIn()
{
  // Description:
  //   CSampleDigitizerPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleDigitizerPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleDigitizerPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();

	m_hThread = 0;
	m_dwThreadID = 0;
	m_bDigitizerConnected = false;
}

CSampleDigitizerPlugIn::~CSampleDigitizerPlugIn()
{
  // Description:
  //   CSampleDigitizerPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleDigitizerPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleDigitizerPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleDigitizerPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleDigitizerPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {B7A2F8E2-5C8E-44C3-8A20-34668613BCF7}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleDigitizerPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // NOTE: DO NOT enable your digitizer here!

  // TODO: Add plug-in initialization code here.
  return CRhinoDigitizerPlugIn::OnLoadPlugIn();
}

void CSampleDigitizerPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.
  EnableDigitizer( false );

  CRhinoDigitizerPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Digitizer overrides

static DWORD WINAPI ThreadProc( LPVOID pVoid )
{
  CSampleDigitizerPlugIn* pPlugIn = (CSampleDigitizerPlugIn*)pVoid;
  ASSERT( pPlugIn );

  ON_3dPoint pt;
  while( 1 )
  {
    /*
    Do whatever you need to do to query the digitizer here
    so you can call CRhinoPlugIn::SendPoint().

    DWORD dwResult = WaitForSingleObject( ... );
    if( WAIT_OBJECT_0 == dwResult )
    {
      ON_3dPoint = ?
      UINT uFlags = ?
      pPlugIn->SendPoint( pt, nFlags );
    }
    */
  }
  
  // Should never get here
  return 0;
}

bool CSampleDigitizerPlugIn::EnableDigitizer( bool bEnable )
{
  // Description:
  //   Called by Rhino to enable/disable input from the digitizer.
  //   If bEnable is true and EnableDigitizer() returns false,
  //   then Rhino will not calibrate the digitizer.

  // In case we need to access our resources...
	AFX_MANAGE_STATE( AfxGetStaticModuleState() ); 

  // In case we need Rhino main window handle...
  HWND hwndRhino = RhinoApp().MainWnd();

	bool rc = false;

	if( bEnable )
	{
    for( ;; )
    {
      if( m_bDigitizerConnected ) 
      {
        ::RhinoApp().Print( L"Digitizer already connected.\n" );
        rc = true;
        break;
      }

      ::RhinoApp().Print( L"Connecting to digitizer ...\n" );

      /*
      Do whatever it takes to connnect to and initialize
      your digitizer here. If you need to call LoadLibrary()
      to load some communication DLL, do it here. Once you 
      have connected and initialized your digitizer, you can
      create the polling thread.
      */

      // Start up the polling thread
      m_hThread = ::CreateThread( NULL, 0, ThreadProc, this, 0, &m_dwThreadID );
      m_bDigitizerConnected = true;
      rc = true;
      break;
    }
  }
  else 
  {
    if( !m_bDigitizerConnected )
      return true;
  
    if( m_hThread ) 
    {
      ::TerminateThread( m_hThread, 0 );
      ::CloseHandle( m_hThread );
      m_hThread = 0;
    }
  
  	/*
    Shut down communications with the digitizer here. If 
    you need to call FreeLibrary to release a communications
    DLL, do it here.
    */

    m_dwThreadID = 0;
    m_bDigitizerConnected = false;
    rc = true;
  }

  return rc;
}

ON::unit_system CSampleDigitizerPlugIn::UnitSystem() const
{
  // TODO: Return the digitizer's unit system.
  return ON::millimeters;
}

double CSampleDigitizerPlugIn::PointTolerance() const
{
  // Description:
  //   Precision of the digitizer in the unit system
  //   returned by CSampleDigitizerPlugIn::UnitSystem().
  //   If this number is too small, digitizer noise will
  //   cause GetPoint() to jitter.

  // TODO: Return the digitizer's point tolerance.
  return 0.01;
}


