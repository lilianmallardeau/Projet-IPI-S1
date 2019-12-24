#ifndef _MATRIX_H
#define _MATRIX_H

typedef char matrix_elem;

typedef struct matrix matrix;
struct matrix {
  int m, n;
  matrix_elem** mat;
};

matrix create_matrix(int m, int n);

void fill_matrix(matrix* mat, matrix_elem e);

matrix copy_matrix(matrix m);

matrix_elem get_matrix_elem(matrix mat, int x, int y);

void set_matrix_elem(matrix* mat, int x, int y, matrix_elem e);

int is_matrix_equal(matrix m1, matrix m2);

void print_matrix(matrix mat);


#endif
