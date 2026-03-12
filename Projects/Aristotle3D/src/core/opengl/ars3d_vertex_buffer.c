#include <aristotle3D/core/opengl/ars3d_vertex_buffer.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <glad/gl.h>

typedef struct Ars3DVertexBuffer
{
    ars3d_uint m_vbo;
} Ars3DVertexBuffer;


Ars3DVertexBuffer *ars3dVertexBufferCreate()
{
    Ars3DVertexBuffer *new_vbo = (Ars3DVertexBuffer *)ars3dMalloc( ARS3D_SIZEOF(Ars3DVertexBuffer) );

    if (!new_vbo)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    glGenBuffers(1, &new_vbo->m_vbo);

    return new_vbo;
}


ars3d_void ars3dVertexBufferDestroy(Ars3DVertexBuffer **p_vbo)
{
    if (!p_vbo || !(*p_vbo))
    {
        ARS3D_WARN("Placeholder or its pointing address is NULL");
        return;
    }

    glDeleteBuffers(1, &(*p_vbo)->m_vbo);

    ars3dFree(*p_vbo);

    *p_vbo = ARS3D_NULL;
}


ars3d_void ars3dVertexBufferBind(Ars3DVertexBuffer *p_vbo)
{
    if (!p_vbo)
    {
        ARS3D_WARN("Required arguments are NULL");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, p_vbo->m_vbo);
}


ars3d_void ars3dVertexBufferUnbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


ars3d_void ars3dVertexBufferData(Ars3DVertexBuffer *p_vbo, ars3d_size p_size, ars3d_void *p_data, ars3d_int p_draw_hint)
{
    if (!p_vbo)
    {
        ARS3D_WARN("Required arguments are NULL");
        return;
    }

    GLenum draw_hint = GL_STATIC_DRAW;

    switch (p_draw_hint)
    {
        case ARS3D_VBO_STREAM_DRAW:
            draw_hint = GL_STREAM_DRAW;
            break;

        case ARS3D_VBO_STREAM_READ:
            draw_hint = GL_STREAM_READ;
            break;

        case ARS3D_VBO_STREAM_COPY:
            draw_hint = GL_STREAM_COPY;
            break;

        case ARS3D_VBO_STATIC_DRAW:
            draw_hint = GL_STATIC_DRAW;
            break;

        case ARS3D_VBO_STATIC_READ:
            draw_hint = GL_STATIC_READ;
            break;

        case ARS3D_VBO_STATIC_COPY:
            draw_hint = GL_STATIC_COPY;
            break;

        case ARS3D_VBO_DYNAMIC_DRAW:
            draw_hint = GL_DYNAMIC_DRAW;
            break;

        case ARS3D_VBO_DYNAMIC_READ:
            draw_hint = GL_DYNAMIC_READ;
            break;

        case ARS3D_VBO_DYNAMIC_COPY:
            draw_hint = GL_DYNAMIC_COPY;
            break;

        default:
            draw_hint = GL_STATIC_DRAW;
            break;
    }

    glBufferData(GL_ARRAY_BUFFER, p_size, p_data, draw_hint);
}
