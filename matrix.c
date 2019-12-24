#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

/* @requieres: m > 0, n > 0
 * @assigns: allocates the memory needed for the matrix_elem** 2d array
 * @ensures: returns the freshly created matrix
 */
matrix create_matrix(int m, int n) {
  matrix mat;
  mat.m = m;
  mat.n = n;
  mat.mat = (matrix_elem**) malloc(m*sizeof(matrix_elem*));
  for (int i = 0; i < m; i++) {
    mat.mat[i] = (matrix_elem*) malloc(n*sizeof(matrix_elem));
  }
  return mat;
}

/* @requieres: *mat is a valid matrix
 * @assigns: sets mat->mat[x][y] to e for all (x, y) in [1..m] * [1..n]
 * @ensures: fills the matrix with e
 */
void fill_matrix(matrix* mat, matrix_elem e) {
  for (int i = 0; i < mat->m; i++) {
    for (int j = 0; j < mat->n; j++) {
      mat->mat[i][j] = e;
    }
  }
}

/* @requieres: mat is a valid matrix
 * @assigns: allocates memory for a new matrix with same size as m and copies
 *  each element of m in the new matrix
 * @ensures: returns a copy of the matrix m
 */
matrix copy_matrix(matrix m) {
  matrix new_mat = create_matrix(m.m, m.n);
  for (int i = 0; i < m.m; i++) {
    for (int j = 0; j < m.n; j++) {
      new_mat.mat[i][j] = m.mat[i][j];
    }
  }
  return new_mat;
}

/* @requieres: x <= n, y <= m, mat is a valid matrix
 * @assigns: nothing
 * @ensures: returns the value of mat->mat[x-1][y-1]
 */
matrix_elem get_matrix_elem(matrix mat, int x, int y) {
  return mat.mat[x-1][y-1];
}

/* @requieres: x <= m, y <= n, *mat is a valid matrix
 * @assigns: changes the value of mat->mat[x-1][y-1] to e
 * @ensures: changes the value of mat->mat[x-1][y-1] to e
 */
void set_matrix_elem(matrix* mat, int x, int y, matrix_elem e) {
  mat->mat[x-1][y-1] = e;
}

/* @requieres: m1 and m2 are valid matrix
 * @assigns: nothing
 * @ensures: returns 1 if both matrices are equal, 0 otherwise
 */
int is_matrix_equal(matrix m1, matrix m2) {
  if (m1.m != m2.m || m1.n != m2.n)
    return 0;
  for (int i = 0; i < m1.m; i++) {
    for (int j = 0; j < m1.n; j++) {
      if (m1.mat[i][j] != m2.mat[i][j])
        return 0;
    }
  }
  return 1;
}

/* @requieres: x <= m, y <= n, *mat is a valid matrix
 * @assigns: changes the value of mat->mat[x][y] to e
 * @ensures: changes the value of mat->mat[x][y] to e
 */
void print_matrix(matrix mat) {
  for (int i = 0; i < mat.m; i++) {
    for (int j = 0; j < mat.n; j++) {
      printf("%c", mat.mat[i][j]);
    }
    printf("\n");
  }
}
