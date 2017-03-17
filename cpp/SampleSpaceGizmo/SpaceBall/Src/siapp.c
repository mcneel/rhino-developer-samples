/*-----------------------------------------------------------------------------
 *
 * siapp.c -- Si static library interface
 *
 * $Id: siapp.c,v 1.3 2001/01/16 01:18:49 HJin Exp $
 *
 * The module contains interface routines to the Si library routines contained 
 * in the associated Dynamic Link Library.  The DLL is loaded explicitly when 
 * Si is initialized.  When the DLL is loaded, the initialization routine finds
 * the addresses of the routines that it exposes to the world.  Once this is
 * done the library routines are used as if they were part of the original 
 * source code.  
 *
 *
 * (C) 1998 Spacetec IMC Corporation (Spacetec). All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and Spacetec will support such modifications only is said modifications are
 * approved by Spacetec.
 *
 */


static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: siapp.c,v 1.3 2001/01/16 01:18:49 HJin Exp $";


#define SPW_DEFINE_COPYRIGHT
#define SPW_DEFINE_GLOBALS

#include <windows.h>
#include "../inc/spwmacro.h"
#include "../inc/spwdata.h"
#include "../inc/si.h"
#include "../inc/spwerror.h"
#include "siapp.h"
#include "../inc/version.h"

/*
 *  Static version strings for executable identification.
 */
static char SpwLibVersion[] = SPW_LIB_VERSION;
static char SiiLibVersion[] = SII_LIB_VERSION;
static char SiiLibDate[] = SII_LIB_DATE;

/* DLL library name */

static LPCTSTR DllLibrary = _T("siappdll");


/* names of DLL variables used in DLL */

static LPCSTR strDLLRetVal          = "SpwErrorVal";

/* Names of functions contained in DLL; used to find their addresses at load 
   time */

static LPCSTR fnSiBeep              = "SiBeep";
static LPCSTR fnSiInitialize        = "SiInitialize";
static LPCSTR fnSiTerminate         = "SiTerminate";
static LPCSTR fnSiGetDeviceID       = "SiGetDeviceID";
static LPCSTR fnSiGetNumDevices     = "SiGetNumDevices";
static LPCSTR fnSiDeviceIndex       = "SiDeviceIndex";
static LPCSTR fnSiDispatch          = "SiDispatch";
static LPCSTR fnSiIsSpaceWareEvent  = "SiIsSpaceWareEvent";
static LPCSTR fnSiOpenWinInit       = "SiOpenWinInit";
static LPCSTR fnSiOpen              = "SiOpen";
static LPCSTR fnSiClose             = "SiClose";
static LPCSTR fnSiGetEventWinInit   = "SiGetEventWinInit";
static LPCSTR fnSiGetEvent          = "SiGetEvent";
static LPCSTR fnSiRezero            = "SiRezero";
static LPCSTR fnSiGrabDevice        = "SiGrabDevice";
static LPCSTR fnSiReleaseDevice     = "SiReleaseDevice";
static LPCSTR fnSiButtonPressed     = "SiButtonPressed";
static LPCSTR fnSiButtonReleased    = "SiButtonReleased";
static LPCSTR fnSiSetUIMode         = "SiSetUiMode";
static LPCSTR fnSiGetDevicePort     = "SiGetDevicePort";
static LPCSTR fnSiGetDriverInfo     = "SiGetDriverInfo";
static LPCSTR fnSiGetLibraryInfo    = "SiGetLibraryInfo";
static LPCSTR fnSiGetDeviceInfo     = "SiGetDeviceInfo";
static LPCSTR fnSpwErrorString      = "SpwErrorString";

/* DLL initialization status */
static enum InitResult gInitStatus = NOT_LOADED;

/* DLL handle */
HINSTANCE ghDll;
enum SpwRetVal SpwErrorVal;

/*-----------------------------------------------------------------------------
 * 
 *  void __SiAppInitialize(void)
 *
 *  Args:
 *    None
 *
 *  Return Value:
 *    None
 *
 *  Description:
 *    This function opens up the DLL library and, if successful, loads a 
 *  function pointer table with exported UAPI functions contained in the DLL.
 *
 *---------------------------------------------------------------------------*/

