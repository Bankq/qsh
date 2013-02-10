CC=gcc
FLAGS= -Wall -g
OBJECTS= myshell.o utils.o io.o parser.o run.o

all: $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o myshell 
myshell.o: include/myshell.h src/myshell.c
	$(CC) $(FLAGS) -c src/myshell.c -o myshell.o
utils.o: include/utils.h src/utils.c
	$(CC) $(FLAGS) -c src/utils.c -o utils.o
io.o: include/io.h src/io.c
	$(CC) $(FLAGS) -c src/io.c -o io.o
parser.o: include/parser.h src/parser.c
	$(CC) $(FLAGS) -c src/parser.c -o parser.o
run.o: include/run.h src/run.c
	$(CC) $(FLAGS) -c src/run.c -o run.o
clean:
	rm -f $(OBJECTS)
