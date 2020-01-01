#define HISTORY_BUFFER_SIZE       500  /* If the history buffer is not large enough, it will be reallocated */
#define BREAKPOINTS_BUFFER_SIZE   100  /* If the breakpoints buffer is not large enough, it will be reallocated */
#define ENABLE_CLEAR_SCREEN       1    /* Clears screen between each iteration */
#define CLEAR_SCREEN_AT_BEGINNING 1    /* Only works if ENABLE_CLEAR_SCREEN == 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "matrix.h"
#include "cursor.h"
#include "colors.h"
#include "interpreter.h"
#include "debugger.h"


/* @requieres: prog is a matrix of a valid p2d program
 * @assigns:
 * @ensures: runs the p2d debugger on the p2d program in the matrix prog_mat
 */
void debugger(matrix prog_mat) {
  stack prog_stack;
  prog_stack = create_stack();

  cursor cur;
  cur.x = 0;
  cur.y = 0;
  cur.dir = right;

  size_t n_breakpoints = 0;
  size_t breakpoints_buffer_size = BREAKPOINTS_BUFFER_SIZE;
  breakpoint* breakpoints = (breakpoint*) calloc(BREAKPOINTS_BUFFER_SIZE, sizeof(breakpoint));

  size_t history_buffer_size = HISTORY_BUFFER_SIZE;
  prog_step* saved_steps = (prog_step*) calloc(HISTORY_BUFFER_SIZE, sizeof(prog_step));
  /* Saving the first step of the program, before execution */
  save_step(0, prog_mat, cur, prog_stack, &saved_steps, &history_buffer_size);
  int index = 1;

  char userinput[256];
  char last_command[256];
  char command[256];
  int nbr_args;
  int a, b;

  #if CLEAR_SCREEN_AT_BEGINNING == 1
  clear_screen();
  #endif
  print_screen(prog_mat, cur, prog_stack);

  /* The loop ends because the debugger function is supposed to receive a
   * valid p2d program, so it should ends with an @.
   */
  while (prog_mat.mat[cur.y][cur.x] != '@') {
    /* Getting user user input */
    fgets(userinput, 256, stdin);
    nbr_args = sscanf(userinput, "%s %d %d", command, &a, &b);
    if (nbr_args == -1) {
      /* If the input is empty, set the command to the last one */
      nbr_args = sscanf(last_command, "%s %d %d", command, &a, &b);
      strcpy(userinput, last_command);
    }

    if (strcmp(command, "step") == 0) {
      if (nbr_args == 2 && a >= 0) {
        for (int i = 0; i < a; i++) {
          next(&index, &prog_mat, &cur, &prog_stack, &saved_steps, &history_buffer_size);
          /* Checking if the cursor is currently on a breakpoint */
          if (check_breakpoint(cur.x, cur.y, breakpoints, n_breakpoints))
            break;
        }
      } else {
        next(&index, &prog_mat, &cur, &prog_stack, &saved_steps, &history_buffer_size);
      }
    }
    else if (strcmp(command, "run") == 0) {
      do {
        next(&index, &prog_mat, &cur, &prog_stack, &saved_steps, &history_buffer_size);
      } while (!check_breakpoint(cur.x, cur.y, breakpoints, n_breakpoints) && prog_mat.mat[cur.y][cur.x] != '@');
    }
    else if (strcmp(command, "restart") == 0) {
      restore(0, &prog_mat, &cur, &prog_stack, saved_steps);
      index = 0;
    }
    else if (strcmp(command, "quit") == 0) {
      printf("Exiting...\n");
      exit(0);
    }
    else if (strcmp(command, "prec") == 0) {
      if (nbr_args == 2 && a >= 0) {
        for (int i = 0; i < a; i++) {
          prev(&index, &prog_mat, &cur, &prog_stack, saved_steps);
          /* Checking if the cursor is currently on a breakpoint */
          if (check_breakpoint(cur.x, cur.y, breakpoints, n_breakpoints))
            break;
        }
      } else {
        prev(&index, &prog_mat, &cur, &prog_stack, saved_steps);
      }
    }
    else if (strcmp(command, "breakpoint") == 0) {
      if (nbr_args == 3) {
        add_breakpoint(a, b, &breakpoints, &n_breakpoints, &breakpoints_buffer_size);
      }
    }
    else if (strcmp(command, "removebp") == 0) {
      if (nbr_args == 3) {
        remove_breakpoint(a, b, breakpoints, &n_breakpoints);
      }
    }

    /* Saving the last user command */
    strcpy(last_command, userinput);

    /* Clearing screen and printing the new program state */
    clear_screen();
    print_screen(prog_mat, cur, prog_stack);
  }
}

