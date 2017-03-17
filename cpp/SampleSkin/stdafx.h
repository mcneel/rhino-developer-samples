// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN                             // Exclude rarely-used stuff from Windows headers
#endif

// This Skin DLL is Rhino 6 ready
#define RHINO_V6_READY

// Skin DLLs must use the release version of MFC used by Rhino.
// Skin DLLs that require debugging information need to be built with
// RHINO_DEBUG_PLUGIN defined.
#if defined(RHINO_DEBUG_PLUGIN) && defined(_DEBUG)
//  Rhino 6 Debug Skin DLLs should define RHINO_DEBUG_PLUGIN, 
//  but not define _DEBUG in the .vcxproj file.
#error Do not define _DEBUG - use RHINO_DEBUG_PLUGIN instead
#endif

// Rhino SDK Preamble
#include "C:\Program Files\Rhino 6.0 SDK\Inc\RhinoSdkStdafxPreamble.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS       // some CString constructors will be explicit

#include <afxwin.h>                              // MFC core and standard components
#include <afxext.h>                              // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>                              // MFC OLE classes
#include <afxodlgs.h>                            // MFC OLE dialog classes
#include <afxdisp.h>                             // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                               // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                              // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>                            // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                              // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


// TODO: include additional commonly used header files here


#if defined(_M_X64) && defined(WIN32) && defined(WIN64)
//  The afxwin.h includes afx.h, which includes afxver_.h, 
//  which unconditionally defines WIN32  This is a bug.
//  Note, all Windows builds (32 and 64 bit) define _WIN32.
//  Only 64-bit builds define _WIN64. Never define/undefine
// _WIN32 or _WIN64.  Only define EXACTLY one of WIN32 or WIN64.
//  See the MSDN "Predefined Macros" help file for details.
#undef WIN32
#endif

// Rhino SDK classes
#include "C:\Program Files\Rhino 6.0 SDK\Inc\RhinoSdk.h" 

// Rhino Render Development Kit (RDK) classes
#include "C:\Program Files\Rhino 6.0 SDK\Inc\RhRdkHeaders.h" 

#if defined(RHINO_DEBUG_PLUGIN)
// Now that all the system headers are read, we can
// safely define _DEBUG so the developers can test
// for _DEBUG in their code.
#define _DEBUG
#endif

// Rhino SDK linking pragmas
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInLinkingPragmas.h"
