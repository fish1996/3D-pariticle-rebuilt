//
// MATLAB Compiler: 6.1 (R2015b)
// Date: Wed Apr 05 22:55:02 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:dwt22" "-T" "link:lib" "-d"
// "C:\Users\Administrator\Desktop\temp\demo 2\demo\dwt22\for_testing" "-v"
// "C:\Users\Administrator\Desktop\temp\demo 2\demo\dwt22.m" 
//

#ifndef __dwt22_h
#define __dwt22_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_dwt22
#define PUBLIC_dwt22_C_API __global
#else
#define PUBLIC_dwt22_C_API /* No import statement needed. */
#endif

#define LIB_dwt22_C_API PUBLIC_dwt22_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_dwt22
#define PUBLIC_dwt22_C_API __declspec(dllexport)
#else
#define PUBLIC_dwt22_C_API __declspec(dllimport)
#endif

#define LIB_dwt22_C_API PUBLIC_dwt22_C_API


#else

#define LIB_dwt22_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_dwt22_C_API 
#define LIB_dwt22_C_API /* No special import/export declaration */
#endif

extern LIB_dwt22_C_API 
bool MW_CALL_CONV dwt22InitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_dwt22_C_API 
bool MW_CALL_CONV dwt22Initialize(void);

extern LIB_dwt22_C_API 
void MW_CALL_CONV dwt22Terminate(void);



extern LIB_dwt22_C_API 
void MW_CALL_CONV dwt22PrintStackTrace(void);

extern LIB_dwt22_C_API 
bool MW_CALL_CONV mlxDwt22(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_dwt22
#define PUBLIC_dwt22_CPP_API __declspec(dllexport)
#else
#define PUBLIC_dwt22_CPP_API __declspec(dllimport)
#endif

#define LIB_dwt22_CPP_API PUBLIC_dwt22_CPP_API

#else

#if !defined(LIB_dwt22_CPP_API)
#if defined(LIB_dwt22_C_API)
#define LIB_dwt22_CPP_API LIB_dwt22_C_API
#else
#define LIB_dwt22_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_dwt22_CPP_API void MW_CALL_CONV dwt22(int nargout, mwArray& ll, mwArray& hh, mwArray& vv, mwArray& dd, const mwArray& Gx);

#endif
#endif
