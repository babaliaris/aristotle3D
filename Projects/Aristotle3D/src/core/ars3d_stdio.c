#include <aristotle3D/core/ars3d_stdio.h>


ars3d_int ars3dPrintf(const ars3d_char* p_fmt, ...)
{
    va_list args;
    va_start(args, p_fmt);
    
    ars3d_int return_val = vprintf(p_fmt, args);
    
    va_end(args);

    return return_val;
}



ars3d_void *ars3dMalloc(ars3d_size p_size)
{
    if (p_size == 0) return ARS3D_NULL;
    
    return malloc(p_size);
}

ars3d_void ars3dFree(ars3d_void *p_ptr)
{
    free(p_ptr);
}


ars3d_void *ars3dMemCopy(ars3d_void *p_dest, const ars3d_void *p_src, ars3d_size p_size)
{
    return memcpy(p_dest, p_src, p_size);
}


ars3d_char *ars3dStrCopy(ars3d_char *p_dest, const ars3d_char *p_src)
{
    return strcpy(p_dest, p_src);
}


ars3d_size ars3dStrLen(const ars3d_char *p_src)
{
    return strlen(p_src);
}