int __SiAppInitialize(void)
{
   enum InitResult nResult;      /* intialization result code */
   enum ErrorCode nErrorCode;    /* function return code */

   /* attempt to load the DLL */
   if ((ghDll = LoadLibrary (DllLibrary)) != NULL)
      {
      /* preset variables and hope for success */
      nResult = LOADED;
      nErrorCode = NO_DLL_ERROR;

      /* load up the global variable used by the DLL to return error info */

      if ((pDllSpwRetVal = (enum SpwRetVal *)
                           GetProcAddress(ghDll, strDLLRetVal)) == NULL)
         {
         nResult = FAILED;
         nErrorCode = DLL_VAR_LOAD_FAILURE;
         }

      /* load up function pointer table */
      if ((pfnSiBeep = (PFNSI_BEEP)
                           GetProcAddress(ghDll, fnSiBeep)) == NULL)
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }

      if ((pfnSiInit = (PFNSI_INIT) 
                           GetProcAddress(ghDll, fnSiInitialize)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiTerminate = (PFNSI_TERMINATE) 
                           GetProcAddress(ghDll, fnSiTerminate)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGetNumDevices = (PFNSI_GETNUMDEVICES)
                           GetProcAddress(ghDll, fnSiGetNumDevices)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
      
      if ((pfnSiGetDeviceID = (PFNSI_GETDEVICEID) 
                           GetProcAddress(ghDll, fnSiGetDeviceID)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }

      if ((pfnSiDeviceIndex = (PFNSI_DEVICEINDEX) 
                           GetProcAddress(ghDll, fnSiDeviceIndex)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiDispatch = (PFNSI_DISPATCH)
                           GetProcAddress(ghDll, fnSiDispatch)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiOpenWinInit = (PFNSI_OPENWININIT)
                           GetProcAddress(ghDll, fnSiOpenWinInit)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiOpen = (PFNSI_OPEN)
                           GetProcAddress(ghDll, fnSiOpen)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiClose = (PFNSI_CLOSE) 
                           GetProcAddress(ghDll, fnSiClose)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGetEventWinInit = (PFNSI_GETEVENTWININIT) 
                           GetProcAddress(ghDll, fnSiGetEventWinInit)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiIsSpaceWareEvent = (PFNSI_ISSPACEWAREEVENT)
                           GetProcAddress(ghDll, fnSiIsSpaceWareEvent)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGetEvent = (PFNSI_GETEVENT)
                           GetProcAddress(ghDll, fnSiGetEvent)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiRezero = (PFNSI_REZERO)
                           GetProcAddress(ghDll, fnSiRezero)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGrabDevice = (PFNSI_GRABDEVICE)
                           GetProcAddress(ghDll, fnSiGrabDevice)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiReleaseDevice = (PFNSI_RELEASEDEVICE)
                           GetProcAddress(ghDll, fnSiReleaseDevice)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiButtonPressed = (PFNSI_BUTTONPRESSED)
                           GetProcAddress(ghDll, fnSiButtonPressed)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiButtonReleased = (PFNSI_BUTTONRELEASED)
                           GetProcAddress(ghDll, fnSiButtonReleased)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiSetUiMode = (PFNSI_SETUIMODE)
                           GetProcAddress(ghDll, fnSiSetUIMode)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGetDevicePort = (PFNSI_GETDEVICEPORT)
                           GetProcAddress(ghDll, fnSiGetDevicePort)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGetDriverInfo = (PFNSI_GETDRIVERINFO)
                           GetProcAddress(ghDll, fnSiGetDriverInfo)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGetLibraryInfo = (PFNSI_GETLIBRARYINFO)
                           GetProcAddress(ghDll, fnSiGetLibraryInfo)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSiGetDeviceInfo = (PFNSI_GETDEVICEINFO)
                           GetProcAddress(ghDll, fnSiGetDeviceInfo)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
         
      if ((pfnSpwErrorString = (PFNSPW_ERRORSTRING)
                           GetProcAddress(ghDll, fnSpwErrorString)) == NULL)       
         {
         nResult = FAILED;
         nErrorCode = DLL_FUNCTION_LOAD_FAILURE;
         }
      }
   else  /* couldn't load DLL */
      {
      nResult = FAILED;
      nErrorCode = DLL_LOAD_FAILURE;
      }

   /* set the global variable */

   gInitStatus = nResult;
   
   return nErrorCode;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiInitialize (void)
 *
 * Args:
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SPW_ERROR         Initialization error
 *
 * Description:
 *    This function initializes the SpaceWare input library.  Since most input
 *    functions require that the library be initialized, this function should
 *    be called before any other input functions are called.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal 
SiInitialize(void)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      __SiAppInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiInit();
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * void SiTerminate (void)
 *
 * Args:
 *   None
 *
 * Return Value:
 *   None
 *
 * Description:
 *    This function must be called to properly shut down the SpaceWare input
 *    library.  No other input functions (except SiInit) should be called
 *    afterwards.
 *
 *---------------------------------------------------------------------------*/

void
SiTerminate(void)
{
   /* It is unlikely that the function table will not have been loaded by the
      time this is called, but we'll cover the bases anyhow */
   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         SpwErrorVal = SPW_DLL_LOAD_ERROR;  /* global variable */
         return;
         }
   
      /* if we weren't returning void, we'd do error handling here */
      }

   pfnSiTerminate();
   SpwErrorVal = *pDllSpwRetVal;

   /* Unload the DLL to prevent memory leaks */
   FreeLibrary(ghDll);

   /* change status in case someone reinitializes */
   gInitStatus = NOT_LOADED;
}

/*-----------------------------------------------------------------------------
 *
 * int SiGetNumDevices (void)
 *
 * Args:
 *
 * Return Value:
 *    If the SpaceWare input library is initialized, the number of devices is
 *    returned and SpwErrorVal is set to SPW_NO_ERROR.  Otherwise, the return
 *    code is -1 and SpwErrorVal is set to either SI_UNINITIALIZED or one of
 *    the other standard error codes.
 *
 * Description:
 *    This function returns the number of input devices detected by the
 *    driver.
 *
 *---------------------------------------------------------------------------*/

int
SiGetNumDevices (void)
{
   int tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         SpwErrorVal = SPW_DLL_LOAD_ERROR;  /* global variable */
         return -1;
         }
      }

   tmpRetVal = pfnSiGetNumDevices();
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * SiDevID SiGetDeviceID (SiHdl hdl)
 *
 * Args:
 *    hdl      (r/o) SpaceWare handle
 *
 * Return Value:
 *    If there is no error, the device ID is returned and SpwErrorVal is set
 *    to SPW_NO_ERROR.  Otherwise, the return value is SI_NO_DEVICE and
 *    SpwErrorVal is set to either SI_BAD_HANDLE (if the SpaceWare handle is
 *    invalid) or one of the other standard error codes.
 *
 * Description:
 *    Given a SpaceWare handle, this function returns the ID of the
 *    associated device.
 *
 *---------------------------------------------------------------------------*/

SiDevID
SiGetDeviceID (SiHdl hdl)
{
   SiDevID tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         SpwErrorVal = SPW_DLL_LOAD_ERROR;  /* global variable */
         return SI_NO_DEVICE;
         }
      }

   tmpRetVal = pfnSiGetDeviceID(hdl);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * SiDevID SiDeviceIndex (int idx)
 *
 * Args:
 *    idx   (r/o) Device index (zero relative)
 *
 * Return Value:
 *    If there is no error, the device ID is returned and SpwErrorVal is set
 *    to SPW_NO_ERROR.  Otherwise, the return value is SI_NO_DEVICE and
 *    SpwErrorVal is set to either SPW_ERROR (if the device index is invalid),
 *    SI_UNINITIALIZED, or SI_NO_DRIVER.
 *
 * Description:
 *    Given a device index, SiDeviceIndex returns the associated device ID.
 *    This routine is particularly useful with the function SiGetDevicePort.
 * 
 *---------------------------------------------------------------------------*/

SiDevID
SiDeviceIndex (int idx)
{
   SiDevID tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         SpwErrorVal = SPW_DLL_LOAD_ERROR;  /* global variable */
         return SI_NO_DEVICE;
         }
      }

   tmpRetVal = pfnSiDeviceIndex(idx);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * int SiDispatch (SiHdl hdl, SiGetEventData *pData, SiSpwEvent *pEvent,
 *    SiSpwHandlers *pHandlers)
 *
 * Args:
 *    hdl         (r/o) SpaceWare handle
 *    pData       (r/o) Pntr to platform specific data (as passed to SiGetEvent)
 *    pEvent      (r/o) Pntr to SpaceWare event (as returned by SiGetEvent)
 *    pHandlers   (r/o) Pntr to SpaceWare event handlers
 *
 * Return Value:
 *    Value returned by called event handler or zero if handler not defined.
 *
 * Description:
 *    Provided as a companion to SiGetEvent, this function calls the appro-
 *    priate handler to process the SpaceWare event returned by SiGetEvent.
 *    The SiSpwHandlers structure contains one handler for each event type.
 *    An event type can be ignored by setting the func member of the appro-
 *    priate SiEventHandler structure to NULL.  Unlike the handler for
 *    SiGetEvent, there is no specific use for the function return value of
 *    these handlers -- the interpretation is entirely the caller's.
 *
 *---------------------------------------------------------------------------*/

