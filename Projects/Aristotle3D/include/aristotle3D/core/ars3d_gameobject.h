#ifndef ARS3D_GAMEOBJECT_H
#define ARS3D_GAMEOBJECT_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <cglm/struct.h>

typedef struct Ars3DVertexArray Ars3DVertexArray;
typedef struct Ars3DMesh        Ars3DMesh;
typedef struct Ars3DMaterial    Ars3DMaterial;
typedef struct Ars3DGameObject  Ars3DGameObject;

typedef struct Ars3DMeshAttribute
{
  const char *m_name;
  ars3d_int   m_type;
  ars3d_uint  m_count;
} Ars3DMeshAttribute;

#define ARS3D_MESH_MAX_ATTRIBUTES 3


/**
 * @brief Creates a new game object.
 * 
 * @returns The newly created instance.
 */
ARS3D_API Ars3DGameObject *ars3dGameObjectCreate(vec3s p_position, vec3s p_rotation, vec3s p_scale);

/**
 * @brief Destroy a gameobject.
 *
 * @param p_obj The user placeholder of the gameobject memory.
 */
ARS3D_API ars3d_void ars3dGameObjectDestroy(Ars3DGameObject **p_obj);

ARS3D_API ars3d_void ars3dGameObjectSetPosition(Ars3DGameObject *p_obj, vec3s p_position);
ARS3D_API ars3d_void ars3dGameObjectSetRotation(Ars3DGameObject *p_obj, vec3s p_rotation);
ARS3D_API ars3d_void ars3dGameObjectSetScale(Ars3DGameObject *p_obj, vec3s p_scale);
ARS3D_API ars3d_void ars3dGameObjectSetMesh(Ars3DGameObject *p_obj, Ars3DMesh *p_mesh);
ARS3D_API ars3d_void ars3dGameObjectSetMaterial(Ars3DGameObject *p_obj, Ars3DMaterial *p_mat);

ARS3D_API ars3d_void ars3dGameObjectUpdatePosition(Ars3DGameObject *p_obj, vec3s p_delta_pos);
ARS3D_API ars3d_void ars3dGameObjectUpdateRotation(Ars3DGameObject *p_obj, vec3s p_delta_rot);
ARS3D_API ars3d_void ars3dGameObjectUpdateScale(Ars3DGameObject *p_obj, vec3s p_delta_scale);

ARS3D_API vec3s ars3dGameObjectGetPosition(Ars3DGameObject *p_obj);
ARS3D_API vec3s ars3dGameObjectGetRotation(Ars3DGameObject *p_obj);
ARS3D_API vec3s ars3dGameObjectGetScale(Ars3DGameObject *p_obj);
ARS3D_API Ars3DMesh *ars3dGameObjectGetMesh(Ars3DGameObject *p_obj);
ARS3D_API Ars3DMaterial *ars3dGameObjectGetMaterial(Ars3DGameObject *p_obj);

ARS3D_API mat4s ars3dGameObjectGetModelMatrix(Ars3DGameObject *p_obj);

ARS3D_API Ars3DMesh *ars3dMeshCreate(void *p_data, ars3d_size p_size, Ars3DMeshAttribute p_attributes[ARS3D_MESH_MAX_ATTRIBUTES], ars3d_size p_attributes_length);
ARS3D_API ars3d_void ars3dMeshDestroy(Ars3DMesh **p_mesh);
ARS3D_API Ars3DVertexArray *ars3dMeshGetVertexArray(Ars3DMesh *p_mesh);

ARS3D_API Ars3DMaterial *ars3dMaterialCreate(vec3s p_ambient, vec3s p_diffuse, vec3s p_specular, float p_shininess);
ARS3D_API ars3d_void ars3dMaterialDestroy(Ars3DMaterial **p_mat);

#endif
