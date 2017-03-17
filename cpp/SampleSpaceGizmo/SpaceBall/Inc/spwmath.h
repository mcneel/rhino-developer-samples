/*-----------------------------------------------------------------------------
 *
 *  spwmath.h: SpaceWare Math Library public definitions 
 *
<<<<<<< spwmath.h
 *  $Id: spwmath.h,v 1.3 2001/01/16 01:18:40 HJin Exp $
=======
 *  $Id: spwmath.h,v 1.3 2001/01/16 01:18:40 HJin Exp $
>>>>>>> 1.1.1.1.4.1
 *
 *-----------------------------------------------------------------------------
 *
 */

#ifndef SPW_MATH_H
#define SPW_MATH_H

#include "spwmacro.h"

#define SPW_MATH_MAJOR    3
#define SPW_MATH_MINOR    0
#define SPW_MATH_UPDATE   0
#define SPW_MATH_BUILD    7
#define SPW_MATH_VERSION  "MATH version 3.0"
#define SPW_MATH_DATE     "March 27, 1998"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef EXPORT_FUNCTIONS
#define DLL_EXPORT __declspec(dllexport)
#define CALL_TYPE __stdcall
#else
#define DLL_EXPORT
#define CALL_TYPE __stdcall
#endif

/* PUBLIC VARIABLES & DEFINES */

#if !defined(__GL_GL_H__) && !defined(SPW_NO_MATRIX)
typedef float Matrix[4][4];
#endif

DLL_EXPORT void SPW_ArbitraryAxisToMatrix (Matrix, float[3], float);
DLL_EXPORT int SPW_CompareMatrices (Matrix, Matrix);
DLL_EXPORT float SPW_DotProduct (float [3], float [3]);
DLL_EXPORT float SPW_GetMatrixScale (Matrix);
DLL_EXPORT void SPW_InvertMatrix (Matrix, Matrix);      
DLL_EXPORT void SPW_LookAtUpVectorToMatrix (Matrix, float [3], float [3], 
                                            float [3]);
DLL_EXPORT void SPW_MatrixToArbitraryAxis (float [3], float *, Matrix);
DLL_EXPORT float SPW_MatrixDeterminant (Matrix);
DLL_EXPORT void SPW_MatrixToLookAtUpVector (float [3], float [3], float [3], 
                                            Matrix);
DLL_EXPORT void SPW_Mult44x44 (Matrix, Matrix, Matrix);
DLL_EXPORT void SPW_MultFull14x44 (float [3], float [3], Matrix);
DLL_EXPORT void SPW_Mult14x44 (float [3], float [3], Matrix);
DLL_EXPORT void SPW_Mult33x33 (Matrix, Matrix, Matrix);
DLL_EXPORT void SPW_Mult13x33 (float [3], float [3], Matrix);
DLL_EXPORT int SPW_OrthoNormMatrix (Matrix);
DLL_EXPORT float SPW_VectorLength (float [3]);


DLL_EXPORT void SPW_TransposeMatrix (Matrix, Matrix);
DLL_EXPORT void SPW_CopyMatrix (Matrix, Matrix);
DLL_EXPORT void SPW_ScaleMatrix (Matrix, Matrix, float);
DLL_EXPORT void SPW_GetTranslationMatrix (Matrix, Matrix);
DLL_EXPORT void SPW_InitializeMatrix (Matrix, float, float, float, float,
                                      float, float, float, float,
                                      float, float, float, float,
                                      float, float, float, float);
DLL_EXPORT void SPW_MakeIdentityMatrix (Matrix);
DLL_EXPORT void SPW_NormalizeVector (float [3], float [3]);
DLL_EXPORT void SPW_CrossProduct (float [3], float [3], float [3]);
DLL_EXPORT void SPW_PrintMatrix (char *, Matrix);
DLL_EXPORT void SPW_PrintVector (char *, float [3]);
DLL_EXPORT void SPW_PrintSpaceballData (char *, float [7]);
DLL_EXPORT void SPW_HighValuePassFilter (float *, int);

#ifdef __cplusplus
}
#endif

#endif /* SPW_MATH_H */






