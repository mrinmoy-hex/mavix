// chunk.h -> Contains the definition of the Chunk structure and the function prototypes for the functions defined in chunk.c file
#pragma once
#include "common.h"
#include "value.h"

// chunk -> sequence of bytecode instructions

// Chunk structure to store bytecode -> each instruction has one-byte opcode (Operation code)
typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NULL,
    OP_TRUE,
    OP_FALSE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_RETURN,
} OpCode;


// struct to store line info (run-length encoding)
typedef struct {
    int lineNumber;     // the actual line number
    int count;          // number of instructions in this line
} LineEntry;

typedef struct {
    int count;              // number of elements in the array
    int capacity;           // size of the array
    uint8_t* code;          // array of instructions
    LineEntry* lines;       // Array of (line, count) pairs
    int lineCount;          // number of used entries in lines
    int lineCapacity;       // capacity of lines array
    ValueArray constants;   // array of constants
} Chunk;



// function prototypes
void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void writeConstant(Chunk* chunk, Value value, int line);
int addConstant(Chunk* chunk, Value value);
int getLine(Chunk* chunk, int index);
