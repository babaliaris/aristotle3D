#ifndef ARS3D_GAMEOBJECT_H
#define ARS3D_GAMEOBJECT_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <cglm/struct.h>

typedef struct Ars3DGameObject Ars3DGameObject;

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

ARS3D_API ars3d_void ars3dGameObjectUpdatePosition(Ars3DGameObject *p_obj, vec3s p_delta_pos);
ARS3D_API ars3d_void ars3dGameObjectUpdateRotation(Ars3DGameObject *p_obj, vec3s p_delta_rot);
ARS3D_API ars3d_void ars3dGameObjectUpdateScale(Ars3DGameObject *p_obj, vec3s p_delta_scale);

ARS3D_API vec3s ars3dGameObjectGetPosition(Ars3DGameObject *p_obj);
ARS3D_API vec3s ars3dGameObjectGetRotation(Ars3DGameObject *p_obj);
ARS3D_API vec3s ars3dGameObjectGetScale(Ars3DGameObject *p_obj);

ARS3D_API mat4s ars3dGameObjectGetModelMatrix(Ars3DGameObject *p_obj);

#endif