/* ------------------------------------------------------------------------------- */
/* ---------------------------- Breakpoints functions ---------------------------- */
/* ------------------------------------------------------------------------------- */

/* @requieres: *breakpoints is of size *buffer_size, 0 <= *n_breakpoints <= *buffer_size
 * @assigns: updates *n_breakpoints, reallocates *breakpoints if needed and updates *buffer_size
 * @ensures: adds the breakpoint (x, y) to the breakpoints array if it doesn't already exist, and reallocates the array if needed
 */
int add_breakpoint(int x, int y, breakpoint* breakpoints[], size_t* n_breakpoints, size_t* buffer_size) {
  if (!check_breakpoint(x, y, *breakpoints, *n_breakpoints)) {
    breakpoint new_breakpoint;
    new_breakpoint.x = x;
    new_breakpoint.y = y;
    
    /* If needed, reallocate the memory for the array */
    /* The while loop ends because either the realloc fails and the program ends,
     *    either it succeeds each time, until the array is large enough */
    while (*buffer_size <= *n_breakpoints) {
      *breakpoints = realloc(*breakpoints, (*buffer_size + BREAKPOINTS_BUFFER_SIZE) * sizeof(struct breakpoint));
      if (*breakpoints != NULL) {
        *buffer_size += BREAKPOINTS_BUFFER_SIZE;
      } else {
        fprintf(stderr, "Error while reallocating breakpoints array. Exiting.");
        exit(2);
      }
    }
    
    (*breakpoints)[*n_breakpoints] = new_breakpoint;
    (*n_breakpoints)++;
    return 1;
  } else {
    return 0;
  }
}

/* @requieres: breakpoints is of size >= *n_breakpoints
 * @assigns: removes the breakpoint (x, y) from the breakpoints array if present, shifts all
 *    the next breakpoints, and updates *n_breakpoints
 * @ensures: removes the breakpoint (x, y) from the breakpoints array if present, and updates *n_breakpoints
 */
void remove_breakpoint(int x, int y, breakpoint breakpoints[], size_t* n_breakpoints) {
  if (check_breakpoint(x, y, breakpoints, *n_breakpoints)) {
    int index = get_breakpoint(x, y, breakpoints, *n_breakpoints);
    for (int i = index; i < *n_breakpoints - 1; i++) {
      breakpoints[i] = breakpoints[i+1];
    }
    (*n_breakpoints)--;
  }
}

/* @requieres: breakpoints is of size >= n_breakpoints
 * @assigns: nothing
 * @ensures: returns the index of the breakpoint (x, y) in the breakpoints array, or -1 if
 *    it doesn't exist or isn't in the n_breakpoints first elements of the breakpoints array
 */
int get_breakpoint(int x, int y, breakpoint breakpoints[], size_t n_breakpoints) {
  for (int i = 0; i < n_breakpoints; i++) {
    if (breakpoints[i].x == x && breakpoints[i].y == y)
      return i;
  }
  return -1;
}

/* @requieres: breakpoints is of size >= n_breakpoints
 * @assigns: nothing
 * @ensures: returns 1 if the breakpoint (x, y) exists in the array breakpoints, 0 otherwise
 */
int check_breakpoint(int x, int y, breakpoint breakpoints[], size_t n_breakpoints) {
  return get_breakpoint(x, y, breakpoints, n_breakpoints) != -1;
}


/* ------------------------------------------------------------------------------- */
/* ------------------------------ History functions ------------------------------ */
/* ------------------------------------------------------------------------------- */

/* @requieres: prog is a valid matrix of a valid p2d program, cur is a valid cursor,
 *    prog_stack is a valid stack, 0 <= index, *saved_steps has a size of *size
 * @assigns: Reallocates *saved_steps array if needed (and updates *size), adds the newly created
 *    prog_step in the saved_steps array
 * @ensures: saves the current state of the program in the *saved_steps array (and reallocates the
 *    array and updates *size if needed)
 */
