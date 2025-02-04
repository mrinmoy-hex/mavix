#include "include/vm.h"
#include "include/common.h"
#include "include/chunk.h"
#include "include/debug.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);  // Initialize the chunk

    writeConstant(&chunk, 44, 1);
    writeConstant(&chunk, 4, 1);

    writeChunk(&chunk, OP_ADD, 1);

    writeConstant(&chunk, 20, 1);

    writeChunk(&chunk, OP_SUBTRACT, 1);



    // Add an OP_RETURN instruction
    writeChunk(&chunk, OP_RETURN, 1);

    // Disassemble the chunk
    // disassembleChunk(&chunk, "test chunk");

    interpret(&chunk);
    
    freeVM();
    freeChunk(&chunk);

    return 0;
}
