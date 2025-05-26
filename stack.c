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

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Initializes a new stack and allocates memory for it
Stack *init_Stack()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack *)); // Incorrect memory allocation (should be sizeof(Stack))
    stack->capacity = 1;
    stack->num_Of_Element = 0;
    stack->arr = (Tree_Node**)malloc(stack->capacity * sizeof(Tree_Node*));
    return stack;
}

// Doubles the stack capacity when needed and reallocates memory
void extend_Stack(Stack *stack)
{
    stack->capacity *= 2;
    stack->arr = realloc(stack->arr, sizeof(Tree_Node *) * stack->capacity);
}

// Returns the top element of the stack without removing it
Tree_Node *top(Stack *stack){
    if(stack->num_Of_Element == 0){
        return NULL; // Return NULL if stack is empty
    }
    return stack->arr[stack->num_Of_Element - 1];
}

// Pushes a new element onto the stack, extending capacity if needed
void push(Stack *stack, Tree_Node *tree_node)
{
    if (stack->capacity == stack->num_Of_Element){
        extend_Stack(stack);
        push(stack, tree_node); // Recursive call after extending
    }
    else{
        stack->arr[stack->num_Of_Element] = tree_node;
        stack->num_Of_Element++;
    }
}

// Pops the top element from the stack and returns it
Tree_Node *pop(Stack *stack){
    if(stack->num_Of_Element == 0){
        return NULL; // Return NULL if stack is empty
    }
    else{
        Tree_Node *tree_node = stack->arr[stack->num_Of_Element - 1];
        stack->arr[stack->num_Of_Element - 1] = NULL; // Clear reference
        stack->num_Of_Element--;
        return tree_node;
    }
}

// Main function (commented out) for testing stack operations
// int main()
// {
//     Stack* stack1 = init_Stack();
// 
//     Tree_Node *tn1 = (Tree_Node*) malloc(sizeof(Tree_Node));
//     tn1->is_terminal = 1;
//     Tree_Node *tn2 = (Tree_Node*) malloc(sizeof(Tree_Node));
//     tn2->is_terminal = 0;
// 
//     push(stack1, tn1);
//     push(stack1, tn2);
//     push(stack1, tn1);
//     push(stack1, tn1);
//     push(stack1, tn2);
// 
//     for(int i = 0; i < 5; i++)
//     {
//         printf("%d" , stack1->arr[i]->is_terminal);
//     }
// 
//     printf("\n");
//     pop(stack1);
// 
//     for(int i = 0; i < 4; i++)
//     {
//         printf("%d" , stack1->arr[i]->is_terminal);
//     }
// }
