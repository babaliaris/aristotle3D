#include <aristotle3D/core/data-structures/ars3d_list.h>
#include <aristotle3D/core/ars3d_debug.h>


typedef struct Ars3DListNode
{
    ars3d_void *            m_data;
    struct Ars3DListNode *  m_next;
    struct Ars3DListNode *  m_prev;

} Ars3DListNode;


typedef struct Ars3DList
{
    ars3d_size      m_size;
    Ars3DListNode * m_head;
    Ars3DListNode * m_tail;
} Ars3DList;


Ars3DList *ars3dListCreate()
{
    Ars3DList *new_list = (Ars3DList *)ars3dMalloc( ARS3D_SIZEOF(Ars3DList) );

    if (!new_list)
    {
        ARS3D_WARN("Not enough memory.");
        return ARS3D_NULL;
    }

    new_list->m_size = 0;
    new_list->m_head = ARS3D_NULL;
    new_list->m_tail = ARS3D_NULL;

    return new_list;
}



ars3d_void ars3dListDestroy(Ars3DList **p_list, Ars3DListUserDestructorkFN p_userDestructor)
{
    if (!p_list || !(*p_list))
    {
        ARS3D_WARN("You provided a placeholder that is NULL or it points to a NULL address.");
        return;
    }

    Ars3DListNode *current  = (*p_list)->m_head;
    Ars3DListNode *temp     = ARS3D_NULL;

    while (current)
    {
        temp    = current;
        current = current->m_next;

        if (p_userDestructor) p_userDestructor(temp->m_data);

        ars3dFree(temp);
    }

    ars3dFree(*p_list);

    *p_list = ARS3D_NULL;
}



ars3d_void ars3dListAppend(Ars3DList *p_list, ars3d_void *p_data)
{
    if (!p_list || !p_data)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    // Try to instantiate the new node.
    Ars3DListNode *new_node = (Ars3DListNode *)ars3dMalloc( ARS3D_SIZEOF(Ars3DListNode) );

    // Not enough memory.
    if (!new_node)
    {
        ARS3D_WARN("Not enough memory.");
        return;
    }

    // Initialize the new node.
    new_node->m_data = p_data;
    new_node->m_next = ARS3D_NULL;
    new_node->m_prev = ARS3D_NULL;

    // List is empty. Append the very first node.
    if (p_list->m_head == ARS3D_NULL && p_list->m_tail == ARS3D_NULL)
    {
        ARS3D_ASSERT(p_list->m_size == 0, "The list size should be zero at this point. Check initialization.");
        p_list->m_head = new_node;
        p_list->m_tail = new_node;
    }

    // There is only one element in the list.
    else if (p_list->m_head && p_list->m_head == p_list->m_tail)
    {
        ARS3D_ASSERT(p_list->m_size == 1, "The list size should have one element. Check previous connection step.");
        
        p_list->m_tail          = new_node;
        p_list->m_head->m_next  = new_node;
        p_list->m_tail->m_prev  = p_list->m_head;

        ARS3D_ASSERT(p_list->m_head->m_prev == ARS3D_NULL, "The previous node of the Head of the List should be NULL.");
        ARS3D_ASSERT(p_list->m_tail->m_next == ARS3D_NULL, "The next node of the Tail of the List should be NULL.");
    }

    // More than 1 elements. Append the new node.
    else
    {
        ARS3D_ASSERT(p_list->m_size > 1, "The list size should be greater than 1. Check previous connection step.");

        p_list->m_tail->m_next  = new_node;
        new_node->m_prev        = p_list->m_tail;
        p_list->m_tail          = new_node;

        ARS3D_ASSERT(new_node->m_next == ARS3D_NULL, "The new node's next, should point to NULL");
    }

    // Increase the size of the list.
    p_list->m_size++;
}



ars3d_void *ars3dListFind(Ars3DList *p_list, Ars3DListUserCmpFN p_compareCB, ars3d_void *p_cmp_data)
{
    if (!p_list || !p_compareCB)
    {
        ARS3D_WARN("Required parameters are NULL");
        return ARS3D_NULL;
    }

    Ars3DListNode *current  = p_list->m_head;
    ars3d_void *data        = ARS3D_NULL;

    while (current)
    {
        if ( p_compareCB(current->m_data, p_cmp_data) )
        {
            data = current->m_data;
            break;
        }

        current = current->m_next;
    }

    return data;
}



