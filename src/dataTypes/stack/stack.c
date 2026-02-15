#include "stack.h"
#include "../LinkedList/linkedList.h"
#include <stdlib.h>
#include <stdio.h>

// Creates a new stack
Stack* createStack() {
    Stack* stack = malloc(sizeof(Stack));
    if (!stack) return NULL; // Memory allocation failed
    stack->top = NULL;
    return stack;
}

// Destroys the stack and frees its memory
void destroyStack(Stack* stack) {
    if (!stack) return;
    destroyLinkedList(stack->top);
    free(stack);
}

// Pushes data onto the stack
void pushStack(Stack* stack, void* data, bool owns_data) {
    if (!stack) return;
    stack->top = prependLinkedListNode(stack->top, data, owns_data);
}

// Pops data from the stack
void* popStack(Stack* stack) {
    if (!stack || !stack->top) {
        fprintf(stderr, "Stack underflow\n");
        return NULL;
    }
    LinkedListNode* to_remove = stack->top;
    void* data = to_remove->data;

    stack->top = to_remove->next;
    free(to_remove); // Free node
    return data;
}

// Returns the top element of the stack without removing it
void* peekStack(const Stack* stack) {
    if (!stack || !stack->top) {
        fprintf(stderr, "Stack is empty\n");
        return NULL;
    }
    return stack->top->data;
}

// Checks if the stack is empty
bool isEmptyStack(const Stack* stack) {
    return stack ? (stack->top == NULL) : true;
}