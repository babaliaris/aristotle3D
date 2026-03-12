#include <aristotle3D/core/opengl/ars3d_vertex_array.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <glad/gl.h>

typedef struct Ars3DVertexArray
{
    ars3d_uint m_vao;
} Ars3DVertexArray;

Ars3DVertexArray *ars3dVertexArrayCreate()
{
    Ars3DVertexArray *new_vao = (Ars3DVertexArray *)ars3dMalloc( ARS3D_SIZEOF(Ars3DVertexArray) );

    if (!new_vao)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }
    
    glGenVertexArrays(1, &new_vao->m_vao);

    return new_vao;
}


ars3d_void ars3dVertexArrayDestroy(Ars3DVertexArray **p_vao)
{
    if (!p_vao || !(*p_vao))
    {
        ARS3D_WARN("Placeholder or its pointing address is NULL");
        return;
    }

    glDeleteVertexArrays(1, &(*p_vao)->m_vao);

    ars3dFree(*p_vao);

    *p_vao = ARS3D_NULL;
}


ars3d_void ars3dVertexArrayBind(Ars3DVertexArray *p_vao)
{
    if (!p_vao)
    {
        ARS3D_WARN("Required arguments are NULL");
        return;
    }

    glBindVertexArray(p_vao->m_vao);
}


ars3d_void ars3dVertexArrayUnbind()
{
    glBindVertexArray(0);
}
