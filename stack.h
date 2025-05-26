#ifndef STACK_H
#define STACK_H

/*
GROUP-9
  Harsh Shah           -     2022A7PS0169P
  Shah Harsh Dhaval    -     2022A7PS0174P
  Siddh Gandhi         -     2022A7PS0081P
  Devansh Shah         -     2022A7PS0005P
  Vrutant Shah         -     2022A7PS0119P
  Sehajpreet Singh     -     2022A7PS0085P
*/

// FIRST and FOLLOW set automated
// Both lexical and syntax analysis modules implemented
// Both lexeical and syntactical errors detected


#include "tree.h"  // Include tree structure definitions

// Structure representing a stack for storing tree nodes
typedef struct stack
{
    Tree_Node **arr;      // Dynamic array of tree node pointers
    int num_Of_Element;   // Current number of elements in the stack
    int capacity;         // Maximum capacity of the stack
} Stack;

// Function prototypes for stack operations

// Initializes a new stack and allocates memory
Stack *init_Stack();

// Pushes a tree node onto the stack
void push(Stack *stack, Tree_Node *tree_node);

// Returns the top element of the stack without removing it
Tree_Node *top(Stack *stack);

// Removes and returns the top element of the stack
Tree_Node *pop(Stack *stack);

#endif // STACK_H
