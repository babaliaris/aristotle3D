#ifndef ARS3D_STRING_H
#define ARS3D_STRING_H
#include <aristotle3D/core/ars3d_stdio.h>


/**
 * @file
 * @brief ASCII only Dynamic (Heap Allocated) Strings.
 */


typedef struct Ars3DDynamicStr Ars3DDynamicStr;


/**
 * @brief Create a new Ars3DDynamicStr
 * 
 * @param p_c_str A regular c string.
 * 
 * @returns The newly created Ars3DDynamicStr instance.
 */
ARS3D_API Ars3DDynamicStr *ars3dCreateDynamicStr(const ars3d_char *p_c_str);


/**
 * @brief Destroy a Ars3DDynamicStr string/
 * 
 * Destroys the string and sets the user's
 * variable to NULL.
 * 
 * @param p_str The use's instance placeholder.
 */
ARS3D_API ars3d_void ars3dDestroyDynamicStr(Ars3DDynamicStr **p_str);



/**
 * @brief Get the C string (The buffer of characters).
 * 
 * @param p_str The Ars3DDynamicStr string.
 * 
 * @returns The starting memory address of the char buffer.
 */
ARS3D_API ars3d_char *ars3dGetCStr(Ars3DDynamicStr *p_str);



/**
 * @brief Calculates the length of a Ars3DDynamicStr.
 * 
 * @returns The length of the Ars3DDynamicStr, excluding '\0'
 */
ARS3D_API ars3d_size ars3dLenDynamicStr(Ars3DDynamicStr *p_str);



/**
 * @brief Compare two strings.
 * 
 * @param p_str1 The first string.
 * @param p_str2 The second string.
 * 
 * @returns 1 if p_str1 equals
 */
ARS3D_API ars3d_uchar ars3dCompareDynamicStr(Ars3DDynamicStr *p_str1, Ars3DDynamicStr *p_str2);

#endif