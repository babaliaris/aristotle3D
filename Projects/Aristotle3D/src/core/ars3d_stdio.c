#include <aristotle3D/core/ars3d_stdio.h>


ars3d_int ars3dPrintf(const ars3d_char* p_fmt, ...)
{
    #if defined (ARS3D_X64)
        va_list args;
        va_start(args, p_fmt);
        
        ars3d_int return_val = vprintf(p_fmt, args);
        
        va_end(args);

        return return_val;
    #endif
}



ars3d_void *ars3dMalloc(ars3d_size p_size)
{   
    #if defined (ARS3D_X64)
        if (p_size == 0) return ARS3D_NULL;
        
        return malloc(p_size);
    #endif
}

ars3d_void ars3dFree(ars3d_void *p_ptr)
{
    #if defined (ARS3D_X64)
        free(p_ptr);
    #endif
}


ars3d_void ars3dExit(ars3d_int p_exit_code)
{
    #if defined (ARS3D_X64)
        exit(p_exit_code);
    #endif
}


ars3d_void *ars3dMemCopy(ars3d_void *p_dest, const ars3d_void *p_src, ars3d_size p_size)
{
    #if defined (ARS3D_X64)
        return memcpy(p_dest, p_src, p_size);
    #endif
}


ars3d_char *ars3dStrCopy(ars3d_char *p_dest, const ars3d_char *p_src)
{
    #if defined (ARS3D_X64)
        return strcpy(p_dest, p_src);
    #endif
}


ars3d_size ars3dStrLen(const ars3d_char *p_src)
{
    #if defined (ARS3D_X64)
        return strlen(p_src);
    #endif
}
