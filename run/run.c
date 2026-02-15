#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/tokenizer/tokenizer.h"
#include "../src/parser/parser.h"

void run_interactive_mode() {
    char input[256];
    printf("$ ");
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0; // remove newline
        int token_count = 0;
        char** tokens = tokenize_string(input, " ", &token_count);
        if (tokens) {
            int exit_signal = parse_tokens(tokens);
            free_tokens(tokens, token_count);
            if (exit_signal) {
                printf("EXITing shell. Have a nice day!\n");
                return;
            }
        }
        printf("$ ");
    }
}

void run_file_mode(const char* filepath) {
    char* delim = "\n";
    int count;
    char** lines = tokenize_file(filepath, delim, &count);
    
    if (lines == NULL) {
        fprintf(stderr, "Error: Failed to tokenize input file.\n");
    }
    else {
        for (int i = 0; i < count; i++) {
            char* line = lines[i];
            int line_count = 0;
            char** tokens = tokenize_string(line, " ", &line_count);
            if (tokens == NULL) {
                fprintf(stderr, "Error: Failed to tokenize line %d.\n", i + 1);
                continue;
            }
            parse_tokens(tokens);
            free_tokens(tokens, line_count);
        }

        for (int i = 0; i < count; i++) {
            free(lines[i]);
        }
    }

    free(lines);
}