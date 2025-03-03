#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef union {
    int32_t intValue;   // Typically 4 bytes.
    double doubleValue; // Typically 8 bytes.
} NumberUnion;

int main(void) {
    NumberUnion data;
    
    // Store a double in the union.
    data.doubleValue = 3.14159;
    
    // The union's size is the size of its largest member, which is the double.
    printf("Size of union: %zu bytes\n", sizeof(data));  // Likely 8 bytes.
    
    // Reading the union as a double gives the expected result.
    printf("As double: %f\n", data.doubleValue);
    
    // Reading the same memory as an int reinterprets the bits.
    printf("Interpreted as int: %" PRId32 "\n", data.intValue);
    
    return 0;
}
