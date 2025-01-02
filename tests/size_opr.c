#include <stdio.h>
#include "./include/common.h"

int main() {
    // Test the size operations

    // Define various size-related variables
    size_t oldSize = 10;             // Previous size
    size_t newSize = 20;             // New size after resizing
    size_t capacity = 10;            // Current capacity of the allocated memory
    size_t count = 5;                // Number of elements currently used
    size_t newCapacity = capacity * 2;   // New capacity after resizing (doubling)
    size_t oldCapacity = capacity;        // Previous capacity before resizing
    size_t oldCount = count;             // Previous number of elements
    size_t newCount = count + 1;         // New number of elements after adding one more

    // Calculate the memory size in bytes for different sizes and counts
    size_t oldSizeBytes = sizeof(uint8_t) * oldSize;         // Memory required for old size (in bytes)
    size_t newSizeBytes = sizeof(uint8_t) * newSize;         // Memory required for new size (in bytes)
    size_t oldSizeBytes2 = sizeof(uint8_t) * oldCount;       // Memory required for old count (in bytes)
    size_t newSizeBytes2 = sizeof(uint8_t) * newCount;       // Memory required for new count (in bytes)
    size_t oldSizeBytes3 = sizeof(uint8_t) * oldCapacity;    // Memory required for old capacity (in bytes)
    size_t newSizeBytes3 = sizeof(uint8_t) * newCapacity;    // Memory required for new capacity (in bytes)

    // Print the values of different variables for comparison
    printf("Old size: %zu\n", oldSize);                          // Output the old size
    printf("New size: %zu\n", newSize);                          // Output the new size
    printf("Capacity: %zu\n", capacity);                         // Output the current capacity
    printf("Count: %zu\n", count);                               // Output the current count of elements
    printf("New capacity: %zu\n", newCapacity);                  // Output the new capacity after resizing
    printf("Old capacity: %zu\n", oldCapacity);                  // Output the old capacity before resizing
    printf("Old count: %zu\n", oldCount);                        // Output the old count before resizing
    printf("New count: %zu\n", newCount);                        // Output the new count after resizing

    // Print the memory usage (in bytes) for different size-related variables
    printf("Old size in bytes: %zu\n", oldSizeBytes);            // Output the memory required for the old size
    printf("New size in bytes: %zu\n", newSizeBytes);            // Output the memory required for the new size
    printf("Old size in bytes 2: %zu\n", oldSizeBytes2);        // Output the memory required for the old count
    printf("New size in bytes 2: %zu\n", newSizeBytes2);        // Output the memory required for the new count
    printf("Old size in bytes 3: %zu\n", oldSizeBytes3);        // Output the memory required for the old capacity
    printf("New size in bytes 3: %zu\n", newSizeBytes3);        // Output the memory required for the new capacity

    return 0;  // Return 0 to indicate successful execution
}
