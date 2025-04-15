#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** tokenize_string(char* str, char* delim, int* count) {
    char* str_copy = strdup(str); // Make a copy to prevent modifying the original
    if (!str_copy) return NULL;

    char* token = strtok(str_copy, delim);
    int token_count = 0;

    // Count tokens
    while (token) {
        token_count++;
        token = strtok(NULL, delim);
    }

    free(str_copy); // Free temporary copy

    // Allocate memory for token array
    char** tokens = (char**)malloc((token_count + 1) * sizeof(char*));
    if (!tokens) return NULL;

    // Tokenize again to fill the array
    str_copy = strdup(str); // Copy again
    token = strtok(str_copy, delim);
    int i = 0;

    while (token) {
        tokens[i] = strdup(token);
        if (!tokens[i]) {
            for (int j = 0; j < i; j++) free(tokens[j]);
            free(tokens);
            free(str_copy);
            return NULL;
        }
        token = strtok(NULL, delim);
        i++;
    }

    tokens[i] = NULL; // Null-terminate the array
    free(str_copy);
    *count = token_count;

    return tokens;
}


char** tokenize_file(char* filepath, char* delim, int* count) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filepath);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char* str = (char*)malloc(file_size + 1);
    if (str == NULL) {
        printf("Memory allocation failed\n");
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