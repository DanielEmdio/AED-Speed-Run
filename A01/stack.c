#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

// stack size
int STACK_SIZE;

// Initializing the top of the stack to be -1
int top = -1;

// Initializing the stack using an array
int *stack;

void create_stack(int size) {
    // printf("STATIC ARRAY (Total Capacity: %d)\n", STACK_SIZE);
    stack = (int *)malloc(size * sizeof(int)); // allocate space for the stack
}

void destroy_stack() {
    free(stack); // free stack space
}

void push_to_stack(int element){
    // Checking overflow state
    if(top != STACK_SIZE - 1) {
        top+=1;
        stack[top] = element;
    }
}

int pop_from_stack(){
    // Checking underflow state
    if(top != -1){
        int x = stack[top];
        top-=1;
        return x;
    }
    return -1;
}

int peek(){
    int x = stack[top];
    // printf("%d is the top most element of the stack\n", x);
    return x;
}

bool isEmpty(){
    /*if(top == -1){
        printf("Stack is empty: Underflow State\n");
        return true;
    }
    printf("Stack is not empty\n");
    return false;*/

    return top == -1;
}

bool isFull(){
    /*if(top == STACK_SIZE-1){
        printf("Stack is full: Overflow State\n");
        return true;
    }
    printf("Stack is not full\n");
    return false;*/

    return top == STACK_SIZE-1;
}