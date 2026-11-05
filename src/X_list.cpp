#include "game.h"

void NodeList::M_00410ff0()
{
    ListNode *node = head;
    ListNode *following;

    while (node) {
        following = node->next;
        if (node->flags & 0x10) {
            if (node)
                delete node;
        } else
            node->M_00410e90();
        node = following;
    }
}
