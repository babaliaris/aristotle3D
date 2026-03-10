#include "../core/unit_testing_framework.h"
#include <aristotle3D/core/data-structures/ars3d_list.h>

typedef struct ListContext
{
    ars3d_int m_sum;
    ars3d_int m_count;
    ars3d_int m_last_value;
} ListContext;

ars3d_void listDestructor(ars3d_void *p_data)
{
    ars3d_int *data = (ars3d_int *)p_data;

    ars3dFree(data);
}


ars3d_uchar listCmp(ars3d_void *p_data, ars3d_void *p_cmp_data)
{
    const ars3d_int *data = (ars3d_int *)p_data;
    const ars3d_int *comp = (ars3d_int *)p_cmp_data;

    if (*data == *comp) return 1;

    return 0;
}


ars3d_uchar listCallback(ars3d_void *p_data, ars3d_void *p_context)
{
    ars3d_int val       = *(ars3d_int*)p_data;
    ListContext* ctx    = (ListContext*)p_context;

    ctx->m_sum += val;
    ctx->m_count++;
    ctx->m_last_value = val;

    return 0; // Continue looping the entire list.
}


ARS3D_DECLARE_UNIT_TEST(LinkedList, create_and_destroy)
{
    Ars3DList *new_list = ars3dListCreate();

    ARS3D_UNIT_TEST_ASSERT(new_list, "List should have been created successfully");
    ARS3D_UNIT_TEST_ASSERT(ars3dListGetSize(new_list) == 0, "List should be empty at creation");

    ars3dListDestroy(&new_list, ARS3D_NULL);

    ARS3D_UNIT_TEST_ASSERT(!new_list, "List should have been NULLED after destruction.");
}


ARS3D_DECLARE_UNIT_TEST(LinkedList, populate_and_destruct)
{
    Ars3DList *new_list = ars3dListCreate();

    ars3d_int *data = ARS3D_NULL;

    for (ars3d_int i = 0; i < 10; i++)
    {
        data    = (ars3d_int *)ars3dMalloc( ARS3D_SIZEOF(ars3d_int) );
        *data   = i;
        ars3dListAppend(new_list, (ars3d_void *)data);
    }

    ARS3D_UNIT_TEST_ASSERT(ars3dListGetSize(new_list) == 10, "The list size should be 10");

    for (ars3d_int i = 0; i < 10; i++)
    {
        data = (ars3d_int *)ars3dListGetAt(new_list, i);
        ARS3D_UNIT_TEST_ASSERT(*data == i);
    }

    ars3dListDestroy(&new_list, listDestructor);
}


ARS3D_DECLARE_UNIT_TEST(LinkedList, find_item)
{
    Ars3DList *new_list = ars3dListCreate();

    ars3d_int *data = ARS3D_NULL;

    for (ars3d_int i = 0; i < 10; i++)
    {
        data    = (ars3d_int *)ars3dMalloc( ARS3D_SIZEOF(ars3d_int) );
        *data   = i;
        ars3dListAppend(new_list, (ars3d_void *)data);
    }

    ARS3D_UNIT_TEST_ASSERT(ars3dListGetSize(new_list) == 10, "The list size should be 10");


    for (ars3d_int i = 0; i < 10; i++)
    {
        *data = i;

        data = (ars3d_int *)ars3dListFind(new_list, listCmp, (ars3d_void *)data);

        ARS3D_UNIT_TEST_ASSERT(*data == i);
    }

    ars3dListDestroy(&new_list, listDestructor);
}


ARS3D_DECLARE_UNIT_TEST(LinkedList, loop_through)
{
    Ars3DList *new_list = ars3dListCreate();
    ars3d_int *data     = ARS3D_NULL;

    ListContext ctx;
    ctx.m_sum           = 0;
    ctx.m_count         = 0;
    ctx.m_last_value    = 0;

    for (ars3d_int i = 0; i < 10; i++)
    {
        data    = (ars3d_int *)ars3dMalloc( ARS3D_SIZEOF(ars3d_int) );
        *data   = i;
        ars3dListAppend(new_list, (ars3d_void *)data);
    }

    ARS3D_UNIT_TEST_ASSERT(ars3dListGetSize(new_list) == 10, "The list size should be 10");

    // Test Regular Order (0 -> 9)
    ars3dListLoopThrough(new_list, listCallback, (ars3d_void *)&ctx, 0);
    ARS3D_UNIT_TEST_ASSERT(ctx.m_count == 10, "The ctx.m_count should be 10, to indicate that we looped through all elements.");
    ARS3D_UNIT_TEST_ASSERT(ctx.m_sum == 45, "The ctx.m_sum should be 45, since sum(0 to 9) = 45.");
    ARS3D_UNIT_TEST_ASSERT(ctx.m_last_value == 9, "The ctx.m_last_value should be 9");

    // Test Reversed Order (9 -> 0)
    ctx.m_sum = 0; ctx.m_count = 0; ctx.m_last_value = -1; // Reset the context.
    ars3dListLoopThrough(new_list, listCallback, &ctx, 1);
    ARS3D_UNIT_TEST_ASSERT(ctx.m_count == 10, "Should have looped 10 times (reversed)");
    ARS3D_UNIT_TEST_ASSERT(ctx.m_last_value == 0, "Last value in reversed loop should be 0");

    ars3dListDestroy(&new_list, listDestructor);
}
