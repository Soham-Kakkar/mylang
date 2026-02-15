#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "../dataTypes/VectorStr/vectorStr.h"

char** tokenize_string(char* str, char* delim, int* count);
char** tokenize_file(const char* filepath, char* delim, int* count);

VectorStr* tokenize_expression(const char* expr); // Changed to return VectorStr*

void free_tokens(char **tokens, int token_count);

#endif // TOKENIZER_H