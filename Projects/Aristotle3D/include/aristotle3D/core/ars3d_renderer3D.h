#ifndef ARS3D_RENDERER_3D_H
#define ARS3D_RENDERER_3D_H
#include <aristotle3D/core/ars3d_stdio.h>
#include <cglm/struct.h>

typedef struct Ars3DGameObject Ars3DGameObject;
typedef struct CameraSystem CameraSystem;
typedef struct Ars3DRenderer3D Ars3DRenderer3D;

ARS3D_API ars3d_void ars3dRenderer3DBeginScene(Ars3DRenderer3D *p_renderer, CameraSystem *p_camera);

ARS3D_API ars3d_void ars3dRenderer3DEndScene(Ars3DRenderer3D *p_renderer);

ARS3D_API ars3d_void ars3dRenderer3DPushGameObject(Ars3DRenderer3D *p_renderer, Ars3DGameObject *p_gameobject);

ARS3D_API ars3d_void ars3dRenderer3DSetAmbientLight(Ars3DRenderer3D *p_renderer, float p_strength, vec3s p_ambient);

ARS3D_API ars3d_void ars3dRenderer3DSetPointLight(
    Ars3DRenderer3D *p_renderer,
    float p_strenght,
    float p_constant,
    float p_linear,
    float p_quadratic,
    vec3s p_position,
    vec3s p_diffuse,
    vec3s p_specular
);

ARS3D_API ars3d_void ars3dRenderer3DSetSpotLight(
    Ars3DRenderer3D *p_renderer,
    float p_strenght,
    float p_constant,
    float p_linear,
    float p_quadratic,
    float p_inner_cos,
    float p_outer_cos,
    vec3s p_position,
    vec3s p_direction,
    vec3s p_diffuse,
    vec3s p_specular
);

#endif
