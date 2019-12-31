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
/* @requieres: prog is a matrix of a valid p2d program
 * @assigns:
 * @ensures: runs the p2d debugger on the p2d program in the matrix prog_mat
 */
void debugger(matrix prog_mat);


/* ---------------------------- Breakpoints functions ---------------------------- */
/* @requieres: *breakpoints is of size *buffer_size, 0 <= *n_breakpoints <= *buffer_size
 * @assigns: updates *n_breakpoints, reallocates *breakpoints if needed and updates *buffer_size
 * @ensures: adds the breakpoint (x, y) to the breakpoints array if it doesn't already exist, and reallocates the array if needed
 */
int add_breakpoint(int x, int y, breakpoint* breakpoints[], size_t* n_breakpoints, size_t* buffer_size);

/* @requieres: breakpoints is of size >= *n_breakpoints
 * @assigns: removes the breakpoint (x, y) from the breakpoints array if present, shifts all
 *    the next breakpoints, and updates *n_breakpoints
 * @ensures: removes the breakpoint (x, y) from the breakpoints array if present, and updates *n_breakpoints
 */
void remove_breakpoint(int x, int y, breakpoint breakpoints[], size_t* n_breakpoints);

/* @requieres: breakpoints is of size >= n_breakpoints
 * @assigns: nothing
 * @ensures: returns the index of the breakpoint (x, y) in the breakpoints array, or -1 if
 *    it doesn't exist or isn't in the n_breakpoints first elements of the breakpoints array
 */
int get_breakpoint(int x, int y, breakpoint breakpoints[], size_t n_breakpoints);

/* @requieres: breakpoints is of size >= n_breakpoints
 * @assigns: nothing
 * @ensures: returns 1 if the breakpoint (x, y) exists in the array breakpoints, 0 otherwise
 */
int check_breakpoint(int x, int y, breakpoint breakpoints[], size_t n_breakpoints);


/* ------------------------------ History functions ------------------------------ */
/* @requieres: prog is a valid matrix of a valid p2d program, cur is a valid cursor,
 *    prog_stack is a valid stack, 0 <= index, *saved_steps has a size of *size
 * @assigns: Reallocates *saved_steps array if needed (and updates *size), adds the newly created
 *    prog_step in the saved_steps array
 * @ensures: saves the current state of the program in the *saved_steps array (and reallocates the
 *    array and updates *size if needed)
 */
void save_step(int index, matrix mat, cursor cur, stack prog_stack, prog_step* saved_steps[], size_t* size);

/* @requieres: saved_steps is of size > index, saved_steps[index] is a valid prog_step element
 * @assigns: updates *mat, *cur, *prog_stack with the saved ones in saved_steps[index]
 * @ensures: restores the index-th saved step
 */
void restore(int index, matrix* mat, cursor* cur, stack* prog_stack, prog_step saved_steps[]);

/* @requieres: *prog_mat is a valid p2d program matrix, *cur a valid cursor, *prog_stack a valid stack, *index >= 0
 * @assigns: updates *prog_mat, *cur, *prog_stack, *index and saves the new state of the program in *saved_steps (and realloc it and updates *size if needed)
 * @ensures: executes one step in the program, saves the new state of the program in *saved_steps, and updates *index
 */
void next(int* index, matrix* prog_mat, cursor* cur, stack* prog_stack, prog_step* saved_steps[], size_t* size);

/* @requieres: saved_steps is of size > index - 1, saved_steps[min(index-1, 0)] is a valid prog_step element
 * @assigns: updates *prog_mat, *cur, *prog_stack with the ones of the previous step of the program
 * @ensures: restores the (index-1)-th step of the program if index >= 0, otherwise restores initial state of the program (before execution)
 */
void prev(int* index, matrix* prog_mat, cursor* cur, stack* prog_stack, prog_step saved_steps[]);


/* ------------------------------ Screens functions ------------------------------ */
/* @requieres: nothing
 * @assigns: nothing
 * @ensures: Clears the console. Works on Linux (bash/zsh), Windows (cmd.exe) and macOS (zsh)
 */
void clear_screen();

/* @requieres: prog is a valid matrix, cur a valid cursor, and prog_stack a valid stack
 * @assigns: nothing
 * @ensures: clears the console and prints the current state of execution of the program,
 *    the program stack and the cursor
 */
void print_screen(matrix prog, cursor cur, stack prog_stack);


#endif
