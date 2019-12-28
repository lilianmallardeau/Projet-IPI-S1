#define HISTORY_BUFFER_SIZE       500
#define BREAKPOINTS_BUFFER_SIZE   100
#define ENABLE_CLEAR_SCREEN       1

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
 * @ensures:
 */
void debugger(matrix prog_mat) {
  stack prog_stack;
  prog_stack = create_stack();

  cursor cur;
  cur.x = 0;
  cur.y = 0;
  cur.dir = right;

  size_t n_breakpoints = 0;
  breakpoint breakpoints[BREAKPOINTS_BUFFER_SIZE];

  size_t current_history_buffer_size = HISTORY_BUFFER_SIZE;
  prog_step* saved_steps = (prog_step*) calloc(HISTORY_BUFFER_SIZE, sizeof(prog_step));
  // Saving the first step of the program, before execution
  save_step(0, prog_mat, cur, prog_stack, saved_steps, &current_history_buffer_size);
  int index = 1;

  char userinput[256];
  char last_command[256];
  char command[256];
  int nbr_args;
  int a, b;

  print_screen(prog_mat, cur, prog_stack);
  /* The loop ends because the debugger function is supposed to receive a
   * valid p2d program, so it should ends with an @.
   */
  while (prog_mat.mat[cur.y][cur.x] != '@') {
    // Getting user user input
    fgets(userinput, 256, stdin);
    nbr_args = sscanf(userinput, "%s %d %d", command, &a, &b);
    if (nbr_args == -1) {
      // If the input is empty, set the command to the last one
      nbr_args = sscanf(last_command, "%s %d %d", command, &a, &b);
      strcpy(userinput, last_command);
    }

    if (strcmp(command, "step") == 0) {
      if (nbr_args == 2 && a >= 0) {
        for (int i = 0; i < a; i++) {
          next(&index, &prog_mat, &cur, &prog_stack, saved_steps, &current_history_buffer_size);
          // Checking if the cursor is currently on a breakpoint
          if (check_breakpoint(cur.x, cur.y, breakpoints, n_breakpoints))
            break;
        }
      } else {
        next(&index, &prog_mat, &cur, &prog_stack, saved_steps, &current_history_buffer_size);
      }
    }
    else if (strcmp(command, "run") == 0) {
      do {
        next(&index, &prog_mat, &cur, &prog_stack, saved_steps, &current_history_buffer_size);
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
          // Checking if the cursor is currently on a breakpoint
          if (check_breakpoint(cur.x, cur.y, breakpoints, n_breakpoints))
            break;
        }
      } else {
        prev(&index, &prog_mat, &cur, &prog_stack, saved_steps);
      }
    }
    else if (strcmp(command, "breakpoint") == 0) {
      if (nbr_args == 3) {
        add_breakpoint(a, b, breakpoints, &n_breakpoints);
      }
    }
    else if (strcmp(command, "removebp") == 0) {
      if (nbr_args == 3) {
        remove_breakpoint(a, b, breakpoints, &n_breakpoints);
      }
    }

    // Saving the last user command
    strcpy(last_command, userinput);

    // Clearing screen and printing the new program state
    clear_screen();
    print_screen(prog_mat, cur, prog_stack);
  }
}

/* ------------------------------------------------------------------------------- */
/* ---------------------------- Breakpoints functions ---------------------------- */
/* ------------------------------------------------------------------------------- */

int add_breakpoint(int x, int y, breakpoint breakpoints[], size_t* len) {
  if (!check_breakpoint(x, y, breakpoints, *len)) {
    breakpoint new_breakpoint;
    new_breakpoint.x = x;
    new_breakpoint.y = y;
    // Here: reallocate if needed
    breakpoints[*len] = new_breakpoint;
    (*len)++;
    return 1;
  } else {
    return 0;
  }
}

void remove_breakpoint(int x, int y, breakpoint breakpoints[], size_t* len) {
  if (check_breakpoint(x, y, breakpoints, *len)) {
    int index = get_breakpoint(x, y, breakpoints, *len);
    for (int i = index; i < *len - 1; i++) {
      breakpoints[i] = breakpoints[i+1];
    }
    (*len)--;
  }
}

int get_breakpoint(int x, int y, breakpoint breakpoints[], size_t len) {
  for (int i = 0; i < len; i++) {
    if (breakpoints[i].x == x && breakpoints[i].y == y)
      return i;
  }
  return -1;
}

int check_breakpoint(int x, int y, breakpoint breakpoints[], size_t len) {
  return get_breakpoint(x, y, breakpoints, len) != -1;
}


/* ------------------------------------------------------------------------------- */
/* ------------------------------ History functions ------------------------------ */
/* ------------------------------------------------------------------------------- */

/* @requieres: prog is a matrix of a valid p2d program, cur is a valid cursor,
 *    prog_stack is a valid stack, , 0 <= index
 * @assigns: Reallocate saved_steps array if needed.
 * @ensures:
 */
void save_step(int index, matrix mat, cursor cur, stack prog_stack, prog_step saved_steps[], size_t* size) {
  prog_step new_step;
  new_step.mat = copy_matrix(mat);
  new_step.cur = cur;
  new_step.stack = copy_stack(prog_stack);

  // If needed, reallocate the memory for the array
  if (*size <= index) {
    saved_steps = realloc(saved_steps, (*size + HISTORY_BUFFER_SIZE) * sizeof(struct prog_step));
    if (saved_steps != NULL) {
      *size += HISTORY_BUFFER_SIZE;
    } else {
      fprintf(stderr, "Error while reallocating program history array. Exiting.");
      exit(2);
    }
  }

  saved_steps[index] = new_step;
}

/* @requieres:
 * @assigns:
 * @ensures:
 */
void restore(int index, matrix* mat, cursor* cur, stack* prog_stack, prog_step saved_steps[]) {
  *mat = copy_matrix(saved_steps[index].mat);
  *cur = saved_steps[index].cur;
  *prog_stack = copy_stack(saved_steps[index].stack);
}

/* @requieres:
 * @assigns:
 * @ensures:
 */
void next(int* index, matrix* prog_mat, cursor* cur, stack* prog_stack, prog_step saved_steps[], size_t* size) {
  one_step(prog_mat, cur, prog_stack);
  save_step(*index, *prog_mat, *cur, *prog_stack, saved_steps, size);
  (*index)++;
}

/* @requieres:
 * @assigns:
 * @ensures:
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
  // OUI c'est crade je sais mais ça fonctionne
  #endif
}


/* @requieres: prog is a valid matrix, cur a valid cursor, and prog_stack a valid stack
 * @assigns: nothing
 * @ensures: clears the console and prints the current state of execution of the program
 */
void print_screen(matrix prog, cursor cur, stack prog_stack) {
  printf("\n");

  // Printing header and character indicator
  for (int i = 0; i < prog.n + 5; i+=5) {
    printf("%5d", i);
  }
  printf("\n    ");
  for (int i = 0; i < prog.n + 5; i++) {
    if (i == cur.x)
      printf(BOLDYELLOW "v" RESET);
    else
      printf("%c", i%5 == 0 ? '|' : ' ');
  }
  printf("\n");

  // Printing program matrix
  for (int i = 0; i < prog.m; i++) {
    if (i%5 == 0)
      printf("%3d%s", i, cur.y == i ? BOLDYELLOW ">" RESET : "-");
    else
      printf("%3c%s", ' ', cur.y == i ? BOLDYELLOW ">" RESET : " ");

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
