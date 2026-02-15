#include <stdlib.h>
#include <string.h>
#include "vectorStr.h"

VectorStr* createVectorStr(int capacity) {
    VectorStr* v = malloc(sizeof(VectorStr));
    v->size = 0;
    v->capacity = capacity > 0 ? capacity : 8;
    v->data = malloc(sizeof(char*) * v->capacity);
    return v;
}

void VectorStr_push(VectorStr* v, const char* s, int owns) {
    if (v->size >= v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, sizeof(char*) * v->capacity);
    }
    if (owns) {
        v->data[v->size++] = strdup(s);
    } else {
        v->data[v->size++] = (char*)s;
    }
}

void VectorStr_free(VectorStr* v) {
    for (int i = 0; i < v->size; i++) free(v->data[i]);
    free(v->data);
    v->size = v->capacity = 0;
    free(v);
}
