#include <stdio.h>
#include <stdlib.h>
#include "dependencies/tokenizer/tokenizer.h"
#include "dependencies/parser/parser.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
      char input[256];
      printf("$");
      while(fgets(input, sizeof(input), stdin)) {
        int line_count = 0;
        char** tokens = tokenize_string(input, " ", &line_count);
        parse_tokens(tokens);
        free(tokens);
        printf("$");
      }
    }
  
    char* filepath = argv[1];
    char* delim = "\n";
    int count;
    char** lines = tokenize_file(filepath, delim, &count);
  
    if (lines == NULL) {
      printf("Error: Failed to tokenize input file.\n");
      return 1;
    }
  
    for (int i = 0; i < count; i++) {
      char* line = lines[i];
      char* line_delim = " ";
      int line_count = 0;
      char** tokens = tokenize_string(line, line_delim, &line_count);
  
      if (tokens == NULL) {
        printf("Error: Failed to tokenize line %d.\n", i + 1);
        continue;
      }
        //   printf("Line %d has %d tokens:\n", i + 1, line_count);
        //   for (int j = 0; j < line_count; j++) {
        //       printf("Token[%d]: %s\n", j, tokens[j]);
        //   }
      parse_tokens(tokens);
      free(tokens);
    }
    for (int i = 0; i < count; i++) {
      free(lines[i]); // Free each line
    }
  
    free(lines); // Free the lines array
    return 0;
}