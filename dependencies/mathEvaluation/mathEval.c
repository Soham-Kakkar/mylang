#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 32

// -------------------- DATA STRUCTURE --------------------

typedef struct {
    char* tokens[MAX_TOKENS];
    int count;
} TokenList;

// -------------------- TOKENIZATION --------------------

int is_operator(const char* token) {
    return (
        strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
        strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
        strcmp(token, "**") == 0
    );
}

int precedence(const char* op) {
    if (strcmp(op, "**") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    return 0;
}

int is_right_associative(const char* op) {
    return strcmp(op, "**") == 0;
}

TokenList tokenize_expr(char* expr) {
    TokenList list = {.count = 0};
    int i = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        char token[MAX_TOKEN_LEN] = {0};
        int j = 0;

        if (isdigit(expr[i])) {
            while (isdigit(expr[i])) token[j++] = expr[i++];
        } else if (expr[i] == '*' && expr[i + 1] == '*') {
            token[j++] = '*'; token[j++] = '*'; i += 2;
        } else if (strchr("+-*/()", expr[i])) {
            token[j++] = expr[i++];
        } else {
            printf("Unknown character: %c\n", expr[i]);
            exit(1);
        }

        token[j] = '\0';
        list.tokens[list.count++] = strdup(token);
    }
    return list;
}

// -------------------- SHUNTING YARD --------------------

TokenList infix_to_postfix(TokenList input) {
    TokenList output = {.count = 0};
    char* stack[MAX_TOKENS];
    int top = -1;

    for (int i = 0; i < input.count; ++i) {
        char* token = input.tokens[i];

        if (isdigit(token[0])) {
            output.tokens[output.count++] = strdup(token);
        } else if (is_operator(token)) {
            while (top >= 0 &&
                   is_operator(stack[top]) &&
                   ((is_right_associative(token) && precedence(token) < precedence(stack[top])) ||
                   (!is_right_associative(token) && precedence(token) <= precedence(stack[top])))) {
                output.tokens[output.count++] = strdup(stack[top--]);
            }
            stack[++top] = token;
        } else if (strcmp(token, "(") == 0) {
            stack[++top] = token;
        } else if (strcmp(token, ")") == 0) {
            while (top >= 0 && strcmp(stack[top], "(") != 0)
                output.tokens[output.count++] = strdup(stack[top--]);
            if (top < 0 || strcmp(stack[top], "(") != 0) {
                printf("Mismatched parentheses!\n");
                exit(1);
            }
            top--; // Pop "("
        }
    }

    while (top >= 0) {
        if (strcmp(stack[top], "(") == 0) {
            printf("Mismatched parentheses!\n");
            exit(1);
        }
        output.tokens[output.count++] = strdup(stack[top--]);
    }

    return output;
}

// -------------------- EVALUATION --------------------

char* evaluate_expression(char* expression) {
    TokenList tokens = tokenize_expr(expression);
    TokenList postfix = infix_to_postfix(tokens);
    int stack[MAX_TOKENS];
    int top = -1;

    for (int i = 0; i < postfix.count; ++i) {
        char* token = postfix.tokens[i];

        if (isdigit(token[0])) {
            stack[++top] = atoi(token);
        } else if (is_operator(token)) {
            if (top < 1) {
                printf("Evaluation error: too few operands.\n");
                exit(1);
            }
            int b = stack[top--];
            int a = stack[top--];
            int res = 0;

            if (strcmp(token, "+") == 0) res = a + b;
            else if (strcmp(token, "-") == 0) res = a - b;
            else if (strcmp(token, "*") == 0) res = a * b;
            else if (strcmp(token, "/") == 0) res = a / b;
            else if (strcmp(token, "**") == 0) res = (int)pow(a, b);
            else {
                printf("Unknown operator: %s\n", token);
                exit(1);
            }

            stack[++top] = res;
        }
    }

    if (top != 0) {
        printf("Evaluation error: stack has extra operands.\n");
        exit(1);
    }

    // Convert result to string
    char* result_str = malloc(32);
    if (!result_str) {
        perror("malloc failed");
        exit(1);
    }
    snprintf(result_str, 32, "%d", stack[top]);

    // Free memory used by tokens
    for (int i = 0; i < tokens.count; i++) free(tokens.tokens[i]);
    for (int i = 0; i < postfix.count; i++) free(postfix.tokens[i]);

    return result_str;
}
