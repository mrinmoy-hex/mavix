#include "include/common.h"  
#include "include/vm.h"      // Virtual machine (VM) definitions
#include <stdio.h>

VM vm;  // Global VM instance

// Initialize the virtual machine
void initVM() {
    // Initialization logic goes here
}

// Free resources used by the virtual machine
void freeVM() {
    // Cleanup logic goes here
}


/**
 * @brief Reads a long index from the bytecode.
 *
 * This function reads a 32-bit unsigned integer from the bytecode stream.
 *
 * @return The 32-bit unsigned integer read from the bytecode.
 */
static uint32_t readLongIndex() {
    uint32_t index = 0;
    index |= (uint32_t)(*vm.ip++) << 16;  // Read the first byte (most significant)
    index |= (uint32_t)(*vm.ip++) << 8;   // Read the second byte
    index |= (uint32_t)(*vm.ip++);        // Read the third byte (least significant)
    return index;
}



// Execute bytecode instructions
static InterpretResult run() {
    /**
     * @brief Macro to read a constant value from the chunk's constants.
     *
     * This macro reads a byte from the bytecode and uses it as an index to 
     * access the corresponding constant value from the chunk's constants array.
     *
     * @return The constant value from the chunk's constants array.
     */
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    // Read the next byte from the instruction pointer using (*) and advance it
    #define READ_BYTE() (*vm.ip++)

    // Continuously execute instructions
    for (;;) {
        uint8_t instruction = READ_BYTE();  // Fetch the next instruction
        switch (instruction) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                printValue(constant);
                printf("\n");
                break;
            }

            case OP_CONSTANT_LONG: {
                // read a 3-byte index and fetch the constant
                uint32_t index = readLongIndex();
                Value constant = vm.chunk->constants.values[index];
                printValue(constant);
                printf("\n");
                break;
            }

            case OP_RETURN: {
                return INTERPRET_OK;  
            }  
        }
    }

    // Clean up the macro
    #undef READ_BYTE  
    #undef READ_CONSTANT
}

// Interpret a chunk of bytecode
InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;          // Set the current chunk
    vm.ip = vm.chunk->code;    // Set instruction pointer to the start of the chunk
    return run();              
}