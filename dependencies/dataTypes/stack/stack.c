#include "stack.h"
#include "../LinkedList/linkedList.h"
#include <stdlib.h>
#include <stdio.h>

struct Stack {
  LinkedListNode* top;
};

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
    void* data = stack->top->data;
    LinkedListNode* to_remove = stack->top;
    stack->top = stack->top->next; // Update top pointer
    
    if (to_remove->owns_data) free(to_remove->data); // Free data if owned
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