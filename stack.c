/**
 * @file stack.c
 * @brief Implementation of a simple stack.
 */

#include "stack.h"

#define EMPTYSTACK -1
#define STACKSIZE 1024

int c_index = EMPTYSTACK; /**< Current index of the top of the stack. */
int stack[STACKSIZE]; /**< Array to store the elements of the stack. */

/**
 * @brief Pushes an element onto the stack.
 * 
 * @param value The value to be pushed onto the stack.
 */
void push(int value)
{
    stack[++c_index] = value;
}

/**
 * @brief Pops an element from the stack.
 * 
 * @return Returns the element popped from the stack, or -1 if the stack is empty.
 */
int pop()
{
    if (c_index == EMPTYSTACK) return -1;
    return stack[c_index--];
}

/**
 * @brief Retrieves the top element from the stack without removing it.
 * 
 * @return Returns the top element of the stack, or -1 if the stack is empty.
 */
int peek()
{
    if (c_index == EMPTYSTACK) return -1;
    return stack[c_index];
}

