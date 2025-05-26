
#ifndef LEXER_DEF
#define LEXER_DEF 

#include <stdbool.h>

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


// Constants for terminal, non-terminal counts and grammar rules
#define NUM_OF_TERMINALS 59
#define NUM_OF_NON_TERMINALS 53
#define TOTAL_GRAMMAR_RULES 95

// Enum defining all possible terminal tokens in the language
typedef enum Char_Terminal
{
    TK_MINUS, TK_IF, TK_DIV, TK_CALL, TK_WITH, TK_MAIN,
    TK_ENDWHILE, TK_TYPE, TK_ELSE, TK_INT, TK_NOT,
    TK_GT, TK_PARAMETERS, TK_THEN, TK_SEM, TK_RETURN,
    TK_ID, TK_DEFINETYPE, TK_OP, TK_WHILE, TK_OR,
    TK_FUNID, TK_COMMA, TK_INPUT, TK_UNION, TK_RECORD,
    TK_DOT, TK_RUID, TK_WRITE, TK_ENDUNION, TK_FIELDID,
    TK_LIST, TK_AND, TK_CL, TK_ASSIGNOP, TK_OUTPUT,
    TK_EQ, TK_ENDIF, TK_GLOBAL, TK_AS, TK_COLON,
    TK_NE, TK_GE, TK_LE, TK_SQR, TK_END,
    TK_PLUS, TK_ENDRECORD, TK_READ, TK_LT, TK_SQL,
    TK_NUM, TK_RNUM, TK_PARAMETER, TK_MUL,
    TK_REAL, TK_EPS, TK_EOF, TK_ERR,
} Char_Terminal;

// Array holding string representations of terminal tokens
extern char *Char_TerminalArr[NUM_OF_TERMINALS];

typedef Char_Terminal Token_ID;

// Union representing possible token values
typedef union Value
{
    int int_Val;
    float real_Val;
} Value;

// Node structure for linked lists to store tokens
typedef struct Node
{
    Token_ID tk_ID;
    char *lexeme;
    struct Node *next;
} Node;

// Structure representing each hash table entry
typedef struct Hash_Node
{
    Node *lst;  // List of nodes in this hash bucket
    int cnt;    // Count of entries in the list
} Hash_Node;

// Structure for the hash table itself
typedef struct Lookup_Table
{
    Hash_Node *Hash_map; // Array of hash nodes
} Lookup_Table;

// Structure representing detailed information about tokens
typedef struct Token_Info
{
    Char_Terminal char_terminal;
    char lexeme[40];
    int line;
    bool is_Integer;
    Value value;  // Union holding the actual token value
} Token_Info;

// List structure for managing tokens
typedef struct Token_List
{
    Token_Info *arr; // Array of tokens
    int size;        // Current size of the token list
    int max_size;    // Maximum capacity of the token list
} Token_List;

#endif
