#include <stdio.h>

#include "include/common.h"
#include "include/scanner.h"    

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;


/**
 * @brief Initializes the scanner with the given source code.
 *
 * This function sets up the scanner to process the provided source code.
 *
 * @param source A pointer to a null-terminated string containing the source code to be scanned.
 */
void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}


// Token scanToken() {
//     scanner.start = scanner.current;

//     if (isAtEnd()) return makeTokens(TOKEN_EOF);

//     return errorToken("Unexpected character.");

// }