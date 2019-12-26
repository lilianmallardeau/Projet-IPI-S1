#ifndef _DEBUGGER_H
#define _DEBUGGER_H

#include "stack.h"
#include "matrix.h"
#include "cursor.h"

typedef struct prog_step prog_step;

/* A step in the program is saved with the program's matrix, the current cursor and the current stack */
struct prog_step {
  matrix mat;
  cursor cur;
  stack prog_stack;
};


void clean_screen();

void print_screen(matrix prog, cursor cur, stack prog_stack);

void debugger(matrix prog);

#endif
