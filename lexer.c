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
// Both lexical and syntactical errors detected
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include <string.h>
#define BUFFER_SIZE 128        /* Size of the twin buffers for lexical analysis */
#define ENDOFFILE '\0'         /* Marker for end of file */
#define NUMBER_OF_KEYWORDS 28  /* Total number of keywords in the language */

/* Global variables for lexer functionality */
char buffer_0[BUFFER_SIZE];	   /* First buffer for reading input */
char buffer_1[BUFFER_SIZE];	   /* Second buffer for reading input */
FILE *ptr;					   /* File pointer for input */
int buffer_Usage = 0;		   /* Current buffer in use: 0 for buffer_0, 1 for buffer_1 */
int next_Buf = 1;			   /* Next buffer to fill */
char *begin_Lexeme = NULL;     /* Pointer to the beginning of the current lexeme */
char *next_Ptr1 = NULL;	       /* Pointer to the current position in the buffer */
char *next_Ptr2 = NULL;	       /* Secondary pointer for buffer management */
int count_line = 1;			   /* Current line number in the source code */
int state = 0;				   /* Current state in the lexer DFA */
bool read_already = false;	   /* Flag to indicate if buffer has been read already */
int last_result;			   /* Last result from file read operation */
bool reached_eof = false;	   /* Flag indicating if end of file has been reached */

int input_Over = 0;            /* Flag indicating if input processing is complete */
Token_List *token_list;        /* List of tokens extracted from source code */
Lookup_Table *lookup_table;    /* Hash table for keyword lookup */

/* Array of token names corresponding to token types */
char *Char_TerminalArr[] = {
	"TK_MINUS",
	"TK_IF",
	"TK_DIV",
	"TK_CALL",
	"TK_WITH",
	"TK_MAIN",
	"TK_ENDWHILE",
	"TK_TYPE",
	"TK_ELSE",
	"TK_INT",
	"TK_NOT",
	"TK_GT",
	"TK_PARAMETERS",
	"TK_THEN",
	"TK_SEM",
	"TK_RETURN",
	"TK_ID",
	"TK_DEFINETYPE",
	"TK_OP",
	"TK_WHILE",
	"TK_OR",
	"TK_FUNID",
	"TK_COMMA",
	"TK_INPUT",
	"TK_UNION",
	"TK_RECORD",
	"TK_DOT",
	"TK_RUID",
	"TK_WRITE",
	"TK_ENDUNION",
	"TK_FIELDID",
	"TK_LIST",
	"TK_AND",
	"TK_CL",
	"TK_ASSIGNOP",
	"TK_OUTPUT",
	"TK_EQ",
	"TK_ENDIF",
	"TK_GLOBAL",
	"TK_AS",
	"TK_COLON",
	"TK_NE",
	"TK_GE",
	"TK_LE",
	"TK_SQR",
	"TK_END",
	"TK_PLUS",
	"TK_ENDRECORD",
	"TK_READ",
	"TK_LT",
	"TK_SQL",
	"TK_NUM",
	"TK_RNUM",
	"TK_PARAMETER",
	"TK_MUL",
	"TK_REAL",
	"TK_EPS",
	"TK_EOF",
	"TK_ERR",
};

/* 
 * Inserts a new node into the linked list of a hash map entry
 * This is used for keyword lookup table construction
 */
Node *list_Insert(Hash_Node hash_node, Token_ID tk_ID, char *lexeme)
{
	if (hash_node.lst == NULL)
	{
		Node *node = (Node *)malloc(sizeof(Node));
		node->tk_ID = tk_ID;
		node->lexeme = lexeme;
		node->next = NULL;
		hash_node.cnt++;
		return node;
	}
	
	Node *node = (Node *)malloc(sizeof(Node));
	node->tk_ID = tk_ID;
	node->lexeme = lexeme;
	node->next = hash_node.lst;
	hash_node.lst = node;
	hash_node.cnt++;
	return node;
}

/*
 * Hash function for the lookup table
 * Generates a hash key for a given string using a simple polynomial rolling hash
 */
