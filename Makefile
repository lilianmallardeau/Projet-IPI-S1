CC = gcc -Wall -Wextra
BINS = interpreter stack.o matrix.o cursor.o test

all: interpreter

interpreter: interpreter.c stack.o matrix.o cursor.o
	$(CC) $^ -o $@

stack.o: stack.c stack.h
	$(CC) -c $< -o $@

matrix.o: matrix.c matrix.h
	$(CC) -c $< -o $@

cursor.o: cursor.c cursor.h
	$(CC) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}

test: test.c stack.o matrix.o cursor.o
	$(CC) $^ -o $@
