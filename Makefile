CC = gcc -Wall -Wextra -g
BINS = interpreter debugger *.o test

all: interpreter

interpreter: interpreter.c stack.o matrix.o cursor.o
	$(CC) $^ -o $@

debugger: debugger.c stack.o matrix.o cursor.o
	$(CC) $^ -o $@

%.o: %.c %.h
	$(CC) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf ${BINS}

re: clean all
