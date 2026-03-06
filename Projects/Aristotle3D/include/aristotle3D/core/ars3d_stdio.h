#ifndef ARS3D_STDIO_H
#define ARS3D_STDIO_H
#include <aristotle3D/core/ars3d_api.h>

/**
 * @file
 * 
 * @brief Aristotle3D standard input/output and memory abstraction layer.
 * This module provides portability by wrapping standard library types and
 * functions, allowing for cross-platform support.
 */



#if defined(__cplusplus)

    #if __cplusplus >= 201103L
        #define ARS3D_NULL nullptr
    #else
        #define ARS3D_NULL 0L
    #endif

#else
    #define ARS3D_NULL ((void*)0)

#endif




#if defined(ARS3D_X64)
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <stdarg.h>
    #include <memory.h>
    #include <string.h>

    /** @addtogroup Types
     * @{ */
    typedef int8_t          ars3d_int8;
    typedef int16_t         ars3d_int16;
    typedef int32_t         ars3d_int32;
    typedef int64_t         ars3d_int64;

    typedef uint8_t         ars3d_uint8;
    typedef uint16_t        ars3d_uint16;
    typedef uint32_t        ars3d_uint32;
    typedef uint64_t        ars3d_uint64;

    typedef void            ars3d_void;
    typedef char            ars3d_char;
    typedef unsigned char   ars3d_uchar;
    typedef int             ars3d_int;
    typedef float           ars3d_float;
    typedef double          ars3d_double;
    typedef size_t          ars3d_size;
    /** @} */

    
    #define ARS3D_SIZEOF sizeof
#endif


/**
 * @brief Format based printf wrapper.
 * 
 * @param p_fmt A standard printf format string.
 * 
 * @returns The number of bytes printed.
 */
ARS3D_API ars3d_int ars3dPrintf(const ars3d_char* p_fmt, ...);


/**
 * @brief Ask the OS to allocate memory in the heap.
 * 
 * @param p_size The size in bytes that you wish to be allocated.
 * 
 * @returns The starting address in the virtual memory.
 */
ARS3D_API ars3d_void *ars3DMalloc(ars3d_size p_size);


/**
 * @brief Free a block of allocated memory in the heap.
 * 
 * @param p_ptr The memory address of the block.
 */
ARS3D_API ars3d_void ars3DFree(ars3d_void *p_ptr);


/**
 * @brief Copy byte by byte a memory buffer to another.
 * 
 * It copies byte by byte the content from p_dest to p_src.
 * 
 * @param p_dest The memory address of the destination buffer.
 * @param p_src The memory address of the source buffer.
 * @param p_size The total size to be copied.
 * 
 * @returns The destrination memory address, same as p_dest.
 */
ARS3D_API ars3d_void *ars3DMemCopy(ars3d_void *p_dest, const ars3d_void *p_src, ars3d_size p_size);



/**
 * @brief Copy a string including the null terminator.
 * 
 * It copies byte by byte the content from p_dest to p_src
 * including the null terminator character '\0'.
 * 
 * @param p_dest The memory address of the destination buffer.
 * @param p_src The memory address of the source buffer.
 * 
 * @returns The destrination memory address, same as p_dest.
 */
ARS3D_API ars3d_char *ars3DStrCopy(ars3d_char *p_dest, const ars3d_char *p_src);



/**
 * @brief Measure the length of a string.
 * 
 * @param p_src The memory address of the string buffer.
 * 
 * @returns The length of the string.
 */
ARS3D_API ars3d_size ars3DStrLen(const ars3d_char *p_src);

#endif
