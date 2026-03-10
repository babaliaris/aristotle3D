#include <aristotle3D/core/ars3d_string.h>
#include <aristotle3D/core/ars3d_debug.h>

typedef struct Ars3DDynamicStr
{
    ars3d_char *m_char_buffer;
    ars3d_size m_length;
} Ars3DDynamicStr;


Ars3DDynamicStr *ars3dCreateDynamicStr(const ars3d_char *p_c_str)
{
    // Return if no string is provided.
    if (p_c_str == ARS3D_NULL) return ARS3D_NULL;

    // Allocate memory for the new string.
    Ars3DDynamicStr *new_string = (Ars3DDynamicStr *)ars3dMalloc( ARS3D_SIZEOF(Ars3DDynamicStr) );

    // Allocate enough memory for the characters buffer.
    ars3d_size c_str_len    = ars3dStrLen(p_c_str);
    ars3d_char *char_buffer = (ars3d_char *)ars3dMalloc( ARS3D_SIZEOF(ars3d_char) * c_str_len + 1);

    // Memory is not enough.
    if (!new_string || !char_buffer)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    // Copy the content of p_c_str to the new buffer.
    ars3dMemCopy(char_buffer, p_c_str, c_str_len);
    char_buffer[c_str_len] = '\0'; // Terminate the string.

    // Initialize the rest of the new string instance.
    new_string->m_char_buffer   = char_buffer;
    new_string->m_length        = c_str_len;

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

    *p_str = ARS3D_NULL;
}



ars3d_char *ars3dGetCStr(Ars3DDynamicStr *p_str)
{
    return p_str->m_char_buffer;
}



ars3d_size ars3dLenDynamicStr(Ars3DDynamicStr *p_str)
{
    return p_str->m_length;
}



ars3d_int ars3dCompareDynamicStr(Ars3DDynamicStr *p_str1, Ars3DDynamicStr *p_str2)
{
    return ars3dStrCmp(p_str1->m_char_buffer, p_str2->m_char_buffer);
}

