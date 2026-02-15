#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../variableStore/variableStore.h"
#include "../mathEvaluation/mathEval.h"

typedef struct {
    char* keyword;
    int (*action)(char**);
} Keyword;

// Action to handle SET command
int set_action(char** tokens) {
    if (tokens[1] == NULL || tokens[2] == NULL) {
        fprintf(stderr, "Error: Missing arguments for SET command.\n");
        return 0;
    }

    if (strcmp(tokens[1], "INT") == 0) {
        char* var_name = tokens[2];
        int value = atoi(tokens[3]); 
        set_variable(var_name, value);
        printf("Set %s = %d\n", var_name, value);
    } else {
        fprintf(stderr, "Error: Invalid type '%s' for SET command.\n", tokens[1]);
    }

    return 0;
}

// Action for PRINT command (modified to handle char**)
int print_action(char** tokens) {
    if (tokens[1] == NULL) {
        fprintf(stderr, "Error: Missing argument for PRINT keyword.\n");
        return 0;
    }
    
    for (int i = 1; tokens[i] != NULL; ++i) {
        char* s = tokens[i];
        for (int j = 0; s[j] != '\0'; j++) {
            if (s[j] == '\\') {
                j++;
                switch (s[j]) {
                    case 'n': putchar('\n'); break;
                    case 't': putchar('\t'); break;
                    case '\\': putchar('\\'); break;
                    default:
                        putchar('\\');
                        putchar(s[j]);
                }
            } else {
                putchar(s[j]);
            }
        }
        if (tokens[i+1]!=NULL) putchar(' ');
    }
    // printf("\n");

    return 0;
}

// Action for EVAL command
int mathEval_action(char** tokens) {
    if (tokens[1] == NULL) {
        fprintf(stderr, "Error: Missing expression for EVAL keyword.\n");
        return 0;
    }

    size_t total_len = 0;
    for (int i = 1; tokens[i] != NULL; i++) {
        total_len += strlen(tokens[i]) + 1;
    }
    char* expression = malloc(total_len);
    if (!expression) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }

    // Step 3: Concatenate tokens with spaces
    size_t len = 0;
    for (int i = 1; tokens[i] != NULL; i++) {
        size_t tlen = strlen(tokens[i]);
        memcpy(expression + len, tokens[i], tlen);
        len += tlen;
        if (tokens[i + 1] != NULL) expression[len++] = ' ';
    }
    expression[len] = '\0';

    char* result = evaluate_expression(expression);
    
    if (result != NULL) {
        printf("Result: %s\n", result);
        free(result);
    } else {
        fprintf(stderr, "Failed to evaluate expression.\n");
    }
    free(expression); // Free the dynamically allocated expression

    return 1;
}

// Action for EXIT command
int exit_action(char** tokens) {
    free_variable_store();
    return 1; 
}
// Define the keywords and their corresponding actions
Keyword keywords[] = {
    {"PRINT", print_action},
    {"EVAL", mathEval_action},
    {"SET", set_action},
    {"EXIT", exit_action},
};

// Function to check if a token is a keyword
int is_keyword(char* token) {
    for (int i = 0; i < (int)(sizeof(keywords) / sizeof(Keyword)); i++) {
        if (strcmp(token, keywords[i].keyword) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to process a token
int parse_tokens(char** tokens) {
    if (tokens == NULL || tokens[0] == NULL) return 0;
    
    int keyword_index = is_keyword(tokens[0]);
    if (keyword_index != -1) {
        return keywords[keyword_index].action(tokens);
    } else {
        fprintf(stderr, "Error: Unknown keyword '%s'.\n", tokens[0]);
        return 0;
    }
}
