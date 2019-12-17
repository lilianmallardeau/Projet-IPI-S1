#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "matrix.h"

enum direction {up, down, left, right, up_left, up_right, down_left, down_right};
typedef struct coordinate coordinate;
struct coordinate {
  int x, y;
};

/************************ Main ************************/
int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Usage :\n");
    printf("%s \n prog.p2d", argv[0]);
  }

  FILE* prog = fopen(argv[0], "r");
  int m, n;
  fscanf(prog, "%d %d", &n, &m);
  getc(prog);

  matrix mat;
  mat = create_matrix(m, n);
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      set_elem(&mat, i, j, (char) getc(prog));
    }
    getc(prog);
  }

  enum direction dir = right;
  coordinate p;
  p.x = 1;
  p.y = 1;
  /* code */
  return 0;
}