int
SiDispatch (SiHdl hdl, SiGetEventData *pData,
                SiSpwEvent *pEvent, SiSpwHandlers *pDHandlers)
{
   int tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return 0;
         }
      }

   tmpRetVal = pfnSiDispatch(hdl, pData, pEvent, pDHandlers);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * void SiOpenWinInit (SiOpenData *pData, HWND hWnd)
 *
 * Args:
 *    pData (w/o) Pointer to storage for returned platform specific data
 *    hWnd  (r/o) Window handle
 *
 * Return Value:
 *    Nothing
 *
 * Description:
 *    This function initializes the Windows platform specific data for a
 *    subsequent call to SiOpen.
 * 
 *---------------------------------------------------------------------------*/

void
SiOpenWinInit (SiOpenData *pData, HWND hWnd)
{
   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         pData = NULL;
         hWnd = NULL;
         return;
         }
      }

   pfnSiOpenWinInit(pData, hWnd);
   SpwErrorVal = *pDllSpwRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * SiHdl SiOpen (char *pAppName, SiDevID devID, SiTypeMask *pTMask, int mode,
 *    SiOpenData *pData)
 *
 * Args:
 *    pAppName (r/o) Pointer to application name
 *    devID    (r/o) Device ID or SI_ANY_DEVICE
 *    pTMask   (r/o) Pointer to device type mask or SI_NO_MASK
 *    mode     (r/o) SpaceWare event retrieval method:  SI_EVENT or SI_POLL
 *    pData    (r/o) Pointer to platform specific data if mode is SI_EVENT
 *
 * Return Value:
 *    SpaceWare handle or SI_NO_HANDLE if error.  SpwErrorVal is set to one
 *    of the following:
 *
 *    SPW_NO_ERROR      No error
 *    SPW_ERROR         Device could not be opened
 *    SI_BAD_ID         Invalid device ID
 *    SI_BAD_VALUE      Invalid argument
 *    SI_UNSUPPORTED    Specified retrieval method is unsupported
 *
 * Description:
 *    SiOpen is called to open a device for input and (sometimes) output
 *    access.  The device in which to open is indicated via the device ID
 *    (devID) and a device type mask (pTMask).  If the ID of a particular
 *    device is known, it is passed in devID.  If the ID is not known or the
 *    application simply doesn't care which device is selected, devID is passed
 *    as SI_ANY_DEVICE.
 *
 *    Irrespective of the device ID, the device type mask indicates which
 *    device types are permitted.  The type may be constrained to particular
 *    devices, classes, or any combination thereof.  The mask is created via
 *    the function SiSetTypeMask (see the definition of that function for more
 *    information).  If the application doesn't wish to constrain the selection
 *    to any particular types or classes, pTMask may be passed as SI_NO_MASK.
 *    This is the equivalent of passing a mask set to SI_ALL_TYPES.
 *
 *    The following table shows how a device is selected based on the device ID
 *    and type mask.  In those cases where SI_ANY_DEVICE is specified, the
 *    order in which the devices are searched is the same order as indexed by
 *    the function SiDevIndex.
 *
 *    devID            pTMask          Device selected
 *
 *    SI_ANY_DEVICE    SI_NO_MASK      The first available device in the list.
 *
 *    A device ID      SI_NO_MASK      The device matching the specified ID if
 *                                     it's available.
 *
 *    SI_ANY_DEVICE    A type mask     The first available device in the list
 *                                     with a type or class that is specified
 *                                     in the mask.
 *
 *    A device ID      A type mask     The device matching the specified ID if
 *                                     its type or class matches one specified
 *                                     in the mask and it's available.
 *
 *    Note that some SpaceWare implementations will run only if all of the
 *    configured devices are attached and working properly.  Thus, all of the
 *    devices are available.  Other implementations may run with some devices
 *    not working.  These constitute unavailable devices.
 *
 *    The mode argument specifies how the application intends to retrieve
 *    data from the device.  SI_EVENT indicates that the application should
 *    be informed, via a system event message, whenever device data is
 *    pending.  The message is acknowledged by a call to SiGetEvent. When
 *    this mode is selected, pData points to the platform specific informa-
 *    tion necessary for the API to generate system event messages.  The mode
 *    SI_POLL indicates that the application will check for data from the
 *    device by calling SiGetEvent regularly.  For this mode, pData is passed
 *    as NULL.
 *
 *---------------------------------------------------------------------------*/

