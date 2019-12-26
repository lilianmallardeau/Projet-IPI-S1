#define PRINT_PROG_MATRIX_BEFORE_RUN 0
#define FORCE_DEBUG                  0

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "interpreter.h"
#include "debugger.h"


/*********************************************************************/
/******************************* Main ********************************/
/*********************************************************************/
int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Usage :\n");
    printf("%s prog.p2d\n", argv[0]);
    printf("prog.p2d: the p2d program you want to run\n");
    printf("\n");
    exit(1);
  }

  int debug_mode;
  if (argc == 2) {
    debug_mode = 0;
  } else {
    debug_mode = (argv[2] == "1" || FORCE_DEBUG);
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
  if (PRINT_PROG_MATRIX_BEFORE_RUN)
    print_matrix(mat);

  if (debug_mode)
    debugger(mat);
  else
    interpreter(mat);

  return 0;
}
