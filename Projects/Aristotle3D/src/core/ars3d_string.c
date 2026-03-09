#include <aristotle3D/core/ars3d_string.h>
#include <aristotle3D/core/ars3d_debug.h>

typedef struct Ars3DDynamicStr
{
    ars3d_char *m_char_buffer;
    ars3d_size m_length;
} Ars3DDynamicStr;


Ars3DDynamicStr *ars3dCreateDynamicStr(const ars3d_char *p_c_str)
{
    Ars3DDynamicStr *new_string = (Ars3DDynamicStr *)ars3dMalloc( ARS3D_SIZEOF(Ars3DDynamicStr) );

    ars3d_size c_str_size = ars3dStrLen(p_c_str);

    ars3d_char *char_buffer = (ars3d_char *)ars3dMalloc( ARS3D_SIZEOF(ars3d_char) * c_str_size);

    if (!new_string || !char_buffer)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    new_string->m_char_buffer   = char_buffer;
    new_string->m_length        = c_str_size;

    return new_string;
}



ars3d_void ars3dDestroyDynamicStr(Ars3DDynamicStr **p_str)
{
    if ( !p_str || !(*p_str) )
    {
        ARS3D_WARN("You provided a NULL placeholder OR one that points to NULL");
        return;
    }

    ars3dFree( (*p_str)->m_char_buffer );
    ars3dFree( *p_str );

    p_str = ARS3D_NULL;
}



ars3d_char *ars3dGetCStr(Ars3DDynamicStr *p_str)
{
    return p_str->m_char_buffer;
}



ars3d_size ars3dLenDynamicStr(Ars3DDynamicStr *p_str)
{
    return p_str->m_length;
}



ars3d_uchar ars3dCompareDynamicStr(Ars3DDynamicStr *p_str1, Ars3DDynamicStr *p_str2)
{
    return 0;
}

