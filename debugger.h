#ifndef _DEBUGGER_H
#define _DEBUGGER_H

#include "stack.h"
#include "matrix.h"
#include "cursor.h"

/* ------------------------------------ Types ------------------------------------ */

/* A step in the program is saved with the program's matrix, the current cursor and the current stack */
typedef struct prog_step prog_step;
struct prog_step {
  matrix mat;
  cursor cur;
  stack stack;
};

/* A breakpoint is defined with the two coordinates x and y of the point */
typedef struct breakpoint breakpoint;
struct breakpoint {
  int x, y;
};

/* ---------------------------------- Debugger ----------------------------------- */
void debugger(matrix prog_mat);

/* ---------------------------- Breakpoints functions ---------------------------- */
int add_breakpoint(int x, int y, breakpoint* breakpoints[], size_t* n_breakpoints, size_t* buffer_size);
void remove_breakpoint(int x, int y, breakpoint breakpoints[], size_t* len);
int get_breakpoint(int x, int y, breakpoint breakpoints[], size_t len);
int check_breakpoint(int x, int y, breakpoint breakpoints[], size_t len);

/* ------------------------------ History functions ------------------------------ */
void save_step(int index, matrix mat, cursor cur, stack prog_stack, prog_step* saved_steps[], size_t* size);
void restore(int index, matrix* mat, cursor* cur, stack* prog_stack, prog_step saved_steps[]);
void next(int* index, matrix* prog_mat, cursor* cur, stack* prog_stack, prog_step* saved_steps[], size_t* size);
void prev(int* index, matrix* prog_mat, cursor* cur, stack* prog_stack, prog_step saved_steps[]);

/* ------------------------------ Screens functions ------------------------------ */
void clear_screen();
void print_screen(matrix prog, cursor cur, stack prog_stack);


#endif
