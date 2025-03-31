#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct Stack Stack;

// Creation/destruction
Stack* createStack();
void destroyStack(Stack* stack);

// Operations
void pushStack(Stack* stack, void* data, bool owns_data);
void* popStack(Stack* stack);
void* peekStack(const Stack* stack);
bool isEmptyStack(const Stack* stack);

#endif