void save_step(int index, matrix mat, cursor cur, stack prog_stack, prog_step* saved_steps[], size_t* size) {
  prog_step new_step;
  new_step.mat = copy_matrix(mat);
  new_step.cur = cur;
  new_step.stack = copy_stack(prog_stack);

  /* If needed, reallocate the memory for the array */
  /* The while loop ends because either the realloc fails and the program ends,
   *    either it succeeds each time, until the array is large enough */
  while (*size <= index) {
    *saved_steps = realloc(*saved_steps, (*size + HISTORY_BUFFER_SIZE) * sizeof(struct prog_step));
    if (*saved_steps != NULL) {
      *size += HISTORY_BUFFER_SIZE;
    } else {
      fprintf(stderr, "Error while reallocating program history array. Exiting.");
      exit(2);
    }
  }

  (*saved_steps)[index] = new_step;
}

/* @requieres: saved_steps is of size > index, saved_steps[index] is a valid prog_step element
 * @assigns: updates *mat, *cur, *prog_stack with the saved ones in saved_steps[index]
 * @ensures: restores the index-th saved step
 */
void restore(int index, matrix* mat, cursor* cur, stack* prog_stack, prog_step saved_steps[]) {
  *mat = copy_matrix(saved_steps[index].mat);
  *cur = saved_steps[index].cur;
  *prog_stack = copy_stack(saved_steps[index].stack);
}

/* @requieres: *prog_mat is a valid p2d program matrix, *cur a valid cursor, *prog_stack a valid stack, *index >= 0
 * @assigns: updates *prog_mat, *cur, *prog_stack, *index and saves the new state of the program in *saved_steps (and realloc it and updates *size if needed)
 * @ensures: executes one step in the program, saves the new state of the program in *saved_steps, and updates *index
 */
void next(int* index, matrix* prog_mat, cursor* cur, stack* prog_stack, prog_step* saved_steps[], size_t* size) {
  one_step(prog_mat, cur, prog_stack);
  save_step(*index, *prog_mat, *cur, *prog_stack, saved_steps, size);
  (*index)++;
}

/* @requieres: saved_steps is of size > index - 1, saved_steps[min(index-1, 0)] is a valid prog_step element
 * @assigns: updates *prog_mat, *cur, *prog_stack with the ones of the previous step of the program
 * @ensures: restores the (index-1)-th step of the program if index >= 0, otherwise restores initial state of the program (before execution)
 */
void prev(int* index, matrix* prog_mat, cursor* cur, stack* prog_stack, prog_step saved_steps[]) {
  if (*index - 1 >= 0) {
    restore(*index - 1, prog_mat, cur, prog_stack, saved_steps);
    (*index)--;
  }
}


/* ------------------------------------------------------------------------------- */
/* ------------------------------ Screens functions ------------------------------ */
/* ------------------------------------------------------------------------------- */

/* @requieres: nothing
 * @assigns: nothing
 * @ensures: Clears the console. Works on Linux (bash/zsh), Windows (cmd.exe) and macOS (zsh)
 */
void clear_screen() {
  #if ENABLE_CLEAR_SCREEN == 1
  system("@cls||clear");
  /* OUI c'est crade je sais mais ça fonctionne */
  #endif
}


/* @requieres: prog is a valid matrix, cur a valid cursor, and prog_stack a valid stack
 * @assigns: nothing
 * @ensures: clears the console and prints the current state of execution of the program,
 *    the program stack and the cursor
 */
void print_screen(matrix prog, cursor cur, stack prog_stack) {
  printf("\n");

  /* Printing header and character indicator */
  for (int i = 0; i < prog.n + 5; i+=5) {
    printf("%5d", i);
  }
  printf("\n    ");
  for (int i = 0; i < prog.n + 5; i++) {
    if (i == cur.x)
      printf(BOLDRED "v" RESET);
    else
      printf("%c", i%5 == 0 ? '|' : ' ');
  }
  printf("\n");

  /* Printing program matrix */
  for (int i = 0; i < prog.m; i++) {
    if (i%5 == 0)
      printf("%3d%s", i, cur.y == i ? BOLDRED ">" RESET : "-");
    else
      printf("%3c%s", ' ', cur.y == i ? BOLDRED ">" RESET : " ");

    for (int j = 0; j < prog.n; j++)
      printf((cur.x == j && cur.y == i) ? INVERTED "%c" RESET : "%c", prog.mat[i][j]);
    printf("\n");
  }
  printf("\n");

  /* Printing program stack */
  while (!is_stack_empty(prog_stack)) {
    printf(" %d (%c) |", prog_stack->head, (char) (prog_stack->head % 255));
    prog_stack = prog_stack->tail;
  }
  printf("\n");

  /* Printing cursor coordinates */
  printf("x: %d, y: %d", cur.x, cur.y);
  printf("\n");  /* And a newline, because it's beautiful */
}
