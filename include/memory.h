#pragma once
#include "common.h"

// Memory management functions

// Grow the capcity of the array
#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)   // If the capacity is less than 8, return 8, else return the double of the capacity/

// grow the array
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*) reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))      // Reallocate memory for the array

// free the array
#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);   