#ifndef ARS3D_LIST_H
#define ARS3D_LIST_H
#include <aristotle3D/core/ars3d_stdio.h>

typedef struct Ars3DList Ars3DList;

/**
 * @brief User difined list data comparator.
 * 
 * The user should implement a function of this signature
 * to compare the data provided through this function with
 * their own logic. The funtion should return 0 if the comparison
 * fails, true otherwise.
 * 
 * @param p_data The data provided by the list.
 * 
 * @returns 0 if the comparison fails, true otherwise.
 */
typedef ars3d_uchar (*Ars3DListUserCmpFN)(ars3d_void *p_data, ars3d_void *p_cmp_data);


typedef ars3d_void (*Ars3DListUserDestructorkFN)(ars3d_void *p_data);


typedef ars3d_uchar (*Ars3DListUserCallbackFN)(ars3d_void *p_data, ars3d_void *p_context);


ARS3D_API Ars3DList *ars3dListCreate();

ARS3D_API ars3d_void ars3dListDestroy(Ars3DList **p_list, Ars3DListUserDestructorkFN p_userDestructor);

ARS3D_API ars3d_void ars3dListAppend(Ars3DList *p_list, ars3d_void *p_data);

ARS3D_API ars3d_void *ars3dListFind(Ars3DList *p_list, Ars3DListUserCmpFN p_compareCB, ars3d_void *p_cmp_data);

ARS3D_API ars3d_size ars3dListGetSize(Ars3DList *p_list);

ARS3D_API ars3d_void *ars3dListGetAt(Ars3DList *p_list, ars3d_size p_index);

ARS3D_API ars3d_void ars3dListLoopThrough(Ars3DList *p_list, Ars3DListUserCallbackFN p_userCB, ars3d_void *p_context, ars3d_uchar p_reversed);


#endif