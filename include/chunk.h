// chunk.h -> Contains the definition of the Chunk structure and the function prototypes for the functions defined in chunk.c file
#pragma once
#include "common.h"
#include "value.h"

// chunk -> sequence of bytecode instructions

// Chunk structure to store bytecode -> each instruction has one-byte opcode (Operation code)
typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;


typedef struct {
    int count;              // number of elements in the array
    int capacity;           // size of the array
    uint8_t* code;          // array of instructions
    int* lines;
    ValueArray constants;   // array of constants
} Chunk;

// function prototypes
void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);