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

/**
 * @brief Disassembles a constant instruction for debugging purposes.
 *
 * This function prints the name of the instruction and the value of the constant
 * at the specified offset within the given chunk.
 *
 * @param name The name of the instruction to be printed.
 * @param chunk A pointer to the Chunk structure containing the bytecode and constants.
 * @param offset The offset within the chunk where the instruction is located.
 * @return The new offset after the instruction has been disassembled.
 */
static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    if (chunk->code[offset] == OP_CONSTANT) {
        uint8_t constant = chunk->code[offset + 1];  // One-byte index
        printf("%-16s %4d '", name, constant);
        printValue(chunk->constants.values[constant]);
        printf("'\n");
        return offset + 2;  // OP_CONSTANT takes 2 bytes
    } 
    
    else if (chunk->code[offset] == OP_CONSTANT_LONG) {
        uint32_t constant = chunk->code[offset + 1] 
                          | (chunk->code[offset + 2] << 8) 
                          | (chunk->code[offset + 3] << 16);  // Three-byte index
        printf("%-16s %4d '", name, constant);
        printValue(chunk->constants.values[constant]);
        printf("'\n");
        return offset + 4;  // OP_CONSTANT_LONG takes 4 bytes
    }

    printf("Unknown constant instruction!\n");
    return offset + 1;  // Move forward to avoid infinite loop
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

        case OP_CONSTANT_LONG:
            return constantInstruction("OP_CONSTANT_LONG", chunk, offset);

        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBSTRACT", offset);

        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);

        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);

        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);

        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);

        default:
            return simpleInstruction("Unknown opcode", offset);
    }
}
