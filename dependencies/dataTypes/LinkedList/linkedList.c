#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linkedList.h"

// Creates a new linked list node with the given data and ownership flag
LinkedListNode* createLinkedListNode(void *data, bool owns_data) {
    LinkedListNode* newNode = malloc(sizeof(LinkedListNode));
    if (!newNode) return NULL; // If memory allocation fails
    newNode->data = data;
    newNode->next = NULL;
    newNode->owns_data = owns_data;
    return newNode;
}

// Prepends a new node to the beginning of the linked list
LinkedListNode* prependLinkedListNode(LinkedListNode* head, void *data, bool owns_data) {
    LinkedListNode* newNode = createLinkedListNode(data, owns_data);
    if (!newNode) return head; // If memory allocation fails
    newNode->next = head;
    return newNode;
}

// Appends a new node to the end of the linked list
LinkedListNode* appendLinkedListNode(LinkedListNode* head, void *data, bool owns_data) {
    LinkedListNode* newNode = createLinkedListNode(data, owns_data);
    if (!newNode) return head; // If memory allocation fails
    
    if (!head) return newNode; // List is empty, return the new node
    
    LinkedListNode* current = head;
    while (current->next) current = current->next; // Traverse to the end of the list
    current->next = newNode;
    return head;
}

// Removes the head node from the linked list
LinkedListNode* removeLinkedListHead(LinkedListNode* head) {
    if (!head) {
        fprintf(stderr, "List is empty\n");
        return NULL;
    }
    LinkedListNode* new_head = head->next;
    if (head->owns_data) free(head->data); // Free the data if owned
    free(head);
    return new_head;
}

// Removes the tail node from the linked list
LinkedListNode* removeLinkedListTail(LinkedListNode* head) {
    if (!head) {
        fprintf(stderr, "List is empty\n");
        return NULL;
    }
    
    if (!head->next) {
        if (head->owns_data) free(head->data); // Free the data if owned
        free(head);
        return NULL;
    }
    
    LinkedListNode* current = head;
    while (current->next->next) current = current->next; // Traverse to the second last node
    if (current->next->owns_data) free(current->next->data); // Free the data if owned
    free(current->next);
    current->next = NULL;
    return head;
}

// Removes a node at the specified position from the linked list
LinkedListNode* removeLinkedListNode(LinkedListNode* node) {
    if (!node) {
        fprintf(stderr, "List is empty\n");
        return NULL;
    }
    
    if (position == 0) return removeLinkedListHead(head); // Remove the head node
    
    LinkedListNode* current = head;
    for (int i = 0; current && i < position - 1; i++) {
        current = current->next; // Traverse to the node before the one to be removed
    }
    
    if (!current || !current->next) {
        fprintf(stderr, "Position out of range\n");
        return head;
    }
    
    LinkedListNode* to_remove = current->next;
    current->next = to_remove->next;
    if (to_remove->owns_data) free(to_remove->data); // Free the data if owned
    free(to_remove);
    return head;
}

// Destroys the entire linked list
void destroyLinkedList(LinkedListNode* head) {
    while (head) {
        head = removeLinkedListHead(head); // Remove the head node until the list is empty
    }
}