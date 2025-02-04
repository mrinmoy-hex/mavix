#define STACK_MAX 4  // Example with a small stack size
#include <stdio.h>
#include <stdlib.h>

typedef double Value;  // Example for simplicity

typedef struct {
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

VM vm;  // Global VM instance

void initVM() {
    vm.stackTop = vm.stack;  // Start at the base of the stack
}

void push(Value value) {
    if (vm.stackTop > vm.stack + STACK_MAX) {  // Check for overflow
        fprintf(stderr, "Stack overflow\n");
        exit(1);
    }
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    if (vm.stackTop == vm.stack) {  // Check for underflow
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
    vm.stackTop--;
    return *vm.stackTop;
}

int main() {
    initVM();
    
    push(1.1);
    push(2.2);
    push(3.3);
    push(4.4);  // Stack is now full

    push(5.5);  // This will trigger "Stack overflow"
    // push(44);

    return 0;
}
