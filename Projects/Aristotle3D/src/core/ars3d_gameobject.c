#include <aristotle3D/core/ars3d_gameobject.h>
#include <aristotle3D/core/ars3d_debug.h>
#include <aristotle3D/core/opengl/ars3d_vertex_array.h>
#include <aristotle3D/core/opengl/ars3d_vertex_buffer.h>
#include <aristotle3D/core/opengl/ars3d_vertex_attributes.h>
#include <cglm/struct.h>

typedef struct Ars3DMesh
{
  Ars3DVertexArray      *m_vao;
  Ars3DVertexBuffer     *m_vbo;
  Ars3DVertexAttributes *m_attributes;
} Ars3DMesh;


typedef struct Ars3DMaterial
{
  vec3s m_ambient;
  vec3s m_diffuse;
  vec3s m_specular;

  float m_shininess;
} Ars3DMaterial;


typedef struct Ars3DGameObject
{
  vec3s     m_position;
  vec3s     m_rotation;
  vec3s     m_scale;
  mat4s     m_model;
  ars3d_int m_dirty_matrix;

  Ars3DMesh     *m_mesh;
  Ars3DMaterial *m_material;
} Ars3DGameObject;


ARS3D_API Ars3DGameObject *ars3dGameObjectCreate(vec3s p_position, vec3s p_rotation, vec3s p_scale)
{
  Ars3DGameObject *new_obj = (Ars3DGameObject *)ars3dMalloc( ARS3D_SIZEOF(Ars3DGameObject) );

  if (!new_obj)
  {
    ARS3D_WARN("Out of memory");
    return ARS3D_NULL;
  }

  new_obj->m_position     = p_position;
  new_obj->m_rotation     = p_rotation;
  new_obj->m_scale        = p_scale;
  new_obj->m_model        = glms_mat4_identity();
  new_obj->m_dirty_matrix = 1;
  new_obj->m_mesh         = ARS3D_NULL;
  new_obj->m_material     = ARS3D_NULL;

  return new_obj;
}

ARS3D_API ars3d_void ars3dGameObjectDestroy(Ars3DGameObject **p_obj)
{
  if (!p_obj || !(*p_obj))
  {
    ARS3D_WARN("Placeholder or the object that is pointing at, is NULL");
    return;
  }

  Ars3DGameObject *obj = *p_obj;

  if (obj->m_mesh) ars3dMeshDestroy(&obj->m_mesh);
  if (obj->m_material) ars3dMaterialDestroy(&obj->m_material);

  ars3dFree(*p_obj);

  *p_obj = ARS3D_NULL;
}

ARS3D_API ars3d_void ars3dGameObjectSetPosition(Ars3DGameObject *p_obj, vec3s p_position)
{

  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_position     = p_position;
  p_obj->m_dirty_matrix = 1;
}


ARS3D_API ars3d_void ars3dGameObjectSetRotation(Ars3DGameObject *p_obj, vec3s p_rotation)
{

  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_rotation     = p_rotation;
  p_obj->m_dirty_matrix = 1;
}


ARS3D_API ars3d_void ars3dGameObjectSetScale(Ars3DGameObject *p_obj, vec3s p_scale)
{

  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_scale        = p_scale;
  p_obj->m_dirty_matrix = 1;
}


ARS3D_API ars3d_void ars3dGameObjectSetMesh(Ars3DGameObject *p_obj, Ars3DMesh *p_mesh)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_mesh = p_mesh;
}


ARS3D_API ars3d_void ars3dGameObjectSetMaterial(Ars3DGameObject *p_obj, Ars3DMaterial *p_mat)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_material = p_mat;
}


ARS3D_API ars3d_void ars3dGameObjectUpdatePosition(Ars3DGameObject *p_obj, vec3s p_delta_pos)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_position     = glms_vec3_add(p_obj->m_position, p_delta_pos);
  p_obj->m_dirty_matrix = 1;
}


ARS3D_API ars3d_void ars3dGameObjectUpdateRotation(Ars3DGameObject *p_obj, vec3s p_delta_rot)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_rotation     = glms_vec3_add(p_obj->m_rotation, p_delta_rot);
  p_obj->m_dirty_matrix = 1;
}


ARS3D_API ars3d_void ars3dGameObjectUpdateScale(Ars3DGameObject *p_obj, vec3s p_delta_scale)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return;
  }

  p_obj->m_scale        = glms_vec3_add(p_obj->m_scale, p_delta_scale);
  p_obj->m_dirty_matrix = 1;
}

ARS3D_API vec3s ars3dGameObjectGetPosition(Ars3DGameObject *p_obj)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return (vec3s){{0.0f, 0.0f, 0.0f}};
  }

  return p_obj->m_position;
}


ARS3D_API vec3s ars3dGameObjectGetRotation(Ars3DGameObject *p_obj)
{

  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return (vec3s){{0.0f, 0.0f, 0.0f}};
  }

  return p_obj->m_rotation;
}


ARS3D_API vec3s ars3dGameObjectGetScale(Ars3DGameObject *p_obj)
{

  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return (vec3s){{1.0f, 1.0f, 1.0f}};
  }

  return p_obj->m_scale;
}