SiHdl
SiOpen (char *pAppName, SiDevID devID, SiTypeMask *pTMask, int mode, 
              SiOpenData *pData)
{
   SiHdl tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         SpwErrorVal = SPW_DLL_LOAD_ERROR;
         return NULL;
         }
      }

   tmpRetVal = pfnSiOpen(pAppName, devID, pTMask, mode, pData);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;

}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiClose (SiHdl hdl)
 *
 * Args:
 *    hdl   (r/o) SpaceWare handle
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SPW_ERROR         Close error
 *    SI_BAD_HANDL      Invalid SpaceWare handle
 *
 * Description:
 *    This function closes a device.   Once closed, the SpaceWare handle is
 *    no longer valid.
 *
 *---------------------------------------------------------------------------*/

enum
SpwRetVal SiClose (SiHdl hdl)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiClose(hdl);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;

}

/*-----------------------------------------------------------------------------
 *
 * SPWbool SiIsSpaceWareEvent (SiGetEventData *pData, SiHdl *pHdl)
 *
 * Args:
 *    pData (r/o) Pointer to platform specific data
 *    pHdl  (w/o) Pointer to storage for returned SpaceWare handle
 *
 * Return Value:
 *    SPW_TRUE if pData refers to a SpaceWare event, otherwise SPW_FALSE.
 *
 * Description:
 *    This function determines whether or not the data addressed by pData per-
 *    tains to a SpaceWare event.  If so, the handle for which the event is
 *    intended is returned.
 *
 * Notes:
 *    NULL may be passed for pHdl if the return of the intended handle is not
 *    required.
 * 
 *---------------------------------------------------------------------------*/

