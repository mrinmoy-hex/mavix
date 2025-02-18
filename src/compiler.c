#include <stdio.h>

#include "include/common.h"
#include "include/compiler.h"
#include "include/scanner.h"

bool compile(const char *source, Chunk* chunk) {
    initScanner(source);
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    
}
