#include <aristotle3D/core/opengl/ars3d_vertex_attributes.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <aristotle3D/core/ars3d_string.h>
#include <aristotle3D/core/data-structures/ars3d_list.h>
#include <glad/gl.h>

#define ARS3D_VERTEX_ATTRIBUTES_TYPE 0
#define ARS3D_VERTEX_ATTRIB_TYPE 1

typedef struct Ars3DVertexAttrib
{
    ars3d_int           m_type;
    Ars3DDynamicStr *   m_name;
    ars3d_uint          m_index;
    ars3d_uint          m_count;
    ars3d_int           m_size;
    GLenum              m_size_type;
    ars3d_size          m_offset;
    
} Ars3DVertexAttrib;


typedef struct Ars3DVertexAttributes
{
    ars3d_int   m_type;
    Ars3DList * m_attributes;
    ars3d_size  m_stride;
} Ars3DVertexAttributes;



ARS3D_API Ars3DVertexAttributes *ars3dVertexAttributesCreate()
{
    Ars3DVertexAttributes *new_attribs = (Ars3DVertexAttributes *)ars3dMalloc( ARS3D_SIZEOF(Ars3DVertexAttributes) );

    if (!new_attribs)
    {
        ARS3D_WARN("Not enough memory");
        return ARS3D_NULL;
    }

    new_attribs->m_type         = ARS3D_VERTEX_ATTRIBUTES_TYPE;
    new_attribs->m_attributes   = ars3dListCreate();
    new_attribs->m_stride       = 0;

    if (!new_attribs->m_attributes)
    {
        ARS3D_WARN("Not enough memory");
        ars3dFree(new_attribs);
        return ARS3D_NULL;
    }

    return new_attribs;
}


ars3d_void attribsListUserDestructorkCB(ars3d_void *p_data)
{
    Ars3DVertexAttrib *attribute = (Ars3DVertexAttrib *)p_data;

    ARS3D_ASSERT(attribute->m_type == ARS3D_VERTEX_ATTRIB_TYPE);

    ars3dDestroyDynamicStr(&attribute->m_name);

    ars3dFree(attribute);
}


ars3d_void ars3dVertexAttributesDestroy(Ars3DVertexAttributes **p_attr)
{
    if (!p_attr || !(*p_attr))
    {
        ARS3D_WARN("Placeholder or its pointing address is NULL");
        return;
    }

    ars3dListDestroy(&(*p_attr)->m_attributes, attribsListUserDestructorkCB);

    ars3dFree(*p_attr);

    *p_attr = ARS3D_NULL;
}



ars3d_uchar atrribBindLoopThrough(ars3d_void *p_data, ars3d_void *p_context)
{
    Ars3DVertexAttrib *attribute = (Ars3DVertexAttrib *)p_data;
    Ars3DVertexAttributes *attributes = (Ars3DVertexAttributes *)p_context;

    ARS3D_ASSERT(attribute->m_type == ARS3D_VERTEX_ATTRIB_TYPE);
    ARS3D_ASSERT(attributes->m_type == ARS3D_VERTEX_ATTRIBUTES_TYPE);

    glVertexAttribPointer(
        attribute->m_index,
        attribute->m_count,
        attribute->m_size_type,
        GL_FALSE,
        attributes->m_stride,
        (const ars3d_void *)attribute->m_offset);

    glEnableVertexAttribArray(attribute->m_index);

    return 0; //Loop through the entire list.
}



ARS3D_API ars3d_void ars3dVertexAttributesBind(Ars3DVertexAttributes *p_attr)
{
    if (!p_attr)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    ars3dListLoopThrough(p_attr->m_attributes, atrribBindLoopThrough, (ars3d_void *)p_attr, 0);
}


ars3d_uchar atrribUnBindLoopThrough(ars3d_void *p_data, ars3d_void *p_context)
{
    Ars3DVertexAttrib *attribute = (Ars3DVertexAttrib *)p_data;
    ARS3D_UNUSED(p_context);

    ARS3D_ASSERT(attribute->m_type == ARS3D_VERTEX_ATTRIB_TYPE);

    glDisableVertexAttribArray(attribute->m_index);

    return 0; //Loop through the entire list.
}


ARS3D_API ars3d_void ars3dVertexAttributesUnbind(Ars3DVertexAttributes *p_attr)
{
    if (!p_attr)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    ars3dListLoopThrough(p_attr->m_attributes, atrribUnBindLoopThrough, ARS3D_NULL, 0);
}



static ars3d_size getGLTypeSize(GLenum type)
{
    switch(type)
    {
        case GL_FLOAT: return ARS3D_SIZEOF(ars3d_float);
        case GL_INT  : return ARS3D_SIZEOF(ars3d_int);
        default:
        {
            ARS3D_ASSERT(0, "This line should not be reached");
        }
    }
    return 0;
}



ars3d_void ars3dVertexAttributesPush(Ars3DVertexAttributes *p_attr, const char *p_name, ars3d_int p_type, ars3d_uint p_count)
{
    if (!p_attr || !p_name)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    ARS3D_ASSERT(
        p_type == ARS3D_VERTEX_ATTRIBUTE_FLOAT
    );

    Ars3DVertexAttrib *new_attribute = (Ars3DVertexAttrib *)ars3dMalloc( ARS3D_SIZEOF(Ars3DVertexAttrib) );

    if (!new_attribute)
    {
        ARS3D_WARN("Not enough memory");
        return;
    }

    GLenum attrib_type = GL_FLOAT;

    switch (p_type)
    {
        case ARS3D_VERTEX_ATTRIBUTE_FLOAT:
            attrib_type = GL_FLOAT;
            break;
        
        default:
            attrib_type = GL_FLOAT;
            break;
    }

    new_attribute->m_type       = ARS3D_VERTEX_ATTRIB_TYPE;
    new_attribute->m_name       = ars3dCreateDynamicStr(p_name);
    new_attribute->m_index      = ars3dListGetSize(p_attr->m_attributes);
    new_attribute->m_size       = getGLTypeSize(attrib_type) * p_count;
    new_attribute->m_size_type  = attrib_type;
    new_attribute->m_count      = p_count;
    new_attribute->m_offset     = p_attr->m_stride;

    p_attr->m_stride           += new_attribute->m_size;

    ars3dListAppend(p_attr->m_attributes, (ars3d_void *)new_attribute);
}
