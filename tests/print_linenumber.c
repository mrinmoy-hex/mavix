#include <stdio.h>

// test to check representation of line numbers 
int main() {
    int lines[] = {1, 1, 1, 2, 2, 3, 3, 3, 3};  // simulated line numbers for instructions
    int count = sizeof(lines)/sizeof(int);
    printf("Number of elements: %d\n", count);

    for (int offset = 0; offset < count; offset++) {
        if (offset > 0 && lines[offset] == lines[offset - 1]) {
            printf("%04d    | Instruction\n", offset);
        } else {
            printf("%04d %4d Instruction\n", offset, lines[offset]);
        }
    }

    return 0;
}