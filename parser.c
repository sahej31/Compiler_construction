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


#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define debug(x) printf("%d\n", x)
#define GRAMMAR_FILE "GRAMMAR.txt"

/* Global data structures for grammar, first/follow sets, and parse table */
Def_Grammar grammar;
First_Follow first_follow;
Parse_Table parse_Table;

/* Array of non-terminal symbols used in the grammar */
char *Char_NonTerminalArr[] = {
    "program",
    "mainFunction",
    "otherFunctions",
    "stmts",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "remainingList",
    "primitiveDatatype",
    "constructedDatatype",
    "typeDefinitions",
    "typeDefinition",
    "fieldDefinitions",
    "fieldDefinition",
    "moreFields",
    "declarations",
    "declaration",
    "global_or_not",
    "otherStmts",
    "stmt",
    "var",
    "assignmentStmt",
    "singleOrRecId",
    "funCallStmt",
    "outputParameters",
    "inputParameters",
    "iterativeStmt",
    "conditionalStmt",
    "ioStmt",
    "moreExpansions",
    "arithmeticExpression",
    "option_single_constructed",
    "expPrime",
    "lowPrecedenceOperators",
    "highPrecedenceOperators",
    "term",
    "factor",
    "booleanExpression",
    "logicalOp",
    "relationalOp",
    "returnStmt",
    "optionalReturn",
    "idList",
    "more_ids",
    "actualOrRedefined",
    "definetypestmt",
    "fieldType",
    "oneExpansion",
    "elsePart",
    "termPrime",
    "A",
};

/* 
 * Initialize parse tree with a root node
 * Returns a newly created parse tree with program as root
 */
Parse_Tree Parse_Tree_Initialization()
{
    Parse_Tree parse_tree;
    parse_tree.root = (Tree_Node *)malloc(sizeof(Tree_Node));
    parse_tree.root->is_terminal = false;
    parse_tree.root->node_types.NT_node.NTerminal_Node = program;
    parse_tree.root->parent = NULL;
    parse_tree.root->next = NULL;
    parse_tree.root->node_types.NT_node.children = NULL;
    parse_tree.root->node_types.NT_node.Number_of_Children = 0;
    return parse_tree;
}

/* 
 * Add a symbol to the linked list representing a grammar rule
 * Updates the rule's head and tail pointers as needed
 */
void replace_char(Grammar_Rule *grammar_rule, Character *character)
{
    grammar_rule->grammar_Rule_Len+= 1;
    if (grammar_rule->head == NULL)
    {
        grammar_rule->head = character;
        grammar_rule->tail = character;
    }
    else
    {
        grammar_rule->tail->next = character;
        grammar_rule->tail = character;
    }
}

/* 
 * Get the index of a non-terminal in the enum from its string representation
 * Returns -1 if the non-terminal is not found
 */
