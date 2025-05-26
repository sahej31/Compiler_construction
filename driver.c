/*
GROUP-9
  Harsh Shah           -     2022A7PS0169P
  Shah Harsh Dhaval    -     2022A7PS0174P
  Siddh Gandhi         -     2022A7PS0081P
  Devansh Shah         -     2022A7PS0005P
  Vrutant Shah         -     2022A7PS0119P
  Sehajpreet Singh     -     2022A7PS0085P
**/

// FIRST and FOLLOW set automated
// Both lexical and syntax analysis modules implemented
// Both lexical and syntactical errors detected

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "time.h"

int main(int argc, char *argv[])
{
    int command;
    lookup_Table_Initialization(); // Initialize lookup table for lexical analysis
    initParser(); // Initialize parser for syntax analysis
    
    while(1){
        printf("Enter 0 to exit the program, 1 to remove the comments, 2 to print the tokens, 3 to get the parse, 4 to get the time\n");
        scanf("%d", &command);
        
        if(command == 0){
            printf("Exiting the program ....\n");
            break; // Exit the program
        }
        else if(command == 1){
            printf("Removing the Comments......\n");
            comment_Remove(argv[1]); // Remove comments from the input file
            printf("Removed Comments Successfully......\n\n");
        }
        else if(command == 2){
            printf("Tokenization has been Started....\n");
            initLexer(); // Initialize the lexer
            file_Read(argv[1]); // Read the input file
            Token_Info t;
            
            while(true){
                t = get_Next(); // Get the next token
                if(t.char_terminal == TK_EOF){
                    break; // Stop at end of file
                }
                printf("%-15s %s\n", find_char_terminal_string(t.char_terminal), t.lexeme); // Print token and lexeme
            }
            printf("Tokenization has been Finished\n\n");
        }
        else if(command == 3){
            parse(argv[1], argv[2]); // Perform syntax analysis
            printf("Parsing has been Finished\n\n");
        }
        else if(command == 4){
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            
            start_time = clock(); // Start measuring time
            parse(argv[1], NULL); // Parse the input file
            end_time = clock(); // End measuring time
            
            total_CPU_time = (double) (end_time - start_time); // Calculate CPU time
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC; // Convert to seconds
            
            printf("Total CPU Time is : %lf\n", total_CPU_time); 
            printf("Total CPU Time in seconds is : %lf\n\n", total_CPU_time_in_seconds);
        }
        else{
            printf("Invalid Command. \n Please enter the correct command.\n");
        }
    }
}
