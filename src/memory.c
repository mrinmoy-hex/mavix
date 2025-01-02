#include <stdlib.h>
#include "include/memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    // if the new size is 0, free the memory
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }
    // Reallocate memory for the array
    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);    // If the result is NULL, exit the program
    return result;
}