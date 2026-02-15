#ifndef VARIABLE_STORE_H
#define VARIABLE_STORE_H

void init_variable_store();
void set_variable(const char* name, int value);
int get_variable(const char* name, int* value);
int get_variable_value(const char* var_name);
void free_variable_store();

#endif