int get_Key(char *s)
{
	unsigned long hash_key = 7349;  /* Prime number as initial value */
	int num;
	int cnt = 0;

	while ((num = *s++))
	{
		hash_key = hash_key * 17 + num;  /* Multiply by prime and add ASCII value */
		cnt++;
	}
	return (hash_key % (2 * NUMBER_OF_KEYWORDS));  /* Modulo operation to fit table size */
}

/*
 * Inserts a keyword into the lookup table with its associated token ID
 */
void insert_Keyword(Lookup_Table *lookup_table, Token_ID tk_ID, char *lexeme)
{
	int num = get_Key(lexeme);
	lookup_table->Hash_map[num].lst = list_Insert(lookup_table->Hash_map[num], tk_ID, lexeme);
}

/*
 * Initializes the keyword lookup table with all language keywords
 * Maps each keyword string to its corresponding token ID
 */
void lookup_Table_Initialization()
{
	lookup_table = (Lookup_Table *)malloc(sizeof(Lookup_Table));
	lookup_table->Hash_map = (Hash_Node *)malloc(2 * NUMBER_OF_KEYWORDS * sizeof(Hash_Node));
	for (int i = 0; i < 2 * NUMBER_OF_KEYWORDS; i++)
	{
		lookup_table->Hash_map[i].lst = NULL;
		lookup_table->Hash_map[i].cnt = 0;
	}
	/* Insert all language keywords with their corresponding token IDs */
	insert_Keyword(lookup_table, TK_ELSE, "else");
	insert_Keyword(lookup_table, TK_WITH, "with");
	insert_Keyword(lookup_table, TK_PARAMETERS, "parameters");
	insert_Keyword(lookup_table, TK_OUTPUT, "output");
	insert_Keyword(lookup_table, TK_INT, "int");
	insert_Keyword(lookup_table, TK_READ, "read");
	insert_Keyword(lookup_table, TK_WRITE, "write");
	insert_Keyword(lookup_table, TK_RETURN, "return");
	insert_Keyword(lookup_table, TK_THEN, "then");
	insert_Keyword(lookup_table, TK_REAL, "real");
	insert_Keyword(lookup_table, TK_ENDWHILE, "endwhile");
	insert_Keyword(lookup_table, TK_IF, "if");
	insert_Keyword(lookup_table, TK_TYPE, "type");
	insert_Keyword(lookup_table, TK_MAIN, "_main");
	insert_Keyword(lookup_table, TK_GLOBAL, "global");
	insert_Keyword(lookup_table, TK_ENDIF, "endif");
	insert_Keyword(lookup_table, TK_ENDUNION, "endunion");
	insert_Keyword(lookup_table, TK_DEFINETYPE, "definetype");
	insert_Keyword(lookup_table, TK_AS, "as");
	insert_Keyword(lookup_table, TK_CALL, "call");
	insert_Keyword(lookup_table, TK_RECORD, "record");
	insert_Keyword(lookup_table, TK_ENDRECORD, "endrecord");
	insert_Keyword(lookup_table, TK_PARAMETER, "parameter");
	insert_Keyword(lookup_table, TK_END, "end");
	insert_Keyword(lookup_table, TK_WHILE, "while");
	insert_Keyword(lookup_table, TK_UNION, "union");
	insert_Keyword(lookup_table, TK_LIST, "list");
	insert_Keyword(lookup_table, TK_INPUT, "input");
}


/*
 * Searches for a lexeme in a linked list
 * Used by the lookup function to find keywords in the hash table
 */
Node *lst_Search(Node *head, char *lexeme)
{
	Node *itr = head;
	while (itr != NULL)
	{
		if (strcmp(itr->lexeme, lexeme) == 0)
			return itr;
		itr = itr->next;
	}
	return NULL;
}

/*
 * Looks up a lexeme in the hash table
 * Returns the token ID if found, or -1 if not found
 */
int lookup(Lookup_Table *lookup_table, char *lexeme)
{
	int ind = get_Key(lexeme);		  
	Node *head = lookup_table->Hash_map[ind].lst;
	if (lst_Search(head, lexeme) != NULL)
	{
		return lst_Search(head, lexeme)->tk_ID;
	}
	return -1;
}

/*
 * Debug function to print the contents of the lookup table
 */
