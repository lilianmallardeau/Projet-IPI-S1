#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include "stack.c"
#include "matrix.c"
#include "cursor.c"

void interpreter(matrix prog);

/*************************** Main ****************************/
int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Usage :\n");
    printf("%s prog.p2d\n", argv[0]);
    printf("prog.p2d: the 2d program you want to run");
    printf("\n");
    exit(1);
  }

  FILE* file = fopen(argv[1], "r");
  char* firstline = NULL;
  size_t firstline_size = 0;
  getline(&firstline, &firstline_size, file);
  int m, n;
  sscanf(firstline, "%d %d", &n, &m);

  matrix mat;
  mat = create_matrix(m, n);
  //fill_matrix(&mat, ' ');
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      mat.mat[i][j] = (char) getc(file);
    }
    getc(file);
  }

  fclose(file);
  // print_matrix(mat);
  interpreter(mat);

  return 0;
}

/************************ Interpreter ************************/
void interpreter(matrix prog) {
  stack prog_stack;
  prog_stack = create_stack();

  cursor cur;
  cur.x = 0;
  cur.y = 0;
  cur.dir = right;

  char current_char = prog.mat[cur.x][cur.y];

  int a, b;
  int x, y, z;
  char c;
  // add parameters if needed

  while (current_char != '@') {
    switch (current_char) {
      case '+':
        push(pop(&prog_stack) + pop(&prog_stack), &prog_stack);
        break;

      case '-':
        a = pop(&prog_stack);
        b = pop(&prog_stack);
        push(b-a, &prog_stack);
        break;

      case '*':
        push(pop(&prog_stack) * pop(&prog_stack), &prog_stack);
        break;

      case ':':
        a = pop(&prog_stack);
        b = pop(&prog_stack);
        push(a == 0 ? 42 : (int)(b/a), &prog_stack);
        break;

      case '%':
        a = pop(&prog_stack);
        b = pop(&prog_stack);
        push(a == 0 ? 0xbadc0de : b%a, &prog_stack);
        break;

      case '!':
        push(pop(&prog_stack) == 0, &prog_stack);
        break;

      case '`':
        a = pop(&prog_stack);
        b = pop(&prog_stack);
        push(b > a, &prog_stack);
        break;

      case '>':
        cur.dir = right;
        break;
      case '<':
        cur.dir = left;
        break;
      case '^':
        cur.dir = up;
        break;
      case 'v':
        cur.dir = down;
        break;

      case '?':
        set_random_dir(&cur);
        break;

      case '\'':

        break;
      case ']':
        rotate_ccw(&cur);
        break;
      case '[':
        rotate_cw(&cur);
        break;

      case '_':
        cur.dir = pop(&prog_stack) == 0 ? right : left;
        break;
      case '|':
        cur.dir = pop(&prog_stack) == 0 ? down : up;
        break;
      case '/':
        cur.dir = pop(&prog_stack) == 0 ? up_right : down_left;
        break;
      case '\\':
        cur.dir = pop(&prog_stack) == 0 ? down_right : up_left;
        break;

      case '"':
        while (current_char != '"') {
          push((int) current_char, &prog_stack);
          move_forward(&cur, 1, prog.m, prog.n);
          current_char = prog.mat[cur.x][cur.y];
        }
        break;

      case '=':
        a = pop(&prog_stack);
        push(a, &prog_stack);
        push(a, &prog_stack);
        break;

      case '$':
        a = pop(&prog_stack);
        b = pop(&prog_stack);
        push(a, &prog_stack);
        push(b, &prog_stack);
        break;

      case ';':
        pop(&prog_stack);
        break;

      case '.':
        printf("%d", pop(&prog_stack));
        break;
      case ',':
        printf("%c", (char) pop(&prog_stack)%255);
        break;

      case '#':
        a = pop(&prog_stack);
        move_forward(&cur, a, prog.m, prog.n);
        break;

      case 'g':
        x = pop(&prog_stack);
        y = pop(&prog_stack);
        if (x < prog.m && y < prog.n)
          push((int) prog.mat[x][y], &prog_stack);
        else
          push(0, &prog_stack);
        break;

      case 'p':
        x = pop(&prog_stack);
        y = pop(&prog_stack);
        z = pop(&prog_stack);
        z = z > 255 ? 255 : z;
        z = z < 0 ? 0 : z;
        if (x < prog.m && y < prog.n)
          prog.mat[x][y] = (char) z;
        break;

      case '&':
        printf("Entrez un entier : ");
        scanf("%d", &a);
        push(a, &prog_stack);
        break;
      case '~':
        printf("Entrez un caractère : ");
        scanf("%c", &c);
        push((int)c, &prog_stack);
        break;

      case '@':
        return;
        break;
      case ' ':
        break;
    }
    if (48 <= (int)current_char && (int)current_char <= 57) {
      push((int)current_char - 48, &prog_stack);
    }

    move_forward(&cur, 1, prog.m, prog.n);
    current_char = prog.mat[cur.x][cur.y];
  }
}
