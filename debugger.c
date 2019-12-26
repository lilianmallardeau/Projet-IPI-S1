#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "matrix.h"
#include "cursor.h"
#include "colors.h"
#include "interpreter.h"
#include "debugger.h"


/* @requieres: nothing
 * @assigns: nothing
 * @ensures: Clears the console. Work on Linux, Windows and macOS
 */
void clean_screen() {
  system("@cls||clear");
}

/* @requieres: prog is a valid matrix, cur a valid cursor, and prog_stack a valid stack
 * @assigns: nothing
 * @ensures: clears the console and prints the current state of execution of the program
 */
void print_screen(matrix prog, cursor cur, stack prog_stack) {
  // Clearing screen
  clean_screen();
  printf("\n");

  // Printing header and character indicator
  for (int i = 0; i < prog.n + 5; i+=5) {
    printf("%5d", i);
  }
  printf("\n    ");
  for (int i = 0; i < prog.n + 5; i++) {
    if (i == cur.x)
      printf("v");
    else
      printf("%c", i%5 == 0 ? '|' : ' ');
  }
  printf("\n");

  // Printing program matrix
  for (int i = 0; i < prog.m; i++) {
    if (i%5 == 0)
      printf("%3d%c", i, cur.y == i ? '>' : '-');
    else
      printf("%4c", cur.y == i ? '>' : ' ');

    for (int j = 0; j < prog.n; j++)
      printf((cur.x == j && cur.y == i) ? BOLDRED "%c" RESET : "%c", prog.mat[i][j]);
    printf("\n");
  }
  printf("\n");

  // Printing program stack
  while (!is_stack_empty(prog_stack)) {
    printf(" %d (%c) |", prog_stack->head, (char) (prog_stack->head % 255));
    prog_stack = prog_stack->tail;
  }
  printf("\n");

  // Printing cursor coordinates
  printf("x: %d, y: %d", cur.x, cur.y);
  printf("\n");  // And a newline, because it's beautiful
}


/* @requieres: prog is a matrix of a valid p2d program
 * @assigns:
 * @ensures:
 */
void debugger(matrix prog) {
  stack prog_stack;
  prog_stack = create_stack();

  cursor cur;
  cur.x = 0;
  cur.y = 0;
  cur.dir = right;

  char* command = "\0";

  /* The loop ends because the interpreter function is supposed to receive a
   * valid p2d program, so it should ends with an @.
   */
  while (prog.mat[cur.y][cur.x] != '@') {
    one_step(&prog, &cur, &prog_stack);
    print_screen(prog, cur, prog_stack);
    // get user command, saved last command, save each step, ...
    getchar();
  }
}
