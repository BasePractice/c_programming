#ifndef IF_H
#define IF_H

#if defined(_MSC_VER)
#if defined(DLL_EXPORT)
#define DLL_EXPORT_API _declspec(dllexport)
#else
#define DLL_EXPORT_API _declspec(dllimport)
#endif
#else
#endif
#if defined(DLL_EXPORT)
#define DLL_EXPORT_API __attribute__ ((visibility ("hidden")))
#else
#define DLL_EXPORT_API
#endif

DLL_EXPORT_API char * hello_text(void);

#endif