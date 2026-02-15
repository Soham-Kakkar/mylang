#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"
#include "../dataTypes/VectorStr/vectorStr.h"

char** tokenize_string(char* str, char* delim, int* count) {
    char** tokens = NULL;
    int token_count = 0;
    char* start = str;
    char* end;

    while ((end = strpbrk(start, delim)) != NULL) {
        tokens = (char**)realloc(tokens, (token_count + 1) * sizeof(char*));
        if (!tokens) return NULL;
        int len = end - start;
        if (len > 0) {
            tokens[token_count] = (char*)malloc(len + 1);
            if (!tokens[token_count]) {
                for (int i = 0; i < token_count; i++) free(tokens[i]);
                free(tokens);
                return NULL;
            }
            strncpy(tokens[token_count], start, len);
            tokens[token_count][len] = '\0';
            token_count++;
        }
        start = end + 1;
    }

    if (*start != '\0') {
        tokens = (char**)realloc(tokens, (token_count + 1) * sizeof(char*));
        if (!tokens) return NULL;
        tokens[token_count] = strdup(start);
        if (!tokens[token_count]) {
            for (int i = 0; i < token_count; i++) free(tokens[i]);
            free(tokens);
            return NULL;
        }
        token_count++;
    }

    tokens = (char**)realloc(tokens, (token_count + 1) * sizeof(char*));
    if (!tokens) return NULL;
    tokens[token_count] = NULL;
    *count = token_count;

    return tokens;
}


char** tokenize_file(const char* filepath, char* delim, int* count) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filepath);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char* str = (char*)malloc(file_size + 1);
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    fread(str, 1, file_size, file);
    str[file_size] = '\0';
    fclose(file);

    char** tokens = tokenize_string(str, delim, count);
    free(str);
    return tokens;
}

VectorStr* tokenize_expression(const char* expr) {
    VectorStr* tokens = createVectorStr(0);

    int i = 0;
    while (expr[i]) {

        if (isspace(expr[i])) {
            i++;
            continue;
        }

        // Number (supports negative if at start or after '(' or operator)
        char* prev = (tokens->size > 0) ? tokens->data[tokens->size - 1] : NULL;

        if (isdigit(expr[i]) ||
            (expr[i] == '-' && (prev == NULL ||
            !strcmp(prev, "+") || !strcmp(prev, "-") || !strcmp(prev, "*")  ||
            !strcmp(prev, "/") || !strcmp(prev, "**") || !strcmp(prev, "(")) &&
            isdigit(expr[i+1]))
        ) {
            
            int start = i++;
            while (isdigit(expr[i])) i++;
            int len = i - start;
            char buf[32];
            strncpy(buf, &expr[start], len);
            buf[len] = '\0';
            VectorStr_push(tokens, buf, 1);
            continue;
        }

        // Variable (letters only)
        if (isalpha(expr[i])) {
            int start = i++;
            while (isalnum(expr[i])) i++;
            int len = i - start;
            char buf[32];
            strncpy(buf, &expr[start], len);
            buf[len] = '\0';
            VectorStr_push(tokens, buf, 1);
            continue;
        }

        // Power operator **
        if (expr[i] == '*' && expr[i + 1] == '*') {
            VectorStr_push(tokens, "**", 1);
            i += 2;
            continue;
        }

        // Single-char operator or parenthesis
        char tmp[2] = {expr[i], '\0'};
        VectorStr_push(tokens, tmp, 1);
        i++;
    }

    return tokens;
}
        
void free_tokens(char **tokens, int token_count) {
	if (tokens == NULL) return;
	for (int i = 0; i < token_count; i++) free(tokens[i]);
	free(tokens);
}
        