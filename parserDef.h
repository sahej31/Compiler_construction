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


#include "lexer.h"
#include "stack.h"

// Parse tree structure with root node
typedef struct Parse_Tree{
    Tree_Node *root;
} Parse_Tree;

// Structure to store first and follow sets for grammar
typedef struct First_Follow{
    int **char_first;    // First sets for non-terminals
    int **char_follow;   // Follow sets for non-terminals
} First_Follow;

// Structure for the LL(1) parse table
typedef struct Parse_Table{
    int **parse_table;   // 2D array for parse table entries
} Parse_Table;

// Enumeration of all non-terminals in the grammar
typedef enum Char_NonTerminal
{
    program,
    mainFunction,
    otherFunctions,
    stmts,
    function,
    input_par,
    parameter_list,
    output_par,
    dataType,
    remainingList,
    primitiveDatatype,
    constructedDatatype,
    typeDefinitions,
    typeDefinition,
    fieldDefinitions,
    fieldDefinition,
    moreFields,
    declarations,
    declaration,
    global_or_not,
    otherStmts,
    stmt,
    var,
    assignmentStmt,
    singleOrRecId,
    funCallStmt,
    outputParameters,
    inputParameters,
    iterativeStmt,
    conditionalStmt,
    ioStmt,
    moreExpansions,
    arithmeticExpression,
    option_single_constructed,
    expPrime,
    lowPrecedenceOperators,
    highPrecedenceOperators,
    term,
    factor,
    booleanExpression,
    allVar,
    logicalOp,
    relationalOp,
    returnStmt,
    optionalReturn,
    idList,
    more_ids,
    actualOrRedefined,
    definetypestmt,
    fieldType,
    oneExpansion,
    elsePart,
    termPrime,
    A,
} Char_NonTerminal;

// Union to represent either terminal or non-terminal
typedef union Char_Type
{
    Char_Terminal terminal;           // Terminal symbol
    Char_NonTerminal char_nonterminal; // Non-terminal symbol
} Char_Type;

// Structure to represent a character in the grammar
typedef struct Character
{
    Char_Type* typeof_char;        // Type of character (terminal or non-terminal)
    int isChar_Terminal;           // Flag to identify if it's a terminal
    struct Character *next;        // Next character in the rule
} Character;

// Structure to represent a grammar rule
typedef struct Grammar_Rule
{
    int grammar_Rule_No;           // Rule number
    int grammar_Rule_Len;          // Length of the rule
    Character *head;               // Start of the rule
    Character *tail;               // End of the rule
} Grammar_Rule;

// Structure to store range of non-terminals
typedef struct Range_Char_NonTerminal{
    int st;                        // Start index
    int en;                        // End index
} Range_Char_NonTerminal;

// Structure to represent the grammar
typedef struct Def_Grammar{
    int no_of_Rules;               // Number of rules
    int no_of_Char_NonTerminals;   // Number of non-terminals
    int no_of_Char_Terminals;      // Number of terminals
    Grammar_Rule **rules;          // Array of grammar rules
    Range_Char_NonTerminal **ranges; // Ranges for non-terminals
} Def_Grammar;

// Forward declaration of Error_Record
typedef struct Error_Record Error_Record;

// Structure to store parsing errors
typedef struct Error_Record{
    int line;                      // Line number where error occurred
    char error_message[200];       // Error message
} Error_Record;
