
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


#include "lexer.h"  // Include the lexer header file for token definitions

#ifndef TREE_  // Include guard to prevent multiple inclusions
#define TREE_

typedef struct Tree_Node Tree_Node; // Forward declaration of Tree_Node structure

// Structure representing a non-terminal node in the parse tree
typedef struct Non_Terminal_Node {
    int NTerminal_Node;          // Stores the identifier for the non-terminal symbol
    int Number_of_Children;      // Number of children nodes
    struct Tree_Node** children; // Array of pointers to child nodes
} Non_Terminal_Node;

// Structure representing a leaf node in the parse tree
typedef struct Leaf_Node {
    Token_Info* token; // Pointer to token information (from lexer)
} Leaf_Node;

// Union representing either a non-terminal or a terminal (leaf) node
typedef union Node_Type {
    Non_Terminal_Node NT_node; // Non-terminal node data
    Leaf_Node L_node;          // Leaf node data
} Node_Type;

// Structure representing a node in the parse tree
typedef struct Tree_Node {
    bool is_terminal;   // Indicates whether the node is a terminal (true) or non-terminal (false)
    Tree_Node* parent;  // Pointer to the parent node
    Tree_Node* next;    // Pointer to the next sibling node (for linked tree representation)
    Node_Type node_types; // Union storing either a non-terminal or terminal node
} Tree_Node;

#endif // TREE_
