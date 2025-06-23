


#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 10

typedef struct {
    char* tokens[MAX_TOKENS];
    int count;
} TokenList;

char* evaluate_expression(char* expression);