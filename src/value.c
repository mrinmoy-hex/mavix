#include <stdio.h>
#include "include/memory.h"
#include "include/value.h"

/**
 * Initializes a ValueArray structure.
 * 
 * @param array A pointer to the ValueArray to initialize.
 * 
 * Sets the array's `values` pointer to NULL, and both `count` (current number of elements)
 * and `capacity` (allocated space for elements) to 0.
*/

void initValueArray(ValueArray* array){
    array->values = NULL;   // no values are intially allocated
    array->count = 0;
    array->capacity = 0;
}

/**
 * Adds a value to the ValueArray.
 * 
 * @param array A pointer to the ValueArray to which the value will be added.
 * @param value The Value to add to the array.
 * 
 * If the array does not have enough capacity to store the new value, the function
 * increases its capacity using the GROW_CAPACITY macro and reallocates memory for
 * the array's storage using the GROW_ARRAY macro.
 */

void writeValueArray(ValueArray* array, Value value) {
    // check if the array needs to grow to accomodate the new value
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->count;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = value;    // add the value to the end of the array
    array->count++;                         // increment the count of elements
}


/**
 * Frees the memory used by a ValueArray.
 * 
 * @param array A pointer to the ValueArray to free.
 * 
 * This function deallocates the memory used to store the array's values using the
 * FREE_ARRAY macro. After freeing the memory, it reinitializes the array by calling
 * initValueArray to reset its fields to default values.
 */

void freeValueArray(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    // reset the array's field to their default initialized state
    initValueArray(array);
}


void printValue(Value value) {
    printf("%g", AS_NUMBER(value));
}