SPWbool
SiIsSpaceWareEvent(SiGetEventData *pData, SiHdl *pHdl)
{
   SPWbool tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_FALSE;
         }
      }

   tmpRetVal = pfnSiIsSpaceWareEvent(pData, pHdl);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * void SiGetEventWinInit (SiGetEventData *pData,
 *    UINT msg, WPARAM wParam, LPARAM lParam)
 *
 * Args:
 *    pData    (w/o) Pointer to storage for returned platform specific data
 *    msg      (r/o) Windows message
 *    wParam   (r/o) Parameter #1
 *    lParam   (r/o) Parameter #2
 *
 * Return Value:
 *    Nothing
 *
 * Description:
 *    This function initializes the Windows platform specific data for a
 *    subsequent call to SiGetEvent.
 * 
 *---------------------------------------------------------------------------*/

void 
SiGetEventWinInit (SiGetEventData *pData, UINT msg, WPARAM wParam, 
                   LPARAM lParam)
{
   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return;
         }
      }

   pfnSiGetEventWinInit(pData, msg, wParam, lParam);
   SpwErrorVal = *pDllSpwRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiGetEvent (SiHdl hdl, int flags,
 *    SiGetEventData *pData, SiSpwEvent *pEvent)
 *
 * Args:
 *    hdl      (r/o) SpaceWare handle
 *    flags    (r/o) Processing flags
 *    pData    (r/o) Pointer to platform specific data
 *    pEvent   (w/o) Pointer to storage for returned SpaceWare event
 *
 * Return Value:
 *    SI_BAD_HANDLE     Invalid SpaceWare handle
 *    SI_NOT_EVENT      The event is not a SpaceWare event or no event pending
 *    SI_IS_EVENT       The event is a SpaceWare event
 *    SI_SKIP_EVENT     The event is a SpaceWare event but it should be skipped
 *
 * Description:
 *    This function determines if device data is pending and if so, returns
 *    the data as a SpaceWare event.  See the "SpaceWare Universal API"
 *    specification for a complete description of this function.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiGetEvent (SiHdl hdl, int flags, SiGetEventData *pData, 
                           SiSpwEvent *pEvent)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiGetEvent(hdl, flags, pData, pEvent);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiBeep (SiHdl hdl, char *pString)
 *
 * Args:
 *    hdl      (r/o) SpaceWare handle
 *    pString  (r/o) Pointer to beep description string
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SI_BAD_HANDLE     Invalid SpaceWare handle
 *    SI_BAD_VALUE      Bad string
 *
 * Description:
 *    If supported, this function causes the input device to emit a sequence
 *    of tones and pauses.  Each character of the string represents either a
 *    tone or a delay.  Lowercase letters [a-z] represent a tone, uppercase
 *    letters [A-Z] represent a pause.  The closer the letter is to the
 *    beginning of the alphabet the shorter the pause or tone ('a' or 'A' is
 *    1/32 second, 'b' or 'B' is 2/32 second, etc).  Up to 14 characters in
 *    a string are processed, additional characters are ignored.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiBeep(SiHdl hdl, char *pString)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiBeep(hdl, pString); 
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiRezero (SiHdl hdl)
 *
 * Args:
 *    hdl   (r/o) SpaceWare handle
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SI_BAD_HANDLE     Invalid SpaceWare handle
 *
 * Description:
 *    This function causes the input device's current setting to be defined
 *    as the rest position.  Movement away from this position will cause
 *    motion events. This can be used to eliminate drift or to set up a
 *    constant motion.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiRezero (SiHdl hdl)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiRezero(hdl); 
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiGrabDevice (SiHdl hdl, SPWbool exclusive)
 *
 * Args:
 *    hdl         (r/o) SpaceWare handle
 *    exclusive   (r/o) Exclusive grab flag
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SPW_ERROR         Grab failed
 *    SI_BAD_HANDLE     Invalid SpaceWare handle
 *
 * Description:
 *    This function is used to "grab" or capture a device in multi-tasking
 *    environments.  For an exclusive capture, the application retains control
 *    of the device regardless of which window has the focus.  For a non-
 *    exclusive capture, the application retains control of the device only if
 *    no other application, sharing the same device, has the focus.  Some
 *    SpaceWare implementations only allow one non-exclusive grab.  Others
 *    allow multiple non-exclusive grabs and provide an interface for inter-
 *    actively selecting the current non-exclusive grab application.  An exclu-
 *    sive grab fails if another application has already exclusively captured 
 *    the device.  A non-exclusive grab fails if the SpaceWare implementation
 *    only allows one non-exclusive grab and another application has already
 *    non-exclusively captured the device.  Note that an exclusive grab should
 *    be used sparingly if at all.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiGrabDevice (SiHdl hdl, SPWbool exclusive)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiGrabDevice(hdl, exclusive);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiReleaseDevice (SiHdl hdl)
 *
 * Args:
 *    hdl   (r/o) SpaceWare handle
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SPW_ERROR         Device not captured
 *    SI_BAD_HANDLE     Invalid SpaceWare handle
 *
 * Description:
 *    This function releases a device that has been captured via the routine
 *    SiGrabDevice.  A captured device is automatically released when closed.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiReleaseDevice (SiHdl hdl)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiReleaseDevice(hdl);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * int SiButtonPressed (SiSpwEvent *pEvent)
 *
 * Args:
 *    pEvent   (r/o) Pointer to SpaceWare event
 *
 * Return Value:
 *    Button number or SI_NO_BUTTON if not a button or combo event or no
 *    button was pressed.  If it's not a button or combo event, SpwErrorVal
 *    is set to SPW_ERROR otherwise SpwErrorVal is SPW_NO_ERROR.
 *
 * Description:
 *    Given a button or combo event, this function returns the number of the
 *    button pressed.  In the event that multiple buttons are pressed, the
 *    lowest button number is returned.
 *
 *---------------------------------------------------------------------------*/

