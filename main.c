#include <stdio.h>
#include <string.h>
#include "run/run.h"
#include "src/variableStore/variableStore.h"

void runMyLang(int argc, char* argv[]) {
    init_variable_store();
    if (argc != 2) {
        run_interactive_mode();
    } else {
        run_file_mode(argv[1]);
    }
    free_variable_store();
}

int main(int argc, char* argv[]) {

    if (argc >= 2 && (strlen(argv[1]) > 1 && argv[1][0]=='-')) {
        if (strcmp(argv[1],"-h") || strcmp(argv[1],"--help")) {
        printf(
            "Welcome to MyLang, a super simple language written in C. A work in progress.\n"
            "Usage: ./mylang [option | file]\n"
            "Examples:\n"
            "\t./mylang\t\tOpens interactive MyLang shell\n"
            "\t./mylang -h\t\tOpens this help menu\n"
            "\t./mylang /path/to/file\tRuns the interpreter on the provided file\n"
            "Options:\n"
            "\t-h | --help:\tdisplays this help menu\n"
        );
        }
    }
    else runMyLang(argc, argv);
    fflush(stdout);
    return 0;
}
