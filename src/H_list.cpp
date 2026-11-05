#include "game.h"

ListNode::ListNode()
{
    prev = 0;
    next = 0;
    owner = 0;
    flags = 0;
}

ListNode::~ListNode()
{
    if (owner != 0)
        owner->FUN_00410f80(this);
}

NodeList::~NodeList()
{
    FUN_00410f40();
}

void NodeList::FUN_00410f80(ListNode *node)
{
    int g = node->flags & 7;
    ListNode *first;

    if (group.slot[g] == node) {
        first = node->prev;
        group.slot[g] = first;
        if (first != 0 && (first->flags & 7) != g)
            group.slot[g] = 0;
    }
    if (head == node)
        head = node->next;
    if (node->prev != 0)
        node->prev->next = node->next;
    if (node->next != 0)
        node->next->prev = node->prev;
    node->owner = 0;
}
