# makefile.txt
# Target to build the final executable
final_exe : driver.o parser.o lexer.o stack.o
	gcc -o final_exe driver.o parser.o lexer.o stack.o

# Compiles the parser module
parser.o: parser.c parser.h
	gcc -c parser.c

# Compiles the driver module
driver.o: driver.c
	gcc -c driver.c

# Compiles the stack module
stack.o: stack.c stack.h
	gcc -c stack.c

# Compiles the lexer module
lexer.o: lexer.c lexer.h
	gcc -c lexer.c

# Clean target to remove object files and executable
clean:
	rm -f *.o final_exe
