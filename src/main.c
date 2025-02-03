#include "include/vm.h"
#include "include/common.h"
#include "include/chunk.h"
#include "include/debug.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);  // Initialize the chunk

    // Add a constant using OP_CONSTANT
    writeConstant(&chunk, 1.2, 1);  // First constant

    // Add a second constant that exceeds the range of OP_CONSTANT (use OP_CONSTANT_LONG)
    writeConstant(&chunk, 1200, 3);  // Second constant

    writeChunk(&chunk, OP_NEGATE, 123);

    // -------------------------------------------------
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    // binary operations
    constant = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_ADD, 123);

    constant = addConstant(&chunk, 5.6);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_DIVIDE, 123);



    // Add an OP_RETURN instruction
    writeChunk(&chunk, OP_RETURN, 1);

    // Disassemble the chunk
    disassembleChunk(&chunk, "test chunk");

    interpret(&chunk);
    
    freeVM();
    freeChunk(&chunk);

    return 0;
}
