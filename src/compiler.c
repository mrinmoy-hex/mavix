#include <stdio.h>

#include "include/common.h"
#include "include/compiler.h"
#include "include/scanner.h"

typedef struct {
    Token current;
    Token previous;
} Parser;


Parser parser;

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}


static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        // if the token is valid, exit the loop
        if (parser.current.type != TOKEN_ERROR) break;

        // report the error and continue scanning
        errorAtCurrent(parser.current.start);
    }
}


bool compile(const char *source, Chunk* chunk) {
    initScanner(source);
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    
}