ars3d_size ars3dListGetSize(Ars3DList *p_list)
{
    if (!p_list) return 0;

    return p_list->m_size;
}



ars3d_void *ars3dListGetAt(Ars3DList *p_list, ars3d_size p_index)
{
    if (!p_list)
    {
        ARS3D_WARN("Required parameters are NULL");
        return ARS3D_NULL;
    }

    Ars3DListNode *current  = p_list->m_head;
    ars3d_void *data        = ARS3D_NULL;
    ars3d_size position     = 0;

    while (current)
    {
        if ( position == p_index)
        {
            data = current->m_data;
            break;
        }

        current = current->m_next;
        position++;
    }

    return data;
}


ars3d_void ars3dListLoopThrough(Ars3DList *p_list, Ars3DListUserLoopThroughFN p_userCB, ars3d_void *p_context, ars3d_uchar p_reversed)
{
    if (!p_list || !p_userCB)
    {
        ARS3D_WARN("Required parameters are NULL");
        return;
    }

    Ars3DListNode *current    = p_reversed == 0 ? p_list->m_head : p_list->m_tail;
    Ars3DListNode *next_node  = ARS3D_NULL;

    while (current)
    {
        next_node = p_reversed == 0 ? current->m_next : current->m_prev;
        if ( p_userCB(current->m_data, p_context) ) break;
        current   = next_node;
    }
}


ars3d_void *ars3dListRemove(Ars3DList *p_list, Ars3DListUserCmpFN p_compareCB, ars3d_void *p_cmp_data)
{
    if (!p_list || !p_compareCB)
    {
        ARS3D_WARN("Required parameters are NULL");
        return ARS3D_NULL;
    }

    Ars3DListNode *current  = p_list->m_head; // If NULL, the list is empty.
    ars3d_void *data        = ARS3D_NULL;

    // Find the element and the data.
    while (current)
    {
        // Use the compare callback.
        if ( p_compareCB(current->m_data, p_cmp_data) )
        {
            data = current->m_data;
            break;
        }

        current = current->m_next;
    }


    //If not found, then return.
    if (!current) return ARS3D_NULL;


    // current is the head.
    if (current == p_list->m_head)
    {
        ARS3D_ASSERT(current->m_prev == ARS3D_NULL, "If current == head, the previous element SHOULD be NULL.");

        // Has next.
        if (p_list->m_head->m_next)
        {
            p_list->m_head = p_list->m_head->m_next;
            p_list->m_head->m_prev = ARS3D_NULL;
        }

        // This is the only element in the list.
        // This also covers if current = tail = head
        else
        {
            ARS3D_ASSERT(p_list->m_size == 1, "If head->next is NULL, the list should have ONE element.");
            ARS3D_ASSERT(p_list->m_head == p_list->m_tail, "If the list has one element, head should EQUAL tail.");
            p_list->m_head = ARS3D_NULL;
            p_list->m_tail = ARS3D_NULL;
        }
    }

    // current is the tail.
    else if (current == p_list->m_tail && current != p_list->m_head)
    {
        ARS3D_ASSERT(p_list->m_tail->m_prev, "Since cuurent = tail and its not the head, previous element MUST exists.");
        ARS3D_ASSERT(p_list->m_tail->m_next == ARS3D_NULL, "Since cuurent = tail the next element should be NULL.");

        p_list->m_tail          = p_list->m_tail->m_prev;
        p_list->m_tail->m_next  = ARS3D_NULL;
    }

    // Intermidiate element.
    else
    {
        ARS3D_ASSERT(current->m_next, "Since it is an intermidate node, next element should be defined");
        ARS3D_ASSERT(current->m_prev, "Since it is an intermidate node, prev element should be defined");
        current->m_prev->m_next = current->m_next;
        current->m_next->m_prev = current->m_prev;
    }

    // Decrease the size of the list.
    p_list->m_size--;

    // Free the current node.
    ars3dFree(current);

    return data;
}
