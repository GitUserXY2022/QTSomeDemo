// .NAME SURV_LIBS_EXPORT - manage Windows system differences
// .SECTION Description
// The SURV_LIBS_EXPORT captures some system differences between Unix
// and Windows operating systems. 

#ifndef _SURV_LIBS_EXPORT_H_
#define _SURV_LIBS_EXPORT_H_

#include <QtGlobal>

#if defined(SURV_LIBS_STATIC) && defined (SURV_LIBS_DYNAMIC)
#  error "SURV_LIBS_STATIC" and "SURV_LIBS_DYNAMIC" can not be defined at the same time
#elif defined(SURV_LIBS_STATIC)
#  define SURV_LIBS_EXPORT
#elif defined(Q_OS_WIN) || defined(Q_OS_SYMBIAN)
// No define of OSGI_DYNAMIC and OSGI_STATIC is also treat as dynamic library.
// because OSGI is a framework, the default way is deal with dynamic library packages.
// Microsoft Visual C++ export
// http://www.codeproject.com/Articles/28969/HowTo-Export-C-classes-from-a-DLL
#  if defined(SURV_LIBS_BUILD)
#    define SURV_LIBS_EXPORT Q_DECL_EXPORT
#  else
#    define SURV_LIBS_EXPORT Q_DECL_IMPORT
#  endif
#endif

#if !defined(SURV_LIBS_EXPORT)
#  define SURV_LIBS_EXPORT
#endif

#endif
