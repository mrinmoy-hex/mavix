#include <stdio.h>
#include <stdlib.h>

#include "include/common.h"
#include "include/compiler.h"
#include "include/scanner.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;


Parser parser;


static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = true;    // activate panic mode to avoid cascading errors
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // Nothing
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}



static void error(const char* message) {
    errorAt(&parser.previous, message);
}



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

/**
 * @brief Consumes a token of the specified type.
 *
 * This function checks if the current token matches the expected type.
 * If it does, the token is consumed. If it does not, an error message
 * is reported.
 *
 * @param type The expected type of the token to consume.
 * @param message The error message to report if the token type does not match.
 */
static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}


bool compile(const char *source, Chunk* chunk) {
    initScanner(source);

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    
    return !parser.hadError;
}
