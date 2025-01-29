#include "include/common.h"
#include "include/chunk.h"
#include "include/debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);                  //  Initialize the chunk

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_RETURN, 13);      // Write the byte to the chunk

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);                  // Free the chunk
    return 0;
}