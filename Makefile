CC = gcc -Wall
BINS = p2d *.o

all: p2d

p2d: main.c interpreter.o debugger.o stack.o matrix.o cursor.o
	$(CC) $^ -o $@

%.o: %.c %.h
	$(CC) -c $< -o $@

debugger: debugger.o

clean:
	@echo "Cleaning up..."
	rm -rvf ${BINS}

re: clean all
