#include "variableStore.h"
#include "../dataTypes/LinkedList/linkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char* name;
    int value;
} Variable;

static LinkedListNode* variable_list = NULL;

void init_variable_store() {
    variable_list = NULL;
}

void set_variable(const char* name, int value) {
    LinkedListNode* current = variable_list;
    while (current) {
        Variable* var = (Variable*)current->data;
        if (strcmp(var->name, name) == 0) {
            var->value = value;
            return;
        }
        current = current->next;
    }

    Variable* new_var = (Variable*)malloc(sizeof(Variable));
    if (!new_var) {
        fprintf(stderr, "Error: Memory allocation failed for new variable.\n");
        return;
    }
    new_var->name = strdup(name);
    if (!new_var->name) {
        fprintf(stderr, "Error: Memory allocation failed for variable name.\n");
        free(new_var);
        return;
    }
    new_var->value = value;
    variable_list = prependLinkedListNode(variable_list, new_var, true);
}

int get_variable(const char* name, int* value) {
    LinkedListNode* current = variable_list;
    while (current) {
        Variable* var = (Variable*)current->data;
        if (strcmp(var->name, name) == 0) {
            *value = var->value;
            return 1; // Success
        }
        current = current->next;
    }
    return 0; // Not found
}

int get_variable_value(const char* var_name) {
    int value;
    if (get_variable(var_name, &value)) {
        return value;
    }
    fprintf(stderr, "Error: Undefined variable '%s'. Using 0.\n", var_name);
    return 0;
}

void free_variable_store() {
    LinkedListNode* current = variable_list;
    while(current) {
        Variable* var = (Variable*)current->data;
        free(var->name);
        free(var);
        current = current->next;
    }
    destroyLinkedList(variable_list);
    variable_list = NULL;
}
