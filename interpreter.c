#include <stdio.h>
#include "stack.h"
#include "matrix.h"
#include "cursor.h"
#include "interpreter.h"


/* @requires: prog est une matrice contenant un programme p2d valide,
 *   c'est à dire qui termine
 * @assigns: nothing
 * @ensures: execute les instructions du programme contenu dans la matrice prog
 */
void interpreter(matrix prog) {
  stack prog_stack;
  prog_stack = create_stack();

  cursor cur;
  cur.x = 0;
  cur.y = 0;
  cur.dir = right;

  /* This loop ends because the interpreter function is supposed to receive a
   * valid p2d program, so it should ends with an @.
   */
  while (prog.mat[cur.y][cur.x] != '@') {
    one_step(&prog, &cur, &prog_stack);
  }
}


/* Execute the next step of the p2d program
 *
 * @requires: *prog est une matrice contenant un programme p2d valide
 *  0 <= cur->x <= prog.n, 0 <= cur->y <= prog.m
 * @assigns: met à jour *prog, *cur et *prog_stack
 * @ensures: execute l'instruction correspondant au caractère courant et
 *  avance le curseur d'un pas dans la direction courante
 */
void one_step(matrix* prog, cursor* cur, stack* prog_stack) {
  char current_char = prog->mat[cur->y][cur->x];

  int a, b;
  int x, y, z;
  char c;

  switch (current_char) {
    case '+':
      push(pop(prog_stack) + pop(prog_stack), prog_stack);
      break;

    case '-':
      a = pop(prog_stack);
      b = pop(prog_stack);
      push(b-a, prog_stack);
      break;

    case '*':
      push(pop(prog_stack) * pop(prog_stack), prog_stack);
      break;

    case ':':
      a = pop(prog_stack);
      b = pop(prog_stack);
      push(a == 0 ? 42 : (int)(b/a), prog_stack);
      break;

    case '%':
      a = pop(prog_stack);
      b = pop(prog_stack);
      push(a == 0 ? 0xbadc0de : b%a, prog_stack);
      break;

    case '!':
      push(pop(prog_stack) == 0, prog_stack);
      break;

    case '`':
      a = pop(prog_stack);
      b = pop(prog_stack);
      push(b > a, prog_stack);
      break;

    case '>':
      cur->dir = right;
      break;
    case '<':
      cur->dir = left;
      break;
    case '^':
      cur->dir = up;
      break;
    case 'v':
      cur->dir = down;
      break;

    case '?':
      set_random_dir(cur);
      break;

    case '\'':
      cur->dir = pop(prog_stack) % 8;
      break;
    case ']':
      rotate_ccw(cur);
      break;
    case '[':
      rotate_cw(cur);
      break;

    case '_':
      cur->dir = pop(prog_stack) == 0 ? right : left;
      break;
    case '|':
      cur->dir = pop(prog_stack) == 0 ? down : up;
      break;
    case '/':
      cur->dir = pop(prog_stack) == 0 ? up_right : down_left;
      break;
    case '\\':
      cur->dir = pop(prog_stack) == 0 ? down_right : up_left;
      break;

    case '"':
      move_forward(cur, 1, prog->n, prog->m);
      current_char = prog->mat[cur->y][cur->x];
      while (current_char != '"') {
        push((int) current_char, prog_stack);
        move_forward(cur, 1, prog->n, prog->m);
        current_char = prog->mat[cur->y][cur->x];
      }
      break;

    case '=':
      a = pop(prog_stack);
      push(a, prog_stack);
      push(a, prog_stack);
      break;

    case '$':
      a = pop(prog_stack);
      b = pop(prog_stack);
      push(a, prog_stack);
      push(b, prog_stack);
      break;

    case ';':
      pop(prog_stack);
      break;

    case '.':
      printf("%d", pop(prog_stack));
      break;
    case ',':
      printf("%c", (char) pop(prog_stack)%255);
      break;

    case '#':
      move_forward(cur, pop(prog_stack), prog->n, prog->m);
      break;

    case 'g':
      y = pop(prog_stack);
      x = pop(prog_stack);
      if (x < prog->n && y < prog->m)
        push((int) prog->mat[y][x], prog_stack);
      else
        push(0, prog_stack);
      break;

    case 'p':
      y = pop(prog_stack);
      x = pop(prog_stack);
      z = pop(prog_stack);
      z = z > 255 ? 255 : z;
      z = z < 0 ? 0 : z;
      if (x < prog->n && y < prog->m)
        prog->mat[y][x] = (char) z;
      break;

    case '&':
      printf("Entrez un entier : ");
      scanf("%d", &a);
      push(a, prog_stack);
      break;
    case '~':
      printf("Entrez un caractère : ");
      scanf("%c", &c);
      push((int)c, prog_stack);
      break;

    case '@':
      return;
      break;
    case ' ':
      break;
    default :
      if (48 <= (int)current_char && (int)current_char <= 57)
        push((int)current_char - 48, prog_stack);
      break;
  }

  // moving cursor one step further in the current direction
  move_forward(cur, 1, prog->n, prog->m);
}
