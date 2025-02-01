#include "include/common.h"
#include "include/chunk.h"
#include "include/debug.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);  // Initialize the chunk

    // Add an OP_CONSTANT instruction
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    // Add an OP_RETURN instruction
    writeChunk(&chunk, OP_RETURN, 1);

    // Debugging output
    // printf("Chunk count: %d\n", chunk.count);
    // for (int i = 0; i < chunk.count; i++) {
    //     printf("Byte %d: %02X\n", i, chunk.code[i]);
    // }

    // Disassemble the chunk
    disassembleChunk(&chunk, "test chunk");

    // Free the chunk
    freeChunk(&chunk);
    return 0;
}
