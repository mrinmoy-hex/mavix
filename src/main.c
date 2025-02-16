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
        printf("Mavix REPL v0.1\nType 'exit' or press Ctrl+D to quit.\n");

        printf("mavix >>> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;      // handle Ctrl+D (EOF)
        }

        // Remove trailing newline if present
        line[strcspn(line, "\n")] = '\0';

        // Exit command check
        if (strcmp(line, "exit") == 0) {
            printf("Exiting REPL...\n");
            break;
        }

        interpret(line);
    }
}


/**
 * @brief Reads the contents of a file and returns it as a string.
 *
 * This function opens the file specified by the given path, reads its contents,
 * and returns the contents as a dynamically allocated string. The caller is
 * responsible for freeing the allocated memory.
 *
 * @param path The path to the file to be read.
 * @return A pointer to the dynamically allocated string containing the file contents,
 *         or NULL if an error occurs (e.g., the file cannot be opened).
 */
static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    // handling function call faileures
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    /**/
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);


    char* buffer = (char*) malloc(fileSize + 1);
    // edge case
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;

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
