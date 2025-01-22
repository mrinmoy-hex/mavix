#include <stdlib.h>
#include "include/chunk.h"
#include "include/memory.h"
#include "include/common.h"

// initialize the chunk with empty array
void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

// free the chunk and initialize it
void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);  // Free the array
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);                                   // Initialize the chunk
}

// write a byte to the end of the chunk 
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    // checks if there is space for one more element in the array
    if (chunk->capacity < chunk->count + 1) {
        // Calculate the new capacity of the array
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        // Reallocate memory for the array
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}


/**
 * Adds a constant value to the constants array in the given chunk.
 * 
 * This function appends the provided constant value to the `constants` array
 * within the `Chunk` and returns the index of the newly added constant.
 * 
 * - The function uses `writeValueArray` to handle the dynamic resizing
 *   of the `constants` array if needed.
 * - The returned index is calculated as `count - 1` because the `count` field
 *   represents the total number of elements in the array after the new value
 *   is added, and array indexing starts at 0.
 * 
 * @param chunk A pointer to the `Chunk` to which the constant is being added.
 * @param value The constant value to add (of type `Value`).
 * @return The index of the newly added constant in the `constants` array.
 */
int addConstant(Chunk* chunk, Value value) {
    // add the value to the constants array in the chunk
    writeValueArray(&chunk->constants, value);
    
    // returns the index of the newly added constants
    return chunk->constants.count - 1;
}