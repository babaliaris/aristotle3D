#ifndef ARS3D_VERTEX_ATTRIBUTE_H
#define ARS3D_VERTEX_ATTRIBUTE_H
#include <aristotle3D/core/ars3d_stdio.h>

/**
 * Attribute Types.
 */
#define ARS3D_VERTEX_ATTRIBUTE_FLOAT 0


typedef struct Ars3DVertexAttributes Ars3DVertexAttributes;


/**
 * @brief Create a vertex attributes object.
 * 
 * @returns The newly created isnstace.
 */
ARS3D_API Ars3DVertexAttributes *ars3dVertexAttributesCreate();


/**
 * @brief Destroy a vertex attributes object.
 * 
 * @param p_attr The user's placeholder that holds the memory address of the instance.
 */
ARS3D_API ars3d_void ars3dVertexAttributesDestroy(Ars3DVertexAttributes **p_attr);


/**
 * @brief Bind vertex attributes.
 * 
 * This function will calculate all the offsets required by
 * each attribute and enable / bind each one accordingly.
 * This should be called AFTER an Ars3DVertexArray object 
 * AND an Ars3DVertexBuffer object have been bound
 * since it creates the links of the attributes that
 * associate with the currently bound Ars3DVertexBuffer object
 * and these LINKS are being STORED in the currently bound
 * Ars3DVertexArray object.
 * 
 * @param p_attr The vertex attributes instance.
 */
ARS3D_API ars3d_void ars3dVertexAttributesBind(Ars3DVertexAttributes *p_attr);


/**
 * @brief Unbind vertex attributes.
 * 
 * This just disables / unbinds each vertex attribute.
 * 
 * @param p_attr The vertex attributes instance.
 */
ARS3D_API ars3d_void ars3dVertexAttributesUnbind(Ars3DVertexAttributes *p_attr);


/**
 * @brief Push an new attribute.
 * 
 * This pushes a new attribute to he attributes list.
 * You should PUSH all REQUIRED attribute layouts
 * BEFORE calling ars3dVertexAttributesBind().
 * 
 * @param p_attr The vertex attributes instance.
 * @param p_name A name for this attrib layout (for debugging).
 * @param p_type The attribute type.
 * @param p_count The count of attribute types (1, 2, 3 OR 4)
 */
ARS3D_API ars3d_void ars3dVertexAttributesPush(Ars3DVertexAttributes *p_attr, const char *p_name, ars3d_int p_type, ars3d_uint p_count);

#endif
