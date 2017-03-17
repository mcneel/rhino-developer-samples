/*
 * (C) 1998-2001 3Dconnexion. All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and 3Dconnexion will support such modifications only is said modifications are
 * approved by 3Dconnexion.
 *
 */
#ifndef _SIAPP_H
#define _SIAPP_H


enum InitResult {
   NOT_LOADED, 
   FAILED, 
   LOADED
};

enum ErrorCode {
   NO_DLL_ERROR=0,
   DLL_LOAD_FAILURE,
   DLL_FUNCTION_LOAD_FAILURE,
   DLL_VAR_LOAD_FAILURE
};


    // protoypes for public functions
sint32_t SiInitialize(void);
void SiTerminate(void);
sint32_t SiGetNumDevices (void);
SiDevID SiGetDeviceID (SiHdl hdl);
SiDevID SiDeviceIndex (sint32_t idx);
sint32_t SiDispatch (SiHdl hdl, SiGetEventData *pData, SiSpwEvent *pEvent, SiSpwHandlers *pDHandlers);
void_t SiOpenWinInit (SiOpenData *pData, HWND hWnd);
SiHdl SiOpen (char *pAppName, SiDevID devID, SiTypeMask *pTMask, sint32_t mode, SiOpenData *pData);
sint32_t SiClose (SiHdl hdl);
boolean_t SiIsSpaceWareEvent(SiGetEventData *pData, SiHdl *pHdl);
void_t SiGetEventWinInit (SiGetEventData *pData, UINT msg, WPARAM wParam, LPARAM lParam);
sint32_t SiGetEvent (SiHdl hdl, sint32_t flags, SiGetEventData *pData, SiSpwEvent *pEvent);
sint32_t SiBeep(SiHdl hdl, char *pString);
sint32_t SiRezero (SiHdl hdl);
sint32_t SiGrabDevice (SiHdl hdl, SPWbool exclusive);
sint32_t SiReleaseDevice (SiHdl hdl);
sint32_t SiButtonPressed (SiSpwEvent *pEvent);
sint32_t SiButtonReleased (SiSpwEvent *pEvent);
sint32_t SiSetUiMode (SiHdl hdl, SPWuint32 mode);
sint32_t SiSetTypeMask (SiTypeMask *pTMask, sint32_t type1, ...);
sint32_t SiGetDevicePort (SiDevID devID, SiDevPort *pPort);
sint32_t SiGetDriverInfo (SiVerInfo *pInfo);
void_t SiGetLibraryInfo (SiVerInfo *pInfo);
sint32_t SiGetDeviceInfo (SiHdl hdl, SiDevInfo *pInfo);
char_t *SpwErrorString (sint32_t val);

#endif