void lookup_Table_Print(Lookup_Table *lookup_table)
{
	for (int i = 0; i < 2 * NUMBER_OF_KEYWORDS; i++)
	{
		printf("Ind: %d\n", i);
		Node *temp = lookup_table->Hash_map[i].lst;
		while (temp != NULL)
		{
			printf("%s\n", temp->lexeme);
			temp = temp->next;
		}
	}
}

/*
 * Gets the terminal index from its string representation
 * Used for parser integration
 */
int terminal_Get(char *char_terminal)
{
	if(strcmp(char_terminal, "TK_EPS")==0){
		return 56;
	}
	for (int i = 0; i < NUM_OF_TERMINALS; i++)
	{
		if (strcmp(char_terminal, Char_TerminalArr[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

/*
 * Appends a character to a string
 * Used when building lexemes character by character
 */
void insert_To_Str(char *str, char c)
{
	int length = strlen(str);
	str = (char *)realloc(str, length + 2);
	str[length] = c;
	str[length + 1] = '\0';
}

/*
 * Adds a token to the token list
 * Expands the list if necessary
 */
void add_Tkn(Token_List *lst, Token_Info tkn)
{
	if (lst->size * 2 >= lst->max_size)
	{
		lst->size *= 2;
		lst->arr = (Token_Info *)realloc(lst->arr, lst->max_size * sizeof(Token_Info));
	}
	lst->arr[lst->size++] = tkn;
}

/*
 * Creates a new token list with initial capacity
 */
Token_List *create_Tkn_Lst()
{
	Token_List *lst = (Token_List *)malloc(sizeof(Token_List));
	lst->max_size = 2;
	lst->size = 0;
	lst->arr = (Token_Info *)malloc(lst->max_size * sizeof(Token_Info));
	return lst;
}

/*
 * Returns pointer to the current buffer in use
 */
char *current_Buffer()
{
	if (buffer_Usage)
		return buffer_1;
	else
		return buffer_0;
}

/*
 * Opens the input file for lexical analysis
 */
void file_Read(char *file)
{
	ptr = fopen(file, "r");
	if (ptr == NULL)
	{
		printf("Error while opening the file\n");
		exit(1);
	}
}

/*
 * Initializes a specific buffer (0 or 1) by filling it with EOF markers
 */
void initBuffer(int buff_no)
{
	if (buff_no == 0)
	{
		memset(buffer_0, ENDOFFILE, BUFFER_SIZE);
	}
	else if (buff_no == 1)
	{
		memset(buffer_1, ENDOFFILE, BUFFER_SIZE);
	}
	buffer_Usage = 0;
	next_Buf = 1;
	begin_Lexeme = NULL;
	next_Ptr1 = NULL;
	next_Ptr2 = NULL;
	state = 0;
}

/*
 * Initializes the lexer state by setting up both buffers
 */
void initLexer()
{
	count_line=1;
	initBuffer(0);
	initBuffer(1);
}

/*
 * Reads a chunk of the file into the next buffer
 * Implements the twin-buffer mechanism for efficient lexical analysis
 */
int str_Get(FILE *fp)
{
	int result;

	if (buffer_Usage)
	{
		/* Reading into buffer_0 while buffer_1 is in use */
		if (read_already)
		{
			result = last_result;
			read_already = false;
		}
		else
		{
			result = fread(buffer_0, sizeof(char), BUFFER_SIZE, fp);
		}
		if (result < BUFFER_SIZE)
		{
			/* Mark end of file in buffer */
			buffer_0[result] = ENDOFFILE;
		}

		buffer_Usage = 0;
		next_Buf = 1;
	}
	else
	{
		/* Reading into buffer_1 while buffer_0 is in use */
		if (read_already)
		{
			result = last_result;
			read_already = false;
		}
		else
		{
			result = fread(buffer_1, sizeof(char), BUFFER_SIZE, fp);
		}
		if (result < BUFFER_SIZE)
		{
			/* Mark end of file in buffer */
			buffer_1[result] = ENDOFFILE;
		}

		buffer_Usage = 1;
		next_Buf = 0;
	}
	last_result = result;
	return result;
}


/*
 * Gets the next character from the input stream
 * Handles buffer transitions transparently
 */
char next_Char()
{
	if (begin_Lexeme == NULL)
	{
		/* First character being read - initialize pointers */
		int read = str_Get(ptr); 
		if (read == -1)
		{
			return ENDOFFILE;
		}

		begin_Lexeme = current_Buffer();
		next_Ptr1 = current_Buffer();
		char *next_current = next_Ptr1;
		next_Ptr1++;			
		return *next_current;
	}
	char *next_current = next_Ptr1;
	next_Ptr1++;
	if (next_Ptr1 - current_Buffer() >= BUFFER_SIZE)	
	{
		/* We've reached the end of current buffer, switch to the other */
		int read = str_Get(ptr); 
		if (read == -1)
		{
			return ENDOFFILE;
		}
		next_Ptr1 = current_Buffer();
	}
	if (*next_current == ENDOFFILE)
	{
		input_Over = 1;
	}
	return *next_current;
}

/*
 * Resets the lexical analyzer state to begin recognizing a new token
 * Called after a token has been successfully recognized
 */
void accept()
{
	state = 0;
	begin_Lexeme = next_Ptr1;
}

/*
 * Moves the input pointer back by the specified number of characters
 * Used when the lexer needs to backtrack during token recognition
 */
void retract(int no, char *lexeme)
{
	next_Ptr1 -= no;
	int len = strlen(lexeme);
	if (reached_eof)
	{
		len++;
	}
	lexeme[len - no] = '\0';  /* Truncate the lexeme by the number of retracted chars */

	/* Handle retraction across buffer boundaries */
	if (next_Ptr1 < current_Buffer())
	{
		int last = current_Buffer() - next_Ptr1;
		buffer_Usage = 1 - buffer_Usage;  /* Switch to the other buffer */
		next_Ptr1 = current_Buffer() + BUFFER_SIZE - last;
		read_already = true;
	}
	begin_Lexeme = next_Ptr1;
}

/*
 * Utility function to remove comments from source file
 * Comments in the language start with '%' and end at newline
 */
void comment_Remove(char *testcase_File)
{
	FILE *ptr = fopen(testcase_File, "r");
	if (ptr == NULL)
	{
		printf("Error while opening the file\n");
		exit(1);
	}
	char c;

	while ((c = fgetc(ptr)) != -1)
	{
		if (c == '%')
		{
			/* Skip everything until end of line */
			while ((c = fgetc(ptr)) != '\n');
		}
		printf("%c", c);
	}
	fclose(ptr);
}

/*
 * Creates a token with its lexeme, type, line number and value (if applicable)
 */
Token_Info token_Form(Char_Terminal char_terminal, char *lexeme)
{
	Token_Info tkn;
	tkn.char_terminal = char_terminal;
	strcpy(tkn.lexeme, lexeme);
	tkn.line = count_line;
	if (char_terminal == TK_NUM)
	{
		tkn.is_Integer = true;
		tkn.value.int_Val = atoi(lexeme);
	}
	else if (char_terminal == TK_RNUM)
	{
		tkn.is_Integer = false;
		tkn.value.real_Val = atof(lexeme);
	}
	return tkn;
}

/*
 * Helper function to check if a character falls within a specified range
 */
bool multiple_Match(char c, char st, char en)
{
	return c >= st && c <= en;
}

/*
 * The core lexical analyzer function
 * Implements a DFA (Deterministic Finite Automaton) for token recognition
 * Returns the next token from the input stream
 */
Token_Info get_Next()
{
	char c = '$';	
	int size = 0;
	char *lexeme = (char *)malloc(sizeof(char) * 10);
	if (lexeme == NULL)
	{
		printf("Failed to allocate memory\n");
		exit(1);
	}
	lexeme[0] = '\0';
	while (1)
	{
		switch (state)
		{
		case 0:  /* Initial state - starting point for all tokens */
		{
			c = next_Char();	
			if (c == ENDOFFILE)	
			{
				return token_Form(TK_EOF, lexeme);
			}
			if (c != '\n')	
			{
				insert_To_Str(lexeme, c);
			}

			/* Determine next state based on current character */
			if (c == '[')	
			{
				state = 26;
			}
			else if (c == ']')
			{
				state = 27;
			}
			else if (c == ',')
			{
				state = 28;
			}
			else if (c == ';')
			{
				state = 29;
			}
			else if (c == ':')
			{
				state = 30;
			}
			else if (c == '.')
			{
				state = 31;
			}
			else if (c == '(')
			{
				state = 32;
			}
			else if (c == ')')
			{
				state = 33;
			}
			else if (c == '+')
			{
				state = 22;
			}
			else if (c == '-')
			{
				state = 23;
			}
			else if (c == '*')
			{
				state = 24;
			}
			else if (c == '/')
			{
				state = 25;
			}
			else if (c == '&')
			{
				state = 19;  /* Start of potential '&&&' token (AND) */
			}
			else if (c == '@')
			{
				state = 16;  /* Start of potential '@@@' token (OR) */
			}
			else if (c == '~')
			{
				state = 15;  /* NOT operator */
			}

			else if (c == '=')
			{
				state = 8;  /* Start of potential '==' token (EQ) */
			}
			else if (c == '!')
			{
				state = 13;  /* Start of potential '!=' token (NE) */
			}
			else if (c == '>')
			{
				state = 10;  /* Greater than or greater than equal */
			}
			else if (c == '<')
			{
				state = 2;  /* Less than, less than equal, or assignment op (<---) */
			}
			else if (c == '#')
			{
				state = 53;  /* Start of RUID token */
			}
			else if (multiple_Match(c, '0', '9'))
			{
				state = 43;  /* Start of numeric token (NUM or RNUM) */
			}
			else if (c == '_')
			{
				state = 56;  /* Start of function ID */
			}
			else if (c == 'a' || multiple_Match(c, 'e', 'z'))
			{
				state = 35;  /* Start of identifier or keyword */
			}
			else if (multiple_Match(c, 'b', 'd'))
			{
				size = 1;
				state = 37;  /* Start of potential variable ID */
			}
			else if (c == '%')
			{
				/* Skip comments until end of line */
				while ((c = next_Char()) != '\n' && c != ENDOFFILE);
				count_line++;
				accept();
				lexeme = (char *)malloc(sizeof(char) * 1);
				lexeme[0] = '\0';
			}
			else if (c == '\t' || c == ' ')
			{
				state = 61;  /* Skip whitespace */
			}
			else if (c == '\n')
			{
				count_line++;
				lexeme = (char *)malloc(sizeof(char) * 1);
				lexeme[0] = '\0';
				accept();
			}
			else
			{
				/* Unknown character - report error */
				char *temp = (char *)malloc(sizeof(char) * 20);
				sprintf(temp, "Unknown symbol <%s>", lexeme);
				return token_Form(TK_ERR, temp);
			}
			break;
		}
		case -1:  /* Error state */
		{
			retract(1, lexeme);
			char *temp = (char *)malloc(sizeof(char) * 40);
			sprintf(temp, "Unknown pattern <%s>", lexeme);
			accept();
			return token_Form(TK_ERR, temp);
		}
		/* Final states for single-character tokens */
		case 26:
		{
			accept();
			return token_Form(TK_SQL, lexeme);  /* Left square bracket '[' */
		}
		case 27:
		{
			accept();
			return token_Form(TK_SQR, lexeme);  /* Right square bracket ']' */
		}
		case 28:
		{
			accept();
			return token_Form(TK_COMMA, lexeme);  /* Comma ',' */
		}
		case 29:
		{
			accept();
			return token_Form(TK_SEM, lexeme);  /* Semicolon ';' */
		}
		case 30:
		{
			accept();
			return token_Form(TK_COLON, lexeme);  /* Colon ':' */
		}
		case 31:
		{
			accept();
			return token_Form(TK_DOT, lexeme);  /* Dot '.' */
		}
		case 32:
		{
			accept();
			return token_Form(TK_OP, lexeme);  /* Left parenthesis '(' */
		}
		case 33:
		{
			accept();
			return token_Form(TK_CL, lexeme);  /* Right parenthesis ')' */
		}
		case 22:
		{
			accept();
			return token_Form(TK_PLUS, lexeme);  /* Plus '+' */
		}

		case 23:
		{
			accept();
			return token_Form(TK_MINUS, lexeme);  /* Minus '-' */
		}
		case 24:
		{
			accept();
			return token_Form(TK_MUL, lexeme);  /* Multiply '*' */
		}
		case 25:
		{
			accept();
			return token_Form(TK_DIV, lexeme);  /* Divide '/' */
		}
		/* States for multi-character operators */
		case 19:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '&')
			{
				state = 20;  /* After seeing '&&' */
			}
			else
			{
				state = -1;  /* Error state */
			}
			break;
		}
		case 20:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '&')
			{
				state = 21;  /* After seeing '&&&' */
			}
			else
			{
				state = -1;  /* Error state */
			}
			break;
		}
		case 21:
		{
			accept();
			return token_Form(TK_AND, lexeme);  /* AND operator '&&&' */
		}
		case 16:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '@')
			{
				state = 17;  /* After seeing '@@' */
			}
			else
			{
				state = -1;  /* Error state */
			}
			break;
		}
		case 17:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '@')
			{
				state = 18;  /* After seeing '@@@' */
			}
			else
			{
				state = -1;  /* Error state */
			}
			break;
		}
		case 18:
		{
			accept();
			return token_Form(TK_OR, lexeme);  /* OR operator '@@@' */
		}
		case 15:
		{
			accept();
			return token_Form(TK_NOT, lexeme);  /* NOT operator '~' */
		}
		case 8:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '=')
			{
				state = 9;  /* After seeing '==' */
			}
			else
			{
				state = -1;  /* Error state */
			}
			break;
		}
		case 9:
		{
			accept();
			return token_Form(TK_EQ, lexeme);  /* Equal operator '==' */
		}
		case 13:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '=')
			{
				state = 14;  /* After seeing '!=' */
			}
			else
			{
				state = -1;  /* Error state */
			}
			break;
		}


		case 14:
		{
			accept();
			return token_Form(TK_NE, lexeme);  /* Not equal operator '!=' */
		}
		case 10:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c != '=')
			{
				state = 12;  /* Just greater than '>' */
			}
			else
			{
				state = 11;  /* Greater than or equal '>=' */
			}
			break;
		}
		case 11:
		{
			accept();
			return token_Form(TK_GE, lexeme);  /* Greater than or equal operator '>=' */
		}
		case 12:
		{
			retract(1, lexeme);
			accept();
			return token_Form(TK_GT, lexeme);  /* Greater than operator '>' */
		}
		case 2:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '=')
				state = 3;  /* Less than or equal '<=' */
			else if (c == '-')
			{
				state = 4;  /* Start of potential assignment operator '<--' */
			}
			else
			{
				state = 7;  /* Just less than '<' */
			}
			break;
		}
		case 3:
		{
			accept();
			return token_Form(TK_LE, lexeme);  /* Less than or equal operator '<=' */
		}
		case 4:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '-')
			{
				state = 5;  /* After seeing '<--' */
			}
			else
			{
				state = 62;  /* Not an assignment operator */
			}
			break;
		}

		case 5:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '-')
			{
				state = 6;
			}
			else
			{
				state = -1;
			}
			break;
		}
		case 6:
		{
			accept();
			return token_Form(TK_ASSIGNOP, lexeme); // Recognizes assignment operator '<-'
		}
		case 62:
		{
			retract(2, lexeme);
			accept();
			return token_Form(TK_LT, lexeme); // Handles less than operator '<'
		}
		case 7:
		{
			retract(1, lexeme);
			accept();
			return token_Form(TK_LT, lexeme); // Handles less than operator '<'
		}
		case 53:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, 'a', 'z'))
			{
				state = 54;
			}
			else
			{
				state = -1;
			}
			break;
		}
		case 54:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, 'a', 'z'))
			{
				state = 54;
			}
			else
			{
				state = 55;
			}
			break;
		}
		case 55:
		{
			retract(1, lexeme);
			accept();
			return token_Form(TK_RUID, lexeme); // Recognizes record unit identifier
		}
		case 43:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, '0', '9'))
			{
				state = 43;
			}
			else if (c == '.')
			{
				state = 44;
			}
			else
				state = 48;
			break;
		}
		case 48:
		{
			retract(1, lexeme);
			accept();
			return token_Form(TK_NUM, lexeme); // Recognizes integer number
		}
		case 44:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, '0', '9'))
			{
				state = 45;
			}
			else
			{
				state = 63;
			}
			break;
		}
		case 63:
		{
			retract(2, lexeme);
			accept();
			return token_Form(TK_NUM, lexeme); // Handles integer with trailing period
		}
		case 45:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, '0', '9'))
			{
				state = 46;
			}
			else
			{
				state = -1;
			}
			break;
		}
		case 46:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == 'E'||c=='e')
			{
				state = 49;
			}
			else
			{
				state = 47;
			}
			break;
		}
		case 47:
		{
			retract(1, lexeme);
			accept();
			return token_Form(TK_RNUM, lexeme); // Recognizes real number without exponent
		}
		case 49:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (c == '+' || c == '-')
			{
				state = 50;
			}
			else if (multiple_Match(c, '0', '9'))
			{
				state = 51;
			}
			else
			{
				state = -1;
			}
			break;
		}
		case 50:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, '0', '9'))
			{
				state = 51;
			}
			else
			{
				state = -1;
			}
			break;
		}
		case 51:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, '0', '9'))
			{
				state = 52;
			}
			else
			{
				state = -1;
			}
			break;
		}
		case 52:
		{
			accept();
			return token_Form(TK_RNUM, lexeme); // Recognizes real number with exponent
		}
		case 56:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, 'a', 'z') || multiple_Match(c, 'A', 'Z'))
			{
				state = 57;
			}
			else
			{
				state = -1;
			}
			break;
		}
		case 57:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, 'a', 'z') || multiple_Match(c, 'A', 'Z'))
			{
				state = 57;
			}
			else if (multiple_Match(c, '0', '9'))
			{
				state = 58;
			}
			else
				state = 59;
			break;
		}
		case 58:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, '0', '9'))
			{
				state = 58;
			}
			else
				state = 59;
			break;
		}
		case 59:
		{
			retract(1, lexeme);
			accept();
			if (lookup(lookup_table, lexeme) != -1)
			{
				return token_Form(TK_MAIN, lexeme); // Recognizes 'main' keyword
			}
			else
				return token_Form(TK_FUNID, lexeme); // Recognizes function identifier
			break;
		}
		case 37:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, 'a', 'z'))
			{
				state = 35;
			}
			else if (multiple_Match(c, '2', '7'))
			{
				state = 38;
			}
			else
			{
				state = 41;
			}
			break;
		}
		case 35:
		{
			c = next_Char();
			insert_To_Str(lexeme, c);
			if (multiple_Match(c, 'a', 'z'))
			{
				state = 35;
			}
			else
			{
				state = 36;
			}
			break;
		}
		case 36:
		{
			if (c == '\0')
			{
				reached_eof = true;
			}
			retract(1, lexeme);
			reached_eof = false;
			accept();
			if (lookup(lookup_table, lexeme) != -1)
			{
				Char_Terminal key_ID = lookup(lookup_table, lexeme);
				return token_Form(key_ID, lexeme); // Recognizes keyword
			}
			else
			{
				return token_Form(TK_FIELDID, lexeme); // Recognizes field identifier
			}
		}
		case 38:
		{
			c = next_Char();
			if (size <= 20)
				insert_To_Str(lexeme, c);
			if (multiple_Match(c, 'b', 'd'))
			{
				size++;
				state = 38;
			}
			else if (multiple_Match(c, '2', '7'))
			{
				size++;
				state = 39;
			}
			else
			{
				state = 40;
			}
			break;
		}
		case 39:
		{
			c = next_Char();
			if (size <= 20)
				insert_To_Str(lexeme, c);
			if (multiple_Match(c, '2', '7'))
			{
				size++;
				state = 39;
			}
			else
			{

				state = 40;
			}
			break;
		}
		case 40:
		{
			retract(1, lexeme);
			accept();
			if (size > 20)
			{
				return token_Form(TK_ERR, "Variable name too long"); // Error for identifier exceeding max length
			}
			return token_Form(TK_ID, lexeme); // Recognizes variable identifier
		}
		case 41:
		{
			retract(1, lexeme);
			accept();
			return token_Form(TK_FIELDID, lexeme); // Recognizes field identifier
		}
		case 61:
		{
			c = next_Char();
			if (c == '\t' || c == ' ')
			{
				state = 61;
			}
			else
			{
				retract(1, lexeme);
				state = 0;
			}
		}
		}
	}
}
