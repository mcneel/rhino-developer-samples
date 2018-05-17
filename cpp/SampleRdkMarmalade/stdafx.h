
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifdef _DEBUG
#undef FULL_DEBUG
#define FULL_DEBUG
#endif

#define RHINO_V6_READY
#define RHINO_SDK_MFC

#pragma warning( disable : 4100 ) // unreferenced formal parameter
#pragma warning( disable : 4189 ) // local variable is initialized but not referenced

// Rhino SDK Preamble
#ifdef WIN64
#undef WIN32
#endif

#include "RhinoSdkStdafxPreamble.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>           // MFC core and standard components
#include <afxext.h>           // MFC extensions
#include <afxmt.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>           // MFC OLE classes
#include <afxodlgs.h>         // MFC OLE dialog classes
#include <afxdisp.h>          // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#include <../src/mfc/afximpl.h> // WIN64 PORTABILITY FIX

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			      // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		      // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			      // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <afxinet.h>
#include <intshcut.h>

#include <string>
#include <list>

#include <initguid.h>
#include <rpc.h>

#include <delayimp.h>

#include <Uxtheme.h>

// Rhino SDK classes

#ifdef WIN64
#undef WIN32
#endif

// Rhino SDK classes
#include "RhinoSdk.h" 

// Rhino Render Development Kit (RDK) classes
#include "RhRdkHeaders.h" 

#if defined( RHINO_DEBUG_PLUGIN )
// Now that all the system headers are read, we can
// safely define _DEBUG so the developers can test
// for _DEBUG in their code.
#define _DEBUG
#endif

// Rhino SDK linking pragmas
#include "rhinoSdkPlugInLinkingPragmas.h"
