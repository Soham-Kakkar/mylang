#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef struct LinkedListNode {
    void *data;
    struct LinkedListNode* next;
    bool owns_data;
} LinkedListNode;

// Creation
LinkedListNode* createLinkedListNode(void *data, bool owns_data);

// Insertions
LinkedListNode* prependLinkedListNode(LinkedListNode* head, void *data, bool owns_data);
LinkedListNode* appendLinkedListNode(LinkedListNode* head, void *data, bool owns_data);

// Removals
LinkedListNode* removeLinkedListHead(LinkedListNode* head);
LinkedListNode* removeLinkedListTail(LinkedListNode* head);
LinkedListNode* removeLinkedListNode(LinkedListNode* head, int position);

// Destruction
void destroyLinkedList(LinkedListNode* head);

#endif