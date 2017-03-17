/*-----------------------------------------------------------------------------
 *
 * siapp.h -- Si static library interface header file
 *
 * $Id: siapp.h,v 1.3 2001/01/16 01:18:49 HJin Exp $
 *
 * Contains function headers and type definitions for siapp.c.
 *
 *-----------------------------------------------------------------------------
 *
 * (C) 1998 Spacetec IMC Corporation (Spacetec). All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and Spacetec will support such modifications only is said modifications are
 * approved by Spacetec.
 *
 */

#ifndef SIAPP_H
#define SIAPP_H


static char SiAppCvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: siapp.h,v 1.3 2001/01/16 01:18:49 HJin Exp $";


/* some enumerated types used in siapp.c */

enum InitResult 
   {
   NOT_LOADED, 
   FAILED, 
   LOADED
   };

enum ErrorCode 
   {
   NO_DLL_ERROR=0,
   DLL_LOAD_FAILURE,
   DLL_FUNCTION_LOAD_FAILURE,
   DLL_VAR_LOAD_FAILURE
   };

enum SpwRetVal *pDllSpwRetVal;

/* interface specifications define parameter passing to DLL functions */

typedef enum SpwRetVal (WINAPI *PFNSI_INIT) (void);
typedef void (WINAPI *PFNSI_TERMINATE) (void);
typedef void (WINAPI *PFNSI_OPENWININIT) (SiOpenData *pData, HWND hWnd);
typedef int (WINAPI *PFNSI_GETNUMDEVICES) (void);
typedef SiDevID (WINAPI *PFNSI_GETDEVICEID) (SiHdl hdl);
typedef int (WINAPI *PFNSI_DISPATCH) (SiHdl hdl, SiGetEventData *pData,
                SiSpwEvent *pEvent, SiSpwHandlers *pDHandlers);
typedef SiHdl (WINAPI *PFNSI_OPEN) (char *pAppName, SiDevID devID, SiTypeMask *pTMask, int mode, 
              SiOpenData *pData);
typedef enum SpwRetVal (WINAPI *PFNSI_CLOSE) (SiHdl hdl);
typedef SPWbool (WINAPI *PFNSI_ISSPACEWAREEVENT) (SiGetEventData *pData, SiHdl *pHdl);
typedef void (WINAPI *PFNSI_GETEVENTWININIT) (SiGetEventData *pData, UINT msg, WPARAM wParam, LPARAM lParam);
typedef enum SpwRetVal (WINAPI *PFNSI_GETEVENT) (SiHdl hdl, int flags, SiGetEventData *pData, 
                           SiSpwEvent *pEvent);
typedef enum SpwRetVal (WINAPI *PFNSI_BEEP) (SiHdl hdl, char *pString);
typedef enum SpwRetVal (WINAPI *PFNSI_REZERO) (SiHdl hdl);
typedef enum SpwRetVal (WINAPI *PFNSI_GRABDEVICE) (SiHdl hdl, SPWbool exclusive);
typedef enum SpwRetVal (WINAPI *PFNSI_RELEASEDEVICE) (SiHdl hdl);
typedef int (WINAPI *PFNSI_BUTTONPRESSED) (SiSpwEvent *pEvent);
typedef int (WINAPI *PFNSI_BUTTONRELEASED) (SiSpwEvent *pEvent);
typedef enum SpwRetVal (WINAPI *PFNSI_SETUIMODE) (SiHdl hdl, SPWuint32 mode);
typedef enum SpwRetVal (WINAPI *PFNSI_SETTYPEMASK) (SiTypeMask *pTMask, int type1, ...);
typedef enum SpwRetVal (WINAPI *PFNSI_GETDEVICEPORT) (SiDevID devID, SiDevPort *pPort);
typedef enum SpwRetVal (WINAPI *PFNSI_GETDRIVERINFO) (SiVerInfo *pInfo);
typedef void (WINAPI *PFNSI_GETLIBRARYINFO) (SiVerInfo *pInfo);
typedef enum SpwRetVal (WINAPI *PFNSI_GETDEVICEINFO) (SiHdl hdl, SiDevInfo *pInfo);
typedef (WINAPI *PFNSPW_ERRORSTRING) (enum SpwRetVal val);
typedef SiDevID (WINAPI *PFNSI_DEVICEINDEX) (int idx);

