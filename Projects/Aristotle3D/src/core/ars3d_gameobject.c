#include <aristotle3D/core/ars3d_gameobject.h>
#include <aristotle3D/core/ars3d_debug.h>


typedef struct Ars3DGameObject
{
  vec3s m_position;
  vec3s m_rotation;
  vec3s m_scale;
  mat4s m_model;

  ars3d_int m_dirty_matrix;
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

  return new_obj;
}

ARS3D_API ars3d_void ars3dGameObjectDestroy(Ars3DGameObject **p_obj)
{
  if (!p_obj || !(*p_obj))
  {
    ARS3D_WARN("Placeholder or the object that is pointing at, is NULL");
    return;
  }

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

