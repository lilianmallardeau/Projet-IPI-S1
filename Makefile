CC = gcc -Wall
BINS = interpreter stack.o matrix.o cursor.o

all: interpreter

interpreter: interpreter.c stack.o matrix.o cursor.o
	$(CC) $^ -o $@

stack.o: stack.c
	$(CC) -c $^ -o $@

matrix.o: matrix.c
	$(CC) -c $^ -o $@

cursor.o: cursor.c
	$(CC) -c $^ -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}

test: stack.o matrix.o cursor.o
	$(CC) -c $^ -o $@
