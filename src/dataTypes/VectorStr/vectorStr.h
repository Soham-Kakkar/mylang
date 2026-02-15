#ifndef VECTORSTR_H
#define VECTORSTR_H

typedef struct VectorStr {
    char** data;
    int size;
    int capacity;
} VectorStr;

VectorStr* createVectorStr(int capacity);
void VectorStr_push(VectorStr* v, const char* s, int owns);
void VectorStr_free(VectorStr* v);

#endif
