#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct to hold keyword information
typedef struct {
    char* keyword;
    int (*action)(char**);
} Keyword;

// Define the actions for each keyword
int print_action(char** tokens) {
  if (tokens[1] != NULL) {
    for (int i = 1; tokens[i] != NULL; i++) {
      if (tokens[i+1] != NULL) printf("%s ",tokens[i]);
      else if (tokens[i] == "\\n" || tokens[i] == "\\t") printf("SPL"); // TODO: FIX THIS
      else printf("%s",tokens[i]);
    }
  } else {
      printf("Error: Missing argument for PRINT keyword.\n");
  }
  return 0;
}


// Define the keywords and their corresponding actions
Keyword keywords[] = {
  {"PRINT", print_action},
  // Add more keywords here...
};

// Function to check if a token is a keyword
int is_keyword(char* token) {
    for (int i = 0; i < sizeof(keywords) / sizeof(Keyword); i++) {
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
      keywords[keyword_index].action(tokens);
  } else {
      printf("Error: Unknown keyword '%s'.\n", tokens[0]);
  }
}