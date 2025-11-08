#include <stdio.h>
#include <stdlib.h>
#include "dependencies/tokenizer/tokenizer.h"
#include "dependencies/parser/parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        // Interactive mode
        char input[256];
        printf("$ ");
        while (fgets(input, sizeof(input), stdin)) {
            int line_count = 0;
            // Tokenize the input string
            char** tokens = tokenize_string(input, " ", &line_count);

            // Parse the tokens using the updated parser
            parse_tokens(tokens);

            // Free the memory allocated for tokens
            free(tokens);

            // Print the prompt again
            printf("$ ");
        }
    } else {
        // File mode
        char* filepath = argv[1];
        char* delim = "\n";
        int count;
        
        // Tokenize the file content into lines
        char** lines = tokenize_file(filepath, delim, &count);
        
        if (lines == NULL) {
            printf("Error: Failed to tokenize input file.\n");
            return 1;
        }

        // Loop through each line in the file
        for (int i = 0; i < count; i++) {
            char* line = lines[i];
            int line_count = 0;
            // Tokenize the line into individual tokens
            char** tokens = tokenize_string(line, " ", &line_count);

            if (tokens == NULL) {
                printf("Error: Failed to tokenize line %d.\n", i + 1);
                continue;
            }

            // Parse the tokens using the updated parser
            parse_tokens(tokens);

            // Free the memory allocated for tokens
            free(tokens);
        }

        // Free each line's memory
        for (int i = 0; i < count; i++) {
            free(lines[i]);
        }

        // Free the lines array
        free(lines);
    }
    return 0;
}
