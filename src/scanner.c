#include <stdio.h>

#include "include/common.h"
#include "include/scanner.h"    

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;


void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}
