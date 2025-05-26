
#ifndef LOOKUP
#define LOOKUP

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


// Constants defining grammar and token structure
#define NUM_OF_TERMINALS 59
#define NUM_OF_NON_TERMINALS 53
#define TOTAL_GRAMMAR_RULES 95

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

// Inserts a new node into the list with given token ID and lexeme
Node *list_Insert(Hash_Node hash_node, Token_ID tk_ID, char *lexeme);

// Appends a character to a string
void insert_To_Str(char *str, char c);

// Initializes the lookup table
void lookup_Table_Initialization();

// Inserts a keyword into the lookup table
void insert_Keyword(Lookup_Table *lookup_table, Token_ID tk_ID, char *lexeme);

// Retrieves the next string from the file stream
int str_Get(FILE *fp);

// Searches for a given lexeme in the list
Node *lst_Search(Node *head, char *lexeme);

// Generates a hash key for the given string
int get_Key(char *s);

// Forms a token from the given character terminal and lexeme
Token_Info token_Form(Char_Terminal char_terminal, char *lexeme);

// Maps a string terminal to its corresponding token ID
int terminal_Get(char *char_terminal);

// Finds and returns the token ID for a given lexeme in the lookup table
int lookup(Lookup_Table *lookup_table, char *lexeme);

// Creates and initializes a new token list
Token_List *create_Tkn_Lst();

// Prints the contents of the lookup table
void lookup_Table_Print(Lookup_Table *lookup_table);

// Adds a token to the token list
void add_Tkn(Token_List *lst, Token_Info tkn);


// Retrieves the current buffer being processed
char *current_Buffer();

// Reads the content of a file into the buffer
void file_Read(char *file);

// Moves the pointer back by 'no' characters in the lexeme
void retract(int no, char *lexeme);

// Marks the current character as accepted
void accept();

// Initializes the lexer before scanning begins
void initLexer();

// Initializes the buffer system for the given buffer number
void initBuffer(int buff_no);

// Retrieves the next character from the buffer
char next_Char();

// Retrieves the next token from the buffer
Token_Info get_Next();

// Removes comments from the provided file
void comment_Remove(char *testcase_File);

// Matches characters within a specific range
bool multiple_Match(char c, char st, char en);





#endif
