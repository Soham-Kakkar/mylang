#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../mathEvaluation/mathEval.h"

// Define a struct to hold keyword information
typedef struct {
    char* keyword;
    int (*action)(char**);  // Updated to accept char**
} Keyword;

// Variable storage
#define MAX_VARS 100
typedef struct {
    char* name;
    int value;
} Variable;

Variable variables[MAX_VARS]; // Array to store variables
int var_count = 0; // To keep track of how many variables we have

// Function to store variable in memory
int set_variable(char* var_name, int value) {
    // Check if variable already exists, update its value
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            variables[i].value = value;
            return 1; // Success, variable updated
        }
    }

    // If not found, add the new variable
    if (var_count < MAX_VARS) {
        variables[var_count].name = strdup(var_name); // Duplicate the string
        variables[var_count].value = value;
        var_count++;
        return 1; // Success, new variable added
    } else {
        printf("Error: Variable storage is full.\n");
        return 0; // Failure, no space for new variable
    }
}

// Action to handle SET command
int set_action(char** tokens) {
    if (tokens[1] == NULL || tokens[2] == NULL) {
        printf("Error: Missing arguments for SET command.\n");
        return 0;
    }

    if (strcmp(tokens[1], "INT") == 0) {
        char* var_name = tokens[2]; // This is the variable name
        int value = atoi(tokens[3]); // This is the integer value (tokens[3] should be the number)
        set_variable(var_name, value); // Store the variable
        printf("Set %s = %d\n", var_name, value); // Debug output
    } else {
        printf("Error: Invalid type '%s' for SET command.\n", tokens[1]);
    }

    return 0;
}

// Action for PRINT command (modified to handle char**)
int print_action(char** tokens) {
    if (tokens[1] == NULL) {
        printf("Error: Missing argument for PRINT keyword.\n");
        return 0;
    }
    
    // Print each token (you could modify this further for more complex output)
    for (int i = 1; tokens[i] != NULL; ++i) {
        printf("%s ", tokens[i]);  // Print the argument(s)
    }
    printf("\n");

    return 0;
}

// Action for EVAL command (modified to handle char**)
int mathEval_action(char** tokens) {
    if (tokens[1] == NULL) {
        printf("Error: Missing argument for EVAL keyword.\n");
        return 0;
    }

    // Assuming that evaluate_expression expects a single string as input
    char* expression = tokens[1]; // The expression to evaluate
    char* result = evaluate_expression(expression);
    printf("Result: %s\n", result);
    free(result);  // Free the result
    return 1;
}

// Define the keywords and their corresponding actions
Keyword keywords[] = {
    {"PRINT", print_action},
    {"EVAL", mathEval_action},
    {"SET", set_action},  // Add the SET keyword here
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
void parse_tokens(char** tokens) {
    int keyword_index = is_keyword(tokens[0]);
    if (keyword_index != -1) {
        if (tokens[1] == NULL) { // Check if argument exists
            printf("Error: Missing argument for keyword '%s'.\n", tokens[0]);
            return;
        }
        keywords[keyword_index].action(tokens);  // Pass tokens as char**
    } else {
        printf("Error: Unknown keyword '%s'.\n", tokens[0]);
    }
}