/* function pointers to functions in DLL */

PFNSI_INIT              pfnSiInit;
PFNSI_TERMINATE         pfnSiTerminate;
PFNSI_OPENWININIT       pfnSiOpenWinInit;
PFNSI_GETNUMDEVICES     pfnSiGetNumDevices;
PFNSI_GETDEVICEID       pfnSiGetDeviceID;
PFNSI_DISPATCH          pfnSiDispatch;
PFNSI_OPEN              pfnSiOpen;
PFNSI_CLOSE             pfnSiClose;
PFNSI_DEVICEINDEX       pfnSiDeviceIndex;
PFNSI_GETEVENTWININIT   pfnSiGetEventWinInit;
PFNSI_GETEVENT          pfnSiGetEvent;
PFNSI_ISSPACEWAREEVENT  pfnSiIsSpaceWareEvent;
PFNSI_BEEP              pfnSiBeep;
PFNSI_REZERO            pfnSiRezero;
PFNSI_GRABDEVICE        pfnSiGrabDevice;
PFNSI_RELEASEDEVICE     pfnSiReleaseDevice;
PFNSI_BUTTONPRESSED     pfnSiButtonPressed;
PFNSI_BUTTONRELEASED    pfnSiButtonReleased;
PFNSI_SETUIMODE         pfnSiSetUiMode;
PFNSI_SETTYPEMASK       pfnSiSetypeMask;
PFNSI_GETDEVICEPORT     pfnSiGetDevicePort;
PFNSI_GETDRIVERINFO     pfnSiGetDriverInfo;
PFNSI_GETLIBRARYINFO    pfnSiGetLibraryInfo;
PFNSI_GETDEVICEINFO     pfnSiGetDeviceInfo;
PFNSPW_ERRORSTRING      pfnSpwErrorString;

/* externally used functions */

enum SpwRetVal SiInitialize(void);
void           SiTerminate(void);
int            SiGetNumDevices (void);
SiDevID        SiDeviceIndex (int idx);
int            SiDispatch (SiHdl hdl, SiGetEventData *pData,
                           SiSpwEvent *pEvent, SiSpwHandlers *pDHandlers);
void           SiOpenWinInit (SiOpenData *pData, HWND hWnd);
SiHdl          SiOpen (char *pAppName, SiDevID devID, SiTypeMask *pTMask, int mode, 
                       SiOpenData *pData);
enum SpwRetVal SiClose (SiHdl hdl);
void           SiGetEventWinInit (SiGetEventData *pData,
                                  UINT msg, WPARAM wParam, LPARAM lParam);
enum SpwRetVal SiGetEvent (SiHdl hdl, int flags, SiGetEventData *pData, 
                           SiSpwEvent *pEvent);
enum SpwRetVal SiBeep (SiHdl hdl, char *string);
enum SpwRetVal SiRezero (SiHdl hdl);
enum SpwRetVal SiGrabDevice (SiHdl hdl, SPWbool exclusive);
enum SpwRetVal SiReleaseDevice (SiHdl hdl);
int            SiButtonPressed (SiSpwEvent *pEvent);
int            SiButtonReleased (SiSpwEvent *pEvent);
enum SpwRetVal SiSetUiMode (SiHdl hdl, SPWuint32 mode);
enum SpwRetVal SiSetTypeMask (SiTypeMask *pTMask, int type1, ...);
enum           SpwRetVal SiGetDevicePort (SiDevID devID, SiDevPort *pPort);
enum           SpwRetVal SiGetDriverInfo (SiVerInfo *pInfo);
void           SiGetLibraryInfo (SiVerInfo *pInfo);
enum           SpwRetVal SiGetDeviceInfo (SiHdl hdl, SiDevInfo *pInfo);
char * SpwErrorString (enum SpwRetVal val);

#endif /* #ifndef SIAPP_H */