int
SiButtonPressed (SiSpwEvent *pEvent)
{
   int tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         SpwErrorVal = SPW_DLL_LOAD_ERROR;
         return SI_NO_BUTTON;
         }
      }

   tmpRetVal = pfnSiButtonPressed(pEvent);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * int SiButtonReleased (SiSpwEvent *pEvent)
 *
 * Args:
 *    pEvent   (r/o) Pointer to SpaceWare event
 *
 * Return Value:
 *    Button number or SI_NO_BUTTON if not a button or combo event or no
 *    button was released. If it's not a button or combo event, SpwErrorVal
 *    is set to SPW_ERROR otherwise SpwErrorVal is SPW_NO_ERROR.
 *
 * Description:
 *    Given a button or combo event, this function returns the number of the
 *    button released.  In the event that multiple buttons are released, the
 *    lowest button number is returned.
 *
 *---------------------------------------------------------------------------*/

int
SiButtonReleased (SiSpwEvent *pEvent)
{
   int tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         SpwErrorVal = SPW_DLL_LOAD_ERROR;
         return SI_NO_BUTTON;
         }
      }

   tmpRetVal = pfnSiButtonReleased(pEvent);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiSetUiMode (SiHdl hdl, SPWuint32 mode)
 *
 * Args:
 *    hdl   (r/o) SpaceWare handle or SI_ALL_HANDLES
 *    mode  (r/o) UI mode
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SI_BAD_HANDLE     Invalid SpaceWare handle
 *
 * Description:
 *    This function sets the current User Interface (UI) mode bit mask for a
 *    specified device.  If hdl is SI_ALL_HANDLES, the mask for each open
 *    device is set.  This option is useful if an app uses the same UI mode
 *    for more than one device.  Each set bit in the mask indicates that a
 *    particular feature is enabled.  The bits are defined as follows.
 *
 *       SI_UI_ALL_CONTROLS      All controls
 *       SI_UI_NO_CONTROLS       No controls
 *       SI_UI_FILTERS           Filter controls
 *       SI_UI_FUNC_BUTTONS      Function buttons
 *       SI_UI_RESET_BUTTONS     Reset buttons
 *       SI_UI_SENSITIVITY       Sensitivity control
 *       SI_UI_TUNING            Tuning control
 *       SI_UI_DIALOG_POPUP      Dialog popup button (on device)
 *
 *    Note that for SI_UI_ALL_CONTROLS, all bits are set; for SI_UI_NO_CONTROLS,
 *    no bits are set.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiSetUiMode (SiHdl hdl, SPWuint32 mode)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiSetUiMode(hdl, mode);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiSetTypeMask (SiTypeMask *pTMask, int type1, ...,
 *    SI_END_ARGS)
 *
 * Args:
 *    pTMask   (w/o) Pointer to storage for returned device type mask
 *    type1    (r/o) First device type
 *    ...      (r/o) Additional device types
 *
 * Return Value:
 *    SPW_NO_ERROR   No error
 *
 * Description:
 *   This function provides compatibility with older software.
 * 
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiSetTypeMask (SiTypeMask *pTMask, int type1, ...)
{
   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   /* stub function -> return a happy value */
   return SPW_NO_ERROR;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiGetDevicePort (SiDevID devID, SiDevPort *pPort)
 *
 * Args:
 *    devID (r/o) Device ID
 *    pPort (w/o) Pointer to storage for returned device port info
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SI_BAD_ID         Invalid device ID
 *
 * Description:
 *    The purpose of this function is to provide a correlation between the
 *    devices and the system ports to which they are connected.  Each call
 *    to SiGetDevicePort returns information for one device.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiGetDevicePort (SiDevID devID, SiDevPort *pPort)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiGetDevicePort(devID, pPort);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiGetDriverInfo (SiVerInfo *pInfo)
 *
 * Args:
 *    pInfo (w/o) Pointer to storage for returned driver version info
 *
 * Return Value:
 *    SPW_NO_ERROR  No error
 *    SPW_ERROR     This version of the SpaceWare library does not use a driver
 *
 * Description:
 *    SiGetDriverInfo returns driver version information.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiGetDriverInfo (SiVerInfo *pInfo)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
      }

   tmpRetVal = pfnSiGetDriverInfo(pInfo);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * void SiGetLibraryInfo (SiVerInfo *pInfo)
 *
 * Args:
 *    pInfo (w/o) Pointer to storage for returned library version info
 *
 * Return Value:
 *    Nothing.  SpwErrorVal is always set to SPW_NO_ERROR by this function.
 *
 * Description:
 *    SiGetLibraryInfo returns library version information.
 *
 *---------------------------------------------------------------------------*/

