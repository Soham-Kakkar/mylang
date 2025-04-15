
#ifndef TOKENIZER_H
#define TOKENIZER_H

char** tokenize_string(char* str, char* delim, int* count);
char** tokenize_file(char* filepath, char* delim, int* count);

#endif