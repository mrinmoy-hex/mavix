#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/vm.h"
#include "include/common.h"
#include "include/chunk.h"
#include "include/debug.h"


/**
 * @brief Starts the Read-Eval-Print Loop (REPL) for the Mavix language interpreter.
 * 
 * This function initializes and runs the REPL, allowing users to interactively
 * enter and evaluate Mavix language expressions. It continuously reads input
 * from the user, evaluates it, and prints the result until the user decides to exit.
 */
static void repl() {
    char line[1024];
    for (;;) {
        printf("mavix =>> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}


static char* readFile(const char* path) {

}



static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    // handle edge cases
    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}



int main(int argc, const char* argv[]) {
    initVM();

    // handling command-line args
    if (argc == 1) {
        repl();                 // activated REPL
    } else if (argc == 2) {
        runFile(argv[1]);       // scans the code
    } else {
        fprintf(stderr, "Usage: mavix [path]\n");
        exit(64);
    }
    
    freeVM();
    return 0;
}
