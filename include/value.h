#pragma once
#include "common.h"


typedef enum {
    VAL_BOOL,
    VAL_NULL,
    VAL_NUMBER,
} ValueType;


typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;


// check the correct type of value
#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NULL(value)     ((value).type == VAL_NULL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)


// extract exact value from the Value struct
#define AS_BOOL(value)     ((value).as.boolean)
#define AS_NUMBER(value)   ((value).as.number)


// value creating macros
#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NULL_VAL           ((Value){VAL_NULL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})


// a dynamic array of values
typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;


// funtion prototypes
bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);