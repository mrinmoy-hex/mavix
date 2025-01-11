#include <stdlib.h>
#include "include/chunk.h"
#include "include/memory.h"
#include "include/common.h"

// initialize the chunk with empty array
void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

// free the chunk and initialize it
void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);  // Free the array
    initChunk(chunk);   // Initialize the chunk
}

// write a byte to the end of the chunk 
void writeChunk(Chunk* chunk, uint8_t byte) {
    // if the number of elements in the array is greater than or equal to the capacity of the array
    if (chunk->capacity < chunk->count + 1) {
        // Calculate the new capacity of the array
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        // Reallocate memory for the array
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}