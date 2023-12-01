#ifndef _STACK_H
#define _STACK_H

/**
 * @brief Pushes an element onto the stack.
 * 
 * @param value The value to be pushed onto the stack.
 */
void push(int value);

/**
 * @brief Pops an element from the stack.
 * 
 * @return Returns the element popped from the stack, or -1 if the stack is empty.
 */
int pop();

/**
 * @brief Retrieves the top element from the stack without removing it.
 * 
 * @return Returns the top element of the stack, or -1 if the stack is empty.
 */
int peek();

#endif

