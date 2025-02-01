#pragma once
#include "chunk.h" 

typedef struct {
    Chunk* chunk;
} VM;


// function prototypes
void initVM();
void freeVM();