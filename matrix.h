#ifndef _MATRIX_H
#define _MATRIX_H

/* Defining the type of the matrices' elements */
typedef char matrix_elem;

/* A matrix is defined by a struct, with 2 integers m and n representing the numbers
 * of rows and columns, and a 2D array as a double pointer.
 */
typedef struct matrix matrix;
struct matrix {
  int m, n;
  matrix_elem** mat;
};

/* @requieres: m > 0, n > 0
 * @assigns: allocates the memory needed for the matrix_elem** 2d array
 * @ensures: returns the freshly created matrix
 * /!\ The matrix' content is not initialized! Use `fill_matrix` to fill the matrix.
 */
matrix create_matrix(int m, int n);

/* @requieres: *mat is a valid matrix
 * @assigns: sets mat->mat[x][y] to e for all (x, y) in [1..m] * [1..n]
 * @ensures: fills the matrix with e
 */
void fill_matrix(matrix* mat, matrix_elem e);

/* @requieres: mat is a valid matrix
 * @assigns: allocates memory for a new matrix with same size as m and copies
 *  each element of m in the new matrix
 * @ensures: returns a copy of the matrix m
 */
matrix copy_matrix(matrix m);

/* @requieres: 0 < x <= mat.m, 0 < y <= mat.n, mat is a valid matrix
 * @assigns: nothing
 * @ensures: returns the value of mat.mat[x-1][y-1]
 */
matrix_elem get_matrix_elem(matrix mat, int x, int y);

/* @requieres: 0 < x <= mat->m, 0 < y <= mat->n, *mat is a valid matrix
 * @assigns: changes the value of mat->mat[x-1][y-1] to e
 * @ensures: changes the value of mat->mat[x-1][y-1] to e
 */
void set_matrix_elem(matrix* mat, int x, int y, matrix_elem e);

/* @requieres: m1 and m2 are valid matrix
 * @assigns: nothing
 * @ensures: returns 1 if both matrices are equal, 0 otherwise
 */
int is_matrix_equal(matrix m1, matrix m2);

/* @requieres: mat is a valid matrix
 * @assigns: nothing
 * @ensures: prints the matrix m
 */
void print_matrix(matrix mat);

#endif
