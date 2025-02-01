#include <stdio.h>
#include "include/debug.h"
#include "include/value.h"

/**
 * Disassembles a chunk of bytecode and prints its contents for debugging purposes.
 *
 * @param chunk A pointer to the Chunk structure containing the bytecode to disassemble.
 * @param name A string representing the name of the chunk, used for labeling the output.
 */
void disassembleChunk(Chunk* chunk, const char* name) {
    printf("=== %s ===\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

/**
 * Disassembles a single instruction from the given chunk at the specified offset.
 *
 * @param chunk A pointer to the Chunk containing the instruction to disassemble.
 * @param offset The offset within the chunk where the instruction starts.
 * @return The offset of the next instruction in the chunk.
 */
int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);

    // fetch the current line number using getLine()
    int lineNumber = getLine(chunk, offset);

    // If it's the same as the previous line, just print "|"
    if (offset > 0 && lineNumber == getLine(chunk, offset - 1)) {
        printf("   | ");
    } else {
        printf("%4d ", lineNumber);
    }

    // Get the instruction at the current offset
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            return simpleInstruction("Unknown opcode", offset);
    }
}
