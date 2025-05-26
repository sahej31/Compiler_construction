/*
GROUP-9

Harsh Shah               2022A7PS0169P
Shah Harsh Dhaval        2022A7PS0174P
Siddh Gandhi             2022A7PS0081P
Devansh Shah             2022A7PS0005P
Vrutant Shah             2022A7PS0119P
Sehajpreet Singh         2022A7PS0085P
*/

// FIRST and FOLLOW set automated
// Both lexical and syntax analysis modules implemented
// Both lexeical and syntactical errors detected

#ifndef PARSER_DEF
#define PARSER_DEF
#include "parserDef.h"
#include <stdbool.h>

// Initialize the parse table
Parse_Table initTable();

// Replace a character in a grammar rule
void replace_char(Grammar_Rule *grammar_rule, Character *character);

// Get the integer representation of a non-terminal character
int get_char_nonterminal(char *char_nonterminal);

// Initialize the parse tree structure
Parse_Tree Parse_Tree_Initialization();

// Find the string representation of a terminal character
char *find_char_terminal_string(int char_terminal);


// Check if a character is a terminal string
bool is_char_terminal_string(char *character);

// Initialize the grammar structure
Def_Grammar grammar_Initialization();

// Find the string representation of a non-terminal character
char *find_char_nonterminal_string(int char_nonTerminal);

// Form the grammar from rules
Def_Grammar grammar_form();

// Parse the input file and write the parse tree to output file
void parse(char* input_filename, char* output_filename);

// Initialize a character string
char *init_char_str();

// Initialize a grammar rule with a given number
Grammar_Rule *grammar_Rule_Initialization(int grammar_rule_no);

// Print the parse table
void parse_Table_Print(Parse_Table parse_table);

// Extract a character from a string
Character *extract_char(char *str);

// Print the grammar definition
void def_Grammar_Print(Def_Grammar *grammar);

// Calculate follow sets for the next rule
void follow_next_rule(Def_Grammar grammar, First_Follow first_follow);

// Create the parse table using first and follow sets
void Parse_Table_Create(First_Follow first_follow, Parse_Table parse_table, Def_Grammar grammar);

// Calculate first set for a non-terminal
void first_calc(Def_Grammar grammar, First_Follow first_follow, bool* complete, Char_NonTerminal nonterminal);

// Form the follow sets for all non-terminals
void follow_form(Def_Grammar grammar, First_Follow first_follow);

// Print the first and follow sets
void first_Follow_Print(First_Follow first_follow);

// Compare first and follow sets
First_Follow first_Follow_Comparision(Def_Grammar grammar);

// Form the first sets for all non-terminals
void first_Form(Def_Grammar grammar, First_Follow first_follow);

// Create a tree node for a non-terminal
Tree_Node *node_NonTerminal(int nonTerminal);


// Create a tree node from a token
Tree_Node *tokenize(Token_Info token);

// Initialize the parser
void initParser();

#endif
