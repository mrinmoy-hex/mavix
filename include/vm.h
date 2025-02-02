#pragma once
#include "chunk.h" 
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;    // instruction pointer (program counter) -> tracks the current instruction being executed by the VM's bytecode
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;


typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;


// function prototypes
void initVM();
void freeVM();

InterpretResult interpret(Chunk* chunk);

// stack operations prototypes
void push(Value value);
Value pop();
