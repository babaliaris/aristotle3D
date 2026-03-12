#ifndef ARS3D_VERTEX_ATTRIBUTE_H
#define ARS3D_VERTEX_ATTRIBUTE_H
#include <aristotle3D/core/ars3d_stdio.h>

#define ARS3D_VERTEX_ATTRIBUTE_FLOAT 0

typedef struct Ars3DVertexAttributes Ars3DVertexAttributes;

ARS3D_API Ars3DVertexAttributes *ars3dVertexAttributesCreate();

ARS3D_API ars3d_void ars3dVertexAttributesDestroy(Ars3DVertexAttributes **p_attr);

ARS3D_API ars3d_void ars3dVertexAttributesBind(Ars3DVertexAttributes *p_attr);

ARS3D_API ars3d_void ars3dVertexAttributesUnbind(Ars3DVertexAttributes *p_attr);

ARS3D_API ars3d_void ars3dVertexAttributesPush(Ars3DVertexAttributes *p_attr, const char *p_name, ars3d_int p_type, ars3d_uint p_count);

#endif
