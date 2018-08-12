#ifndef IF_H
#define IF_H

#include <base_macro.h>

#if defined(_MSC_VER)
#if defined(DLL_EXPORT)
#define DLL_EXPORT_API _declspec(dllexport)
#else
#define DLL_EXPORT_API _declspec(dllimport)
#endif
#else
#endif
#if defined(DLL_EXPORT)
#define DLL_EXPORT_API __attribute__ ((visibility ("default")))
#else
#define DLL_EXPORT_API
#endif

EXTERN_C DLL_EXPORT_API char * hello_text(void);

#endif