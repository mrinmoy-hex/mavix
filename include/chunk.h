// chunk.h -> Contains the definition of the Chunk structure and the function prototypes for the functions defined in chunk.c file
#pragma once
#include "common.h"

// chunk -> sequence of bytecode instructions

// Chunk structure to store bytecode -> each instruction has one-byte opcode (Operation code)
typedef enum {
    OP_RETURN,
} OpCode;


typedef struct {
    int count;  // Number of elements in the array
    int capacity;   // Number of elements that can be stored in the array (size of the array)
    uint8_t* code;  // Array of bytes
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);