ARS3D_API mat4s ars3dGameObjectGetModelMatrix(Ars3DGameObject *p_obj)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return glms_mat4_identity();
  }

  if (p_obj->m_dirty_matrix)
  {
    p_obj->m_model = glms_mat4_identity();
    p_obj->m_model = glms_translate(p_obj->m_model, p_obj->m_position);
    p_obj->m_model = glms_rotate(p_obj->m_model, glm_rad(p_obj->m_rotation.raw[0]), (vec3s){{1.0f, 0.0f, 0.0f}});
    p_obj->m_model = glms_rotate(p_obj->m_model, glm_rad(p_obj->m_rotation.raw[1]), (vec3s){{0.0f, 1.0f, 0.0f}});
    p_obj->m_model = glms_rotate(p_obj->m_model, glm_rad(p_obj->m_rotation.raw[2]), (vec3s){{0.0f, 0.0f, 1.0f}});
    p_obj->m_model = glms_scale(p_obj->m_model, p_obj->m_scale);
    p_obj->m_dirty_matrix = 0;
  }

  return p_obj->m_model;
}


ARS3D_API Ars3DMesh *ars3dGameObjectGetMesh(Ars3DGameObject *p_obj)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return ARS3D_NULL;
  }

  return p_obj->m_mesh;
}

ARS3D_API Ars3DMaterial *ars3dGameObjectGetMaterial(Ars3DGameObject *p_obj)
{
  if (!p_obj)
  {
    ARS3D_WARN("Required parameters are not provided");
    return ARS3D_NULL;
  }

  return p_obj->m_material;
}



ARS3D_API Ars3DMesh *ars3dMeshCreate(void *p_data, ars3d_size p_size, Ars3DMeshAttribute p_attributes[ARS3D_MESH_MAX_ATTRIBUTES], ars3d_size p_attributes_length)
{
  if (!p_data || p_size <= 0)
  {
    ARS3D_WARN("Data are mandatory for building a mehs");
    return ARS3D_NULL;
  }

  if (p_attributes_length > ARS3D_MESH_MAX_ATTRIBUTES)
  {
    ARS3D_WARN("p_attributes_length must be less or equal to ARS3D_MESH_MAX_ATTRIBUTES");
    return ARS3D_NULL;
  }

  Ars3DMesh *new_mesh = (Ars3DMesh *)ars3dMalloc(ARS3D_SIZEOF(Ars3DMesh));

  if (!new_mesh)
  {
    ARS3D_WARN("Out Of Memory");
    return ARS3D_NULL;
  }

  new_mesh->m_vao         = ars3dVertexArrayCreate();
  new_mesh->m_vbo         = ars3dVertexBufferCreate();
  new_mesh->m_attributes  = ars3dVertexAttributesCreate();

  ars3dVertexArrayBind(new_mesh->m_vao);
  ars3dVertexBufferBind(new_mesh->m_vbo);

  ars3dVertexBufferData(new_mesh->m_vbo, p_size, p_data, ARS3D_VBO_STATIC_DRAW);

  for (ars3d_size i = 0; i < p_attributes_length; i++)
  {
    if ( ars3dStrCmp(p_attributes[i].m_name, "null") == 0 ) continue;

    ars3dVertexAttributesPush(
        new_mesh->m_attributes,
        p_attributes[i].m_name,
        p_attributes[i].m_type,
        p_attributes[i].m_count
    );
  }

  ars3dVertexAttributesBind(new_mesh->m_attributes);

  ars3dVertexArrayUnbind();
  ars3dVertexBufferUnbind();

  return new_mesh;
}


ARS3D_API ars3d_void ars3dMeshDestroy(Ars3DMesh **p_mesh)
{
  if (!p_mesh || !(*p_mesh))
  {
    ARS3D_WARN("Placeholder or the object itself is NULL");
    return;
  }

  Ars3DMesh *mesh = *p_mesh;

  ars3dVertexArrayDestroy(&mesh->m_vao);
  ars3dVertexBufferDestroy(&mesh->m_vbo);
  ars3dVertexAttributesDestroy(&mesh->m_attributes);

  ars3dFree(*p_mesh);

  *p_mesh = ARS3D_NULL;
}


ARS3D_API Ars3DVertexArray *ars3dMeshGetVertexArray(Ars3DMesh *p_mesh)
{
  if (!p_mesh)
  {
    ARS3D_WARN("Required parameters are not provided");
    return ARS3D_NULL;
  }

  return p_mesh->m_vao;
}


ARS3D_API Ars3DMaterial *ars3dMaterialCreate(vec3s p_ambient, vec3s p_diffuse, vec3s p_specular, float p_shininess)
{
  Ars3DMaterial *new_mat = (Ars3DMaterial *)ars3dMalloc(ARS3D_SIZEOF(Ars3DMaterial));

  if (!new_mat)
  {
    ARS3D_WARN("Out Of Memory");
    return ARS3D_NULL;
  }

  new_mat->m_ambient    = p_ambient;
  new_mat->m_diffuse    = p_diffuse;
  new_mat->m_specular   = p_specular;
  new_mat->m_shininess  = p_shininess;

  return new_mat;
}


ARS3D_API ars3d_void ars3dMaterialDestroy(Ars3DMaterial **p_mat)
{
  if (!p_mat || !(*p_mat))
  {
    ARS3D_WARN("Placeholder or the object itself is NULL");
    return;
  }

  ars3dFree(*p_mat);

  *p_mat = ARS3D_NULL;
}

