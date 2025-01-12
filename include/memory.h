#pragma once
#include "common.h"

// Memory management functions and macros

/**
 * Macro to calculate the new capacity for a dynamic array.
 * 
 * If the current capacity is less than 8, it returns 8.
 * Otherwise, it doubles the existing capacity.
 * 
 * This ensures an efficient growth strategy for dynamic arrays,
 * avoiding frequent reallocations.
 * 
 * @param capacity The current capacity of the array.
 * @return The new capacity for the array.
 */
#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

/**
 * Macro to resize a dynamic array.
 * 
 * This macro reallocates memory for an array of the given `type` to
 * accommodate the new size (`newCount`) of the array. It ensures safe
 * reallocation and type casting.
 * 
 * @param type The data type of the elements in the array.
 * @param pointer A pointer to the existing array.
 * @param oldCount The current number of elements in the array.
 * @param newCount The desired number of elements in the array.
 * @return A pointer to the newly resized array.
 */
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*) reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

/**
 * Macro to free memory allocated for a dynamic array.
 * 
 * This macro effectively frees the memory by calling the `reallocate` function
 * with a `newSize` of 0, which is the standard way of releasing memory.
 * 
 * @param type The data type of the elements in the array.
 * @param pointer A pointer to the array to be freed.
 * @param oldCount The current number of elements in the array.
 */
#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

/**
 * Reallocate memory for a block of data.
 * 
 * This function handles dynamic memory allocation and resizing for data structures.
 * It is a low-level function that adjusts the memory block pointed to by `pointer`
 * to the new size (`newSize`).
 * 
 * - If `pointer` is `NULL`, the function behaves like `malloc(newSize)`.
 * - If `newSize` is `0`, the function behaves like `free(pointer)`.
 * - Otherwise, the function resizes the existing memory block to `newSize`.
 * 
 * @param pointer A pointer to the memory block to be resized or freed.
 * @param oldSize The current size of the memory block.
 * @param newSize The desired size of the memory block.
 * @return A pointer to the newly allocated or resized memory block, or `NULL` if freed.
 */
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

