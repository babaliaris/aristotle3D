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


/**
 * @brief User Destructor function callback signature.
 * 
 * Upon deleting the list, the user SHOULD provide a destruction
 * callback function, that is called for each element in the list
 * while the list is getting destroyed. This allows the user to
 * clean up their data allocations.
 * 
 * @param p_data The data provided by the list.
 */
typedef ars3d_void (*Ars3DListUserDestructorkFN)(ars3d_void *p_data);


/**
 * @brief User's Loop Through function.
 * 
 * This is being used by the loop through functionality of the Ars3DList
 * and it is called for each element in the list, until it returns a value
 * different than 0.
 * 
 * @param p_data The current element user's data.
 * @param p_context The user's context, that was given by the user.
 * 
 * @returns 0 = continue looping, otherwise break the loop.
 */
typedef ars3d_uchar (*Ars3DListUserLoopThroughFN)(ars3d_void *p_data, ars3d_void *p_context);



/**
 * @brief Create a new Ars3DList.
 * 
 * @returns The newly created instance.
 */
ARS3D_API Ars3DList *ars3dListCreate();


/**
 * @brief Destroy a list [ O(n) ].
 * 
 * This function will make sure the Ars3DList is destroyed, but at the same time
 * it will call Ars3DListUserDestructorkFN for EACH ELEMENT in the list. The user
 * can use this callback to cleanup their data allocations.
 * 
 * @param p_list The user's placeholder that holds the mem address of the list instance.
 * @param p_userDestructor The users Ars3DListUserDestructorkFN callback function.
 */
ARS3D_API ars3d_void ars3dListDestroy(Ars3DList **p_list, Ars3DListUserDestructorkFN p_userDestructor);


/**
 * @brief Append a new element in the list [ O(1) ].
 * 
 * @param p_list The list instance.
 * @param p_data The user's data to be appended into the list.
 */
ARS3D_API ars3d_void ars3dListAppend(Ars3DList *p_list, ars3d_void *p_data);


/**
 * @brief Find an element in the list [ O(n) ].
 * 
 * This function will return the data of the found element.
 * The found element is being determined by the return value
 * of the Ars3DListUserCmpFN callback.
 * 
 * @param p_list The list instance.
 * @param p_compareCB The user's Ars3DListUserCmpFN function callback.
 * @param p_cmp_data The extra data to be passed from the user to the Ars3DListUserCmpFN callback.
 * 
 * @returns The data of the found element.
 */
ARS3D_API ars3d_void *ars3dListFind(Ars3DList *p_list, Ars3DListUserCmpFN p_compareCB, ars3d_void *p_cmp_data);


/**
 * @brief Get the size of the list [ O(1) ].
 * 
 * @param p_list The list instance.
 * 
 * @returns The size of the list (How many elements it contains).
 */
ARS3D_API ars3d_size ars3dListGetSize(Ars3DList *p_list);


/**
 * @brief Get an element at a known position / index [ O(n) ].
 * 
 * This function SHOULD ONLY be used when the index
 * is known. DO NOT call this function inside a LOOP!
 * Calling this function inside a loop, will cause
 * a search of O(m * n) where m is the loop size 
 * while n is the list size.
 * 
 * @param p_list The instance of the list.
 * @param p_index The index/position on the list.
 * 
 * @returns The data of the element at position = p_index
 */
ARS3D_API ars3d_void *ars3dListGetAt(Ars3DList *p_list, ars3d_size p_index);


/**
 * @brief Loop through the list [ O(n) ].
 * 
 * This function will loop through the list elements
 * and call the Ars3DListUserLoopThroughFN function for
 * each element. If that function returns 0, the loop
 * continues, otherwise the loop will stop / break.
 * 
 * @param p_list The instance of the list.
 * @param p_userCB The Ars3DListUserLoopThroughFN callback function.
 * @param p_context The extra user's context object.
 * @param p_reversed if 0, the loop happens in the same order the items where appended, otherwise the loop iteration is reversed.
 */
ARS3D_API ars3d_void ars3dListLoopThrough(Ars3DList *p_list, Ars3DListUserLoopThroughFN p_userCB, ars3d_void *p_context, ars3d_uchar p_reversed);


#endif