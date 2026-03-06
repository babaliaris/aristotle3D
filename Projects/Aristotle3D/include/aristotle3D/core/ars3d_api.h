#ifndef ARS3D_API_H
#define ARS3D_API_H

#ifdef __cplusplus
    #define ARS3D_EXTERN extern "C"
#else
    #define ARS3D_EXTERN extern
#endif

#if defined(ARS3D_WINDOWS)

    #ifdef ARS3D_EXPORT_DLL
        #define ARS3D_API ARS3D_EXTERN __declspec(dllexport)
    #else
        #define ARS3D_API ARS3D_EXTERN __declspec(dllimport)
    #endif

#elif defined(ARS3D_LINUX)
    #define ARS3D_API ARS3D_EXTERN

#else
    #error Operating System is not supported
#endif


#if defined(ARS3D_X64)

#else
    #error Platform is not supported

#endif

#endif