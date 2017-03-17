
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifdef _DEBUG
#undef FULL_DEBUG
#define FULL_DEBUG
#endif

#ifdef WIN64
#pragma message ("WIN64\n")
#endif

#pragma warning( disable : 4100 ) // unreferenced formal parameter
#pragma warning( disable : 4189 ) // local variable is initialized but not referenced

// Rhino SDK Preamble
#ifdef WIN64
#undef WIN32
#endif

#include "C:\Program Files (x86)\Rhino 5.0 x64 SDK\Inc\RhinoSdkStdafxPreamble.h"

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

#pragma message("Rhino 5.0 x64 SDK")
#include "C:\Program Files (x86)\Rhino 5.0 x64 SDK\Inc\RhinoSdk.h"
#include "C:\Program Files (x86)\Rhino 5.0 x64 SDK\Inc\RhRdkHeaders.h"

#ifdef WIN64
	#ifdef FULL_DEBUG
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib64\\rhino5x64_d.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib64\\opennurbsx64_d.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib64\\rdk_d.lib")
	#else
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib64\\rhino.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib64\\opennurbsx64.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib64\\rdk.lib")
	#endif
#else
	#ifdef FULL_DEBUG
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib32\\rhino4_d.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib32\\opennurbs_d.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib32\\rdk_d.lib")
	#else
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib32\\rhino4.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib32\\opennurbs.lib")
		#pragma comment(lib, "C:\\Program Files (x86)\\Rhino 5.0 x64 SDK\\lib32\\rdk.lib")
	#endif
#endif

#if defined( RHINO_DEBUG_PLUGIN )
// Now that all the system headers are read, we can
// safely define _DEBUG so the developers can test
// for _DEBUG in their code.
#define _DEBUG
#endif
