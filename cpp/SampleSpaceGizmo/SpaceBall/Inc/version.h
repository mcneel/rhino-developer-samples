/*----------------------------------------------------------------------
 *  version.h - Version numbers for SpaceWare
 *
<<<<<<< version.h
 *  $Id: version.h,v 1.6 2001/05/21 18:13:15 HJin Exp $
=======
 *  $Id: version.h,v 1.6 2001/05/21 18:13:15 HJin Exp $
>>>>>>> 1.1.1.1.4.1
 *
 *  SpaceWare input library
 *
 *----------------------------------------------------------------------
 */

#ifndef _VERSION_H_
#define _VERSION_H_ 1

/* The 1 in the preceding #define is to satisfy InstallShield */


/*
 *  Dates must follow the Month Day, Year format, in order for
 *  the localized demos/apps to read the date and reformat it in
 *  the appropriate user locale.  More specifically the full capitalized 
 *  month name must be used!  In addition the day and year must be
 *  be comma separated and have one space.  For example:
 *  March 24, 1998
 *  July 14, 1998
 */
#define SPW_SDK_VERSION       "1.0.002"
#define SII_SDK_MAJOR         1
#define SII_SDK_MINOR         0
#define SII_SDK_BUILD         2
#define SII_SDK_VERSION       "SpaceWare SDK"
#define SII_SDK_DATE          "March 24, 1998"

#define SPW_LIB_VERSION       "9.5.0"
#define SII_LIB_MAJOR         9
#define SII_LIB_MINOR         5
#define SII_LIB_BUILD         0
#define SII_LIB_VERSION       "SpaceWare Library"
#define SII_LIB_DATE          "September 6, 2001"

#define SPW_DVR_VERSION       "1.1.0 Build 2"
#define SII_DVR_MAJOR         1
#define SII_DVR_MINOR         1
#define SII_DVR_POINT         0
#define SII_DVR_BUILD         2
#define SII_DVR_VERSION       "3DxWare Driver"
#define SII_DVR_DATE          "December 2, 2001"

#endif
