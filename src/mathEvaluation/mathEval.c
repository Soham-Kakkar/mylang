#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "mathEval.h"

#include "../tokenizer/tokenizer.h"
#include "../variableStore/variableStore.h"
#include "../dataTypes/VectorStr/vectorStr.h"

// -------------------- HELPERS --------------------

static int is_number(const char* token) {
    if (token[0] == '-' && isdigit(token[1])) return 1;
    return isdigit(*token);
}

static int is_operator(const char* token) {
    return (
        strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
        strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
        strcmp(token, "**") == 0
    );
}

static int precedence(const char* op) {
    if (strcmp(op, "**") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    return 0;
}

static int is_right_associative(const char* op) {
    return strcmp(op, "**") == 0;
}

static VectorStr* infix_to_postfix(VectorStr* tokens) {
    VectorStr* output = createVectorStr(0);
    VectorStr* op_stack = createVectorStr(0);

    for (int i = 0; i < tokens->size; i++) {
        char* token = tokens->data[i];

        if (is_number(token) || isalpha(token[0])) {
            VectorStr_push(output, token, 1);
        }
        else if (is_operator(token)) {

            while (op_stack->size > 0 && is_operator(op_stack->data[op_stack->size - 1])) {
                char* top_op = op_stack->data[op_stack->size - 1];

                    if ((!is_right_associative(token) && precedence(token) <= precedence(top_op)) ||
                    (is_right_associative(token) && precedence(token) < precedence(top_op))) {
                    VectorStr_push(output, top_op, 1);
                    free(op_stack->data[op_stack->size - 1]); // Free the popped operator string
                    op_stack->size--;
                } else break;
            }

            VectorStr_push(op_stack, token, 1);
        }
        else if (!strcmp(token, "(")) {
            VectorStr_push(op_stack, token, 1);
        }
        else if (!strcmp(token, ")")) {
            while (op_stack->size > 0 && strcmp(op_stack->data[op_stack->size - 1], "(") != 0) {
                VectorStr_push(output, op_stack->data[op_stack->size - 1], 1);
                free(op_stack->data[op_stack->size - 1]); // Free the popped operator string
                op_stack->size--;
            }
            if (op_stack->size == 0) {
                fprintf(stderr, "Error: mismatched parentheses\n");
                VectorStr_free(output);
                VectorStr_free(op_stack);
                VectorStr* empty = createVectorStr(0);
                return empty; // Return the pointer
            }
            free(op_stack->data[op_stack->size - 1]); // Free the popped '(' string
            op_stack->size--; // pop "("
        }
    }

    while (op_stack->size > 0) {
        VectorStr_push(output, op_stack->data[op_stack->size - 1], 1);
        free(op_stack->data[op_stack->size - 1]); // Free the popped operator string
        op_stack->size--;
    }

    VectorStr_free(op_stack);
    return output; // Return the pointer
}

// -------------------- Postfix Evaluation --------------------

static int evaluate_postfix(VectorStr* postfix) {
    
    int* stack = malloc(sizeof(int) * postfix->size);
    int top = -1;

    for (int i = 0; i < postfix->size; i++) {
        char* token = postfix->data[i];

        if (is_number(token)) {
            stack[++top] = atoi(token);
        }

        else if (isalpha(token[0])) {
            int val = get_variable_value(token);
            stack[++top] = val;
        }
        else if (is_operator(token)) {
            int b = stack[top--];
            int a = stack[top--];

            if (!strcmp(token, "+")) stack[++top] = a + b;
            else if (!strcmp(token, "-")) stack[++top] = a - b;
            else if (!strcmp(token, "*")) stack[++top] = a * b;
            else if (!strcmp(token, "/")) stack[++top] = a / b;
            else if (!strcmp(token, "**")) stack[++top] = (int)pow(a, b);
;        }
    }

    int result = stack[top];
    free(stack);
    return result;
}

// -------------------- Public Evaluate Function --------------------

char* evaluate_expression(const char* expr) {
    VectorStr* tokens = tokenize_expression(expr);
    VectorStr* postfix = infix_to_postfix(tokens);

    int result = evaluate_postfix(postfix);

    VectorStr_free(tokens);
    VectorStr_free(postfix);

    char* result_str = malloc(32);
    if (!result_str) return NULL;
    snprintf(result_str, 32, "%d", result);

    return result_str;

}