int get_char_nonterminal(char *char_nonterminal)
{
    int char_nonterminals = NUM_OF_NON_TERMINALS;
    for (int i = 0; i < char_nonterminals; i++)
    {
        if (strcmp(char_nonterminal, Char_NonTerminalArr[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* Get string representation of a terminal symbol from its enum value */
char *find_char_terminal_string(int char_terminal)
{
    char *terminal_string = Char_TerminalArr[char_terminal];
    return terminal_string;
}

/* Get string representation of a non-terminal symbol from its enum value */
char *find_char_nonterminal_string(int char_nonTerminal)
{
    char *nonTerminal_string = Char_NonTerminalArr[char_nonTerminal];
    return nonTerminal_string;
}

/* 
 * Check if a string represents a terminal symbol
 * Terminals are prefixed with 'T' in their string representation
 */
bool is_char_terminal_string(char *character)
{
    return *character == 'T';
}

/* 
 * Initialize the grammar data structure 
 * Allocates memory for rules and non-terminal ranges
 */
Def_Grammar grammar_Initialization()
{
    Def_Grammar grammar;
    grammar.no_of_Rules = TOTAL_GRAMMAR_RULES;
    grammar.no_of_Char_NonTerminals = NUM_OF_NON_TERMINALS;
    grammar.no_of_Char_Terminals = NUM_OF_TERMINALS;
    grammar.rules = (Grammar_Rule **)malloc(sizeof(Grammar_Rule *) * grammar.no_of_Rules);
    grammar.ranges = (Range_Char_NonTerminal **)malloc(sizeof(Range_Char_NonTerminal *) * grammar.no_of_Char_NonTerminals);
    for (int i = 0; i < grammar.no_of_Char_NonTerminals; i++)
    {
        grammar.ranges[i] = (Range_Char_NonTerminal *)malloc(sizeof(Range_Char_NonTerminal));
        if (grammar.ranges[i] == NULL)
        {
            printf("Error in malloc\n");
        }
        grammar.ranges[i]->st = -1;
        grammar.ranges[i]->en = -1;
    }
    return grammar;
}

/* 
 * Initialize a new grammar rule with the given rule number
 * Sets up an empty linked list for the rule's symbols
 */
Grammar_Rule *grammar_Rule_Initialization(int grammar_rule_no)
{
    Grammar_Rule *rule = (Grammar_Rule *)malloc(sizeof(Grammar_Rule));
    rule->grammar_Rule_No = grammar_rule_no;
    rule->head = NULL;
    rule->tail = NULL;
    rule->grammar_Rule_Len = 0;
    return rule;
}

/* 
 * Parse a string and create a Character struct representing a terminal or non-terminal
 * Determines if the symbol is a terminal based on its prefix
 */
Character *extract_char(char *str)
{
    Character *character_current = (Character *)malloc(sizeof(Character));
    if (is_char_terminal_string(str))
    {
        character_current->typeof_char = (Char_Type *)malloc(sizeof(Char_Type));
        character_current->typeof_char->terminal = terminal_Get(str);
       character_current->isChar_Terminal = 1;
    }
    else
    {
         character_current->typeof_char = (Char_Type *)malloc(sizeof(Char_Type));
        character_current->typeof_char->char_nonterminal = get_char_nonterminal(str);
       character_current->isChar_Terminal = 0;
    }
    return character_current;
}

/* Initialize an empty string for symbol parsing */
char *init_char_str()
{
    char *char_string = (char *)malloc(sizeof(char) * 1);
    char_string[0] = '\0';
    return char_string;
}


/*
 * Reads the grammar from a file and builds the grammar data structure
 * Parses rules and organizes them by non-terminal
 */
Def_Grammar grammar_form()
{
    FILE *fp = fopen(GRAMMAR_FILE, "r"); 
    if (fp == NULL)
    {
        printf("There was an error while opening the file\n");
        exit(1);
    };
    Def_Grammar grammar = grammar_Initialization(); 
    char current;
    int rule_no = 0;
    int char_no = 0;
    char *str = (char *)malloc(sizeof(char) * 1);
    str[0] = '\0';
    Character *current_char_nonterminal = NULL;
    Grammar_Rule *current_rule = grammar_Rule_Initialization(rule_no);
    while (fscanf(fp, "%c", &current) != EOF) 
    {
        if (current == '\n') //initialize new rule
        {
            Character *current_char = extract_char(str);
            replace_char(current_rule, current_char);
            grammar.rules[rule_no] = current_rule;
            rule_no++;
            char_no = 0;
            current_rule = grammar_Rule_Initialization(rule_no);
            str = init_char_str();
        }
        else if (current == ' ')
        {
            Character *current_char = extract_char(str);
            replace_char(current_rule, current_char);
            if (char_no == 0)
            {
                if (current_char_nonterminal == NULL) // first non terminal
                {
                    current_char_nonterminal = current_char;
                    grammar.ranges[get_char_nonterminal(str)]->st = rule_no;
                }
                else if (current_char->typeof_char->char_nonterminal != current_char_nonterminal->typeof_char->char_nonterminal) // new non terminal
                {
                    grammar.ranges[current_char_nonterminal->typeof_char->char_nonterminal]->en = rule_no - 1;
                    current_char_nonterminal = current_char;
                    grammar.ranges[get_char_nonterminal(str)]->st = rule_no;
                }
            }
            char_no++;
            str = init_char_str();
        }
        else
        {
            insert_To_Str(str, current);
        }
    }
    Character *current_char = extract_char(str); // last symbol
    replace_char(current_rule, current_char);
    grammar.rules[rule_no] = current_rule;
    grammar.ranges[current_char_nonterminal->typeof_char->char_nonterminal]->en = rule_no;
    fclose(fp);
    return grammar;
}

/*
 * Calculate the First set for non-terminals using DFS approach
 * For each rule, recursively computes the First set based on grammar rules
 */
void first_calc(Def_Grammar grammar, First_Follow first_follow, bool* complete, Char_NonTerminal nonterminal){
    Range_Char_NonTerminal* range = grammar.ranges[nonterminal];
    for(int i=range->st;i<=range->en;i++){
        Grammar_Rule* current_rule = grammar.rules[i];
        Character* current = current_rule->head->next;
        bool is_Epsilon = true; 
        while(current!=NULL){
            if(current->isChar_Terminal){
                first_follow.char_first[nonterminal][current->typeof_char->terminal] = 1;
                is_Epsilon = false;
                break;
            }else{
               
                if(!complete[current->typeof_char->char_nonterminal]){
                    first_calc(grammar, first_follow, complete, current->typeof_char->char_nonterminal);
                }
                for(int j=0;j<NUM_OF_TERMINALS;j++){
                    if(first_follow.char_first[current->typeof_char->char_nonterminal][j] && j!=terminal_Get("TK_EPS")){
                        first_follow.char_first[nonterminal][j] = 1;
                    }
                }
                if(first_follow.char_first[current->typeof_char->char_nonterminal][terminal_Get("TK_EPS")]){
                    current = current->next;
                    is_Epsilon = true;
                }else{
                    is_Epsilon = false;
                    break;
                }
            }
        }
        if(is_Epsilon){
            first_follow.char_first[nonterminal][terminal_Get("TK_EPS")] = 1;
        }

    }
    complete[nonterminal] = true;
}

/*
 * Compute First sets for all non-terminals in the grammar
 * Uses the first_calc helper function for recursive computation
 */
void first_Form(Def_Grammar grammar, First_Follow first_follow)
{
    bool* complete = (bool*)malloc(sizeof(bool)*NUM_OF_NON_TERMINALS);
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        complete[i] = false;
    }
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        if(!complete[i]){
           first_calc(grammar, first_follow, complete, i);
            complete[i] = true;
        }
    }
}

/*
 * Calculate the Follow set for non-terminals
 * Repeatedly updates the Follow sets until no more changes occur
 */
void follow_form(Def_Grammar grammar, First_Follow first_follow)
{
    int modified = 1;
    while(modified){
        modified = 0;
        for(int i=0;i<grammar.no_of_Rules;i++){
            Grammar_Rule* current_rule = grammar.rules[i];
            Character* current = current_rule->head->next;
            while(current!=NULL){
                if(!current->isChar_Terminal){
                    Character* next = current->next;
                    while(next!=NULL){
                        if(next->isChar_Terminal){
                            if(next->typeof_char->terminal == TK_EPS){
                                next = next->next;
                            }else{
                                if(first_follow.char_follow[current->typeof_char->char_nonterminal][next->typeof_char->terminal]==0){
                                    modified = 1;
                                }
                                first_follow.char_follow[current->typeof_char->char_nonterminal][next->typeof_char->terminal] = 1;
                            }
                            break;
                        }else{
                            for(int j=0;j<NUM_OF_TERMINALS;j++){
                                if(first_follow.char_first[next->typeof_char->char_nonterminal][j] && j!=terminal_Get("TK_EPS")){
                                    if(first_follow.char_follow[current->typeof_char->char_nonterminal][j]==0) modified = 1;
                                    first_follow.char_follow[current->typeof_char->char_nonterminal][j] = 1;
                                }
                            }
                            if(first_follow.char_first[next->typeof_char->char_nonterminal][terminal_Get("TK_EPS")]){
                                next = next->next;
                            }else{
                                break;
                            }
                        }
                    }
                    if(next==NULL){
                        for(int j=0;j<NUM_OF_TERMINALS;j++){
                            if(first_follow.char_follow[current_rule->head->typeof_char->char_nonterminal][j]){
                                if(first_follow.char_follow[current->typeof_char->char_nonterminal][j]==0) modified = 1;
                                first_follow.char_follow[current->typeof_char->char_nonterminal][j] = 1;
                            }
                        }
                    }
                }
                current = current->next;
            }
        }
    }


}

/*
 * Initialize the parse table with default values (-1)
 * Prepares the table for construction with grammar rules
 */
Parse_Table initTable()
{
    Parse_Table parse_table;
    parse_table.parse_table = (int **)malloc(sizeof(int *) * NUM_OF_NON_TERMINALS);
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        parse_table.parse_table[i] = (int *)malloc(sizeof(int) * NUM_OF_TERMINALS);
        for (int j = 0; j < NUM_OF_TERMINALS; j++)
        {
            parse_table.parse_table[i][j] = -1;
        }
    }
    return parse_table;
}

/*
 * Create the parse table using the First and Follow sets
 * Fills the table with appropriate grammar rule numbers
 * -1 indicates no rule, -2 indicates synchronization error recovery point
 */
void Parse_Table_Create(First_Follow first_follow, Parse_Table parse_table, Def_Grammar grammar)
{
    for (int i = 0; i < grammar.no_of_Char_NonTerminals; i++)
    {
        parse_table.parse_table[i] = (int *)malloc(sizeof(int) * grammar.no_of_Char_Terminals);
        for (int j = 0; j < grammar.no_of_Char_Terminals; j++)
        {
            parse_table.parse_table[i][j] = -1;
        }
    }
    
    for (int i = 0; i < grammar.no_of_Rules; i++)
    {
        Grammar_Rule *current_Rule = grammar.rules[i];
        Character *left = current_Rule->head;
        Character *right = left->next;
        Character *current = right;
        bool is_Epsilon = false;
        while (current != NULL)
        {
            int ind_Epsilon = terminal_Get("TK_EPS");
            if (current->isChar_Terminal && current->typeof_char->terminal != ind_Epsilon)
            {
                parse_table.parse_table[left->typeof_char->char_nonterminal][current->typeof_char->terminal] = i;
                is_Epsilon = false;
                break;
            }
            else if (current->isChar_Terminal && current->typeof_char->terminal == ind_Epsilon)
            {
                is_Epsilon = true;
                break;
            }
            else
            {
                for (int j = 0; j < NUM_OF_TERMINALS; j++)
                {
                    if (first_follow.char_first[current->typeof_char->char_nonterminal][j] && j != ind_Epsilon)
                    {
                        parse_table.parse_table[left->typeof_char->char_nonterminal][j] = i;
                    }
                }
                if (first_follow.char_first[current->typeof_char->char_nonterminal][ind_Epsilon])
                {
                    current = current->next;
                    is_Epsilon = true;
                }
                else
                {
                    is_Epsilon = false;
                    break;
                }
            }
        }

        if (is_Epsilon)
        {
            for (int j = 0; j < NUM_OF_TERMINALS; j++)
            {
                if (first_follow.char_follow[left->typeof_char->char_nonterminal][j])
                {
                     parse_table.parse_table[left->typeof_char->char_nonterminal][j] = i;
                }
            }
        }
    }
    
    /* 
     * Mark synchronization points with -2 
     * These are points where error recovery can synchronize
     */
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        for (int j = 0; j < NUM_OF_TERMINALS; j++)
        {
            if (first_follow.char_follow[i][j] && parse_table.parse_table[i][j] == -1)
            {
                parse_table.parse_table[i][j] = -2;
            }
        }
    }
}

/*
 * Initialize the First and Follow sets data structure
 * Computes the actual First and Follow sets using the grammar
 */
First_Follow first_Follow_Comparision(Def_Grammar grammar)
{
    First_Follow f_f;
    /* Initialize First set structure */
    f_f.char_first = (int **)malloc(sizeof(int *) * NUM_OF_NON_TERMINALS);
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        f_f.char_first[i] = (int *)malloc(sizeof(int) * NUM_OF_TERMINALS);
        for (int j = 0; j < NUM_OF_TERMINALS; j++)
        {
            f_f.char_first[i][j] = 0;
        }
    }
    
    /* Initialize Follow set structure */
    f_f.char_follow = (int **)malloc(sizeof(int *) * NUM_OF_NON_TERMINALS);
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        f_f.char_follow[i] = (int *)malloc(sizeof(int) * NUM_OF_TERMINALS);
        for (int j = 0; j < NUM_OF_TERMINALS; j++)
        {
            f_f.char_follow[i][j] = 0;
        }
    }
    
    /* Compute First and Follow sets */
    first_Form(grammar, f_f);
    follow_form(grammar, f_f);
    return f_f;
}

/*
 * Print the grammar rules and non-terminal ranges
 * Debugging function to verify grammar construction
 */
void def_Grammar_Print(Def_Grammar *grammar)
{
    for (int i = 0; i < grammar->no_of_Rules; i++)
    {
        printf("Rule %d: ", grammar->rules[i]->grammar_Rule_No);
        Character *current = grammar->rules[i]->head;
        while (current != NULL)
        {
            if (current->isChar_Terminal)
            {
                printf("%s ", Char_TerminalArr[current->typeof_char->terminal]);
            }
            else
            {
                printf("%s ", Char_NonTerminalArr[current->typeof_char->char_nonterminal]);
            }
            current = current->next;
        }
        printf("\n");
    }

    for (int i = 0; i < grammar->no_of_Char_NonTerminals; i++)
    {
        printf("NonTerminal Symbols %s: %d %d\n", Char_NonTerminalArr[i], grammar->ranges[i]->st, grammar->ranges[i]->en);
    }
}

/*
 * Print the First and Follow sets for all non-terminals
 * Debugging function to verify First/Follow set computation
 */
void first_Follow_Print(First_Follow first_follow)
{
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        printf("First of %s: ", Char_NonTerminalArr[i]);
        for (int j = 0; j < NUM_OF_TERMINALS; j++)
        {
            if (first_follow.char_first[i][j])
            {
                printf("%s ", Char_TerminalArr[j]);
            }
        }
        printf("\n");
    }
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        printf("Follow of %s: ", Char_NonTerminalArr[i]);
        for (int j = 0; j < NUM_OF_TERMINALS; j++)
        {
            if (first_follow.char_follow[i][j])
            {
                printf("%s ", Char_TerminalArr[j]);
            }
        }
        printf("\n");
    }

}

/*
 * Print a parse tree node and recursively its children to a file
 * Formats the node information based on whether it's a terminal or non-terminal
 */
void printNode(Tree_Node *node, FILE* fp)
{
    if(node->is_terminal){
        char* lexeme = node->node_types.L_node.token->lexeme;
        int line_no = node->node_types.L_node.token->line;
        char* token_name = Char_TerminalArr[node->node_types.L_node.token->char_terminal];
        char* parent_node_symbol = Char_NonTerminalArr[node->parent->node_types.NT_node.NTerminal_Node];
        char* is_Leaf_Node = "yes";
        if(node->node_types.L_node.token->char_terminal != TK_NUM && node->node_types.L_node.token->char_terminal != TK_RNUM){
            fprintf(fp, "%-20s %-12d %-27s %-15s %-27s %-15s %-25s\n", lexeme, line_no, token_name, "NIL" ,parent_node_symbol, "YES", "NIL");
        }else if(node->node_types.L_node.token->char_terminal == TK_NUM){
            fprintf(fp, "%-20s %-12d %-27s %-15d %-27s %-15s %-25s\n", lexeme, line_no, token_name, node->node_types.L_node.token->value.int_Val, parent_node_symbol, "YES", "NIL");
        }else if(node->node_types.L_node.token->char_terminal == TK_RNUM){
            fprintf(fp, "%-20s %-12d %-27s %-15f %-27s %-15s %-25s\n", lexeme, line_no, token_name, node->node_types.L_node.token->value.real_Val, parent_node_symbol, "YES", "NIL");
        }else{
            printf("abnormal error has been occurred\n");
        }
    }else{
        /* For non-terminals, print the first child first, then the node itself, then remaining children */
        if(node->node_types.NT_node.Number_of_Children > 0){
            printNode(node->node_types.NT_node.children[0], fp);
        }
        char* node_symbol = Char_NonTerminalArr[node->node_types.NT_node.NTerminal_Node];
        if(node->parent == NULL){
            fprintf(fp, "--------------------------------- %-27s %-15s %-27s %-15s %-25s\n", "NIL", "NIL", "ROOT", "NO", node_symbol);
        }else{ 
            fprintf(fp, "--------------------------------- %-27s %-15s %-27s %-15s %-25s\n", "NIL","NIL", Char_NonTerminalArr[node->parent->node_types.NT_node.NTerminal_Node], "NO", node_symbol);
        }
        for(int i = 1; i < node->node_types.NT_node.Number_of_Children; i++){
            printNode(node->node_types.NT_node.children[i], fp);
        }
    }
}

/*
 * Print the entire parse tree to a specified output file
 * Writes the tree in a tabular format with headers
 */
void printTree(Parse_Tree pt, char* outfile)
{
    FILE* fp = fopen(outfile, "w");
    Tree_Node* tn = pt.root;
    fprintf(fp, "%-20s %-12s %-27s %-15s %-27s %-15s %-25s\n", "Lexeme", "Line_Number", "Token_Name", "Value_If_Number", "Parent_Node_Symbol", "Is_Leaf_Node", "Node_Symbol");
    fprintf(fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printNode(tn, fp);
    fclose(fp);
}

/*
 * Create a terminal node for the parse tree from a token
 * Initializes a tree node with the token's information
 */
Tree_Node *tokenize(Token_Info token)
{
    Tree_Node *node = (Tree_Node *)malloc(sizeof(Tree_Node));
    node->is_terminal = true;
    node->node_types.L_node.token = (Token_Info *)malloc(sizeof(Token_Info));
    node->node_types.L_node.token->char_terminal = token.char_terminal;
    strcpy(node->node_types.L_node.token->lexeme, token.lexeme);
    node->node_types.L_node.token->line = token.line;
    node->node_types.L_node.token->is_Integer = token.is_Integer;
    node->node_types.L_node.token->value = token.value;
    node->parent = NULL;
    node->next = NULL;
    return node;
}

/*
 * Create a non-terminal node for the parse tree
 * Initializes a tree node with the specified non-terminal symbol
 */
Tree_Node *node_NonTerminal(int non_terminal)
{
    Tree_Node *node = (Tree_Node *)malloc(sizeof(Tree_Node));
    node->is_terminal = false;
    node->node_types.NT_node.NTerminal_Node = non_terminal;
    node->node_types.NT_node.children = NULL;
    node->node_types.NT_node.Number_of_Children = 0;
    node->parent = NULL;
    node->next = NULL;
    return node;
}

/*
 * Print the parse table entries for debugging
 * Shows non-empty entries with corresponding grammar rules
 */
void parse_Table_Print(Parse_Table parse_table)
{
    for (int i = 0; i < NUM_OF_NON_TERMINALS; i++)
    {
        for (int j = 0; j < NUM_OF_TERMINALS; j++)
        {
            if (parse_table.parse_table[i][j] != -1 && parse_table.parse_table[i][j] != -2)
            {
                printf("Non-Terminal Symbol is: %s while Terminal Symbol is: %s and Rule is: %d\n", Char_NonTerminalArr[i], Char_TerminalArr[j], parse_table.parse_table[i][j] + 1);
            }
        }
    }
}




void parse_Input(char *test_case_file, Parse_Table parse_table, char* output_file)
{
    // Initialize lexical analyzer
    initLexer();
    // Read input file
    file_Read(test_case_file);
    Error_Record error[100];
    int error_number = 0;
    bool panic_mode = false; 
    // Get first token from lexer
    Token_Info token = get_Next();
    // Initialize stack for parser
    Stack *stack = init_Stack();
    // Initialize parse tree
    Parse_Tree parse_tree = Parse_Tree_Initialization();
    // Push root node onto stack
    push(stack, parse_tree.root);
    while (stack->num_Of_Element > 0) // while stack is not empty
    {
        // End parsing if EOF is reached
        if (token.char_terminal == TK_EOF)
        {
            break;
        }
        // Pop top node from stack
        Tree_Node *top_node = pop(stack);
        if (token.char_terminal == TK_ERR)
        {
            // Handle lexical error case
            char * error_messages = (char *)malloc(sizeof(char) * 200);
            sprintf(error_messages, "Line_No %d  Error: %s", token.line, token.lexeme);
            strcpy(error[error_number].error_message, error_messages);
            error_number++;
            error[error_number].line = token.line;
            // Push the node back and get next token
            push(stack, top_node);
            token = get_Next();
        }
        else if (top_node->is_terminal)
        {
            // Terminal node matching
            if (top_node->node_types.L_node.token->char_terminal == token.char_terminal) 
            {
                // Match successful, update terminal node with token info
                if (token.char_terminal != TK_EOF)
                {
                    top_node->node_types.L_node.token->line = token.line;
                }
                top_node->node_types.L_node.token->is_Integer = token.is_Integer;
                top_node->node_types.L_node.token->value = token.value;
                strcpy(top_node->node_types.L_node.token->lexeme, token.lexeme);
                // Get next token after successful match
                token = get_Next();
            }
            else
            {
                // Terminal mismatch error
                char *error_message = (char *)malloc(sizeof(char) * 200);
                sprintf(error_message, "Line number %d  Error: Invalid token %s found with value %s top of the stack %s", token.line, Char_TerminalArr[token.char_terminal], token.lexeme, Char_TerminalArr[top_node->node_types.L_node.token->char_terminal]);
                strcpy(error[error_number].error_message, error_message);
                error_number++;
                error[error_number].line = token.line;
            }
        }
        else
        {
            // Non-terminal node processing
            // Look up production rule in parse table
            int rule_no = parse_table.parse_table[top_node->node_types.NT_node.NTerminal_Node][token.char_terminal];
            //printf("%d\n",rule_no);
            if (rule_no == -1 || rule_no == -2) // error
            {
                // Parsing error: no valid production rule found
                char *error_message = (char *)malloc(sizeof(char) * 200);
                sprintf(error_message, "Line number %d  Error: The token %s for lexeme %s  does not match with the expected token %s", token.line, Char_TerminalArr[token.char_terminal], token.lexeme, Char_NonTerminalArr[top_node->node_types.NT_node.NTerminal_Node]);
                strcpy(error[error_number].error_message, error_message);
                error_number++;
                error[error_number].line = token.line;
                if (rule_no == -2)
                {
                    continue;
                }
                // Error recovery: skip tokens until a follow set element is found
                while (1)
                {
                    token = get_Next();
                    if (token.char_terminal == TK_ERR || token.char_terminal == TK_EOF)
                    {
                        break;
                    }
                    if(first_follow.char_follow[top_node->node_types.NT_node.NTerminal_Node][token.char_terminal]){
                        break;
                    }
                }
                continue;
            }
            // Get grammar rule for expansion
            Grammar_Rule *rule = grammar.rules[rule_no];
            Character *current = rule->head->next;
            int children = 0;
            Tree_Node *child_arr[20]; 
            // Create nodes for each symbol in the right-hand side of the rule
            while (current != NULL) 
            {
                if (current->isChar_Terminal)
                {
                    // Create terminal node
                    child_arr[children] = tokenize(token_Form(current->typeof_char->terminal, "\0"));
                }
                else
                {
                    // Create non-terminal node
                    child_arr[children] = node_NonTerminal(current->typeof_char->char_nonterminal);
                }
                children++;
                current = current->next;
            }
            // Push children onto stack in reverse order
            for (int i = children - 1; i >= 0; i--)
            {
                if(child_arr[i]->is_terminal && child_arr[i]->node_types.L_node.token->char_terminal == TK_EPS){
                    continue; // Skip epsilon productions
                }
                push(stack, child_arr[i]);
            }
            // Set up parent-child relationships in parse tree
            top_node->node_types.NT_node.Number_of_Children = children;
            top_node->node_types.NT_node.children = (Tree_Node **)malloc(sizeof(Tree_Node *) * children);
            for (int i = 0; i < children; i++)
            {
                top_node->node_types.NT_node.children[i] = child_arr[i];
                child_arr[i]->parent = top_node;
                if (i != 0)
                {
                    child_arr[i - 1]->next = child_arr[i];
                }
            }
        }
    }
    
    // Handle output and error reporting
    if (error_number == 0)
    {
        printf("There are no errors in the file!\n");
        if(output_file!=NULL){
            printTree(parse_tree, output_file);
            printf("Now we are printing parse tree to %s file\n", output_file);
        }
    }
    else if(error_number)
    {
        printf("1");
    }

    // Print errors if output file is specified
    if(output_file!=NULL){
        for (int i = 0; i < error_number; i++)
        {
            printf("%s\n", error[i].error_message);
        }
    }
}

// Initializes the parser by creating grammar, first and follow set and parse table
void initParser(){
    grammar = grammar_form();
    first_follow = first_Follow_Comparision(grammar);
    parse_Table = initTable();
    Parse_Table_Create(first_follow, parse_Table, grammar);
}

// Main parsing function that processes input file and generates parse tree output
void parse(char* input_filename, char* output_filename){
    parse_Input(input_filename, parse_Table, output_filename);
}