void
SiGetLibraryInfo (SiVerInfo *pInfo)
{
   if (gInitStatus != LOADED)
      {   
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         pInfo = NULL;
         return;
         }

      }

   pfnSiGetLibraryInfo(pInfo);
   SpwErrorVal = *pDllSpwRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * enum SpwRetVal SiGetDeviceInfo (SiHdl hdl, SiDevInfo *pInfo)
 *
 * Args:
 *    hdl   (r/o) SpaceWare handle
 *    pInfo (w/o) Pointer to storage for returned device information
 *
 * Return Value:
 *    SPW_NO_ERROR      No error
 *    SI_BAD_HANDLE     Invalid SpaceWare handle
 *
 * Description:
 *    This function returns all sorts of information on the specified device.
 *
 *---------------------------------------------------------------------------*/

enum SpwRetVal
SiGetDeviceInfo (SiHdl hdl, SiDevInfo *pInfo)
{
   enum SpwRetVal tmpRetVal;  /* temporary return value */

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return SPW_DLL_LOAD_ERROR;
         }
   }

   tmpRetVal = pfnSiGetDeviceInfo(hdl, pInfo);
   SpwErrorVal = *pDllSpwRetVal;
   return tmpRetVal;
}

/*-----------------------------------------------------------------------------
 *
 * char *SpwErrorString (enum SpwRetVal val)
 *
 * Parameters:
 *    val   (r/o) Error value
 *
 * Return:
 *    Pointer to string containing error message.
 *
 * Description:
 *    This function returns a pointer to the error message string associated
 *    with the given error value.  Note that the returned pointer points to
 *    static memory that is overridden each time SpwErrorString is called.
 *
 *---------------------------------------------------------------------------*/

char *
SpwErrorString (enum SpwRetVal val)
{
  int p = 0;

   if (gInitStatus != LOADED)
      {
      SiInitialize();
      if (gInitStatus == FAILED)
         {
         return NULL;
         }
      }

    p = pfnSpwErrorString(val);
    if ( 4 == sizeof(char*) )
    {
      // this was used in V3 and is used in the 32 bit V4
      // I'm supressing the warning because the 4 == sizeof(char*)
      // check prevents a bogus pointer from being returned.
#pragma warning( push )
#pragma warning( disable : 4312 )
  // warning C4312: 'type cast' : conversion from 'unsigned int' 
  //                              to 'int *' of greater size
        return ((char*)p);
#pragma warning ( pop )
    }

    return 0; // 64 bit version cannot cast the int as a pointer
}
