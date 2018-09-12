#ifndef C_PROGRAMMING_PRACTICE_IF_H
#define C_PROGRAMMING_PRACTICE_IF_H

#include <base_macro.h>

#if defined(_MSC_VER)
#if defined(DLL_EXPORT)
#define DLL_EXPORT_API _declspec(dllexport)
#else
#define DLL_EXPORT_API _declspec(dllimport)
#endif
#else
#if defined(DLL_EXPORT)
#define DLL_EXPORT_API __attribute__ ((visibility ("default")))
#else
#define DLL_EXPORT_API
#endif
#endif

EXTERN_C DLL_EXPORT_API char * hello_text(void);

#endif