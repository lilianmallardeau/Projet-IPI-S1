#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "stack.h"
#include "matrix.h"
#include "cursor.h"


/* @requires: prog est une matrice contenant un programme p2d valide,
 *   c'est à dire qui termine
 * @assigns: nothing
 * @ensures: execute les instructions du programme contenu dans la matrice prog
 */
void interpreter(matrix prog);


/* Execute the next step of the p2d program
 *
 * @requires: *prog est une matrice contenant un programme p2d valide
 *  0 <= cur->x <= prog.n, 0 <= cur->y <= prog.m
 * @assigns: met à jour *prog, *cur et *prog_stack
 * @ensures: execute l'instruction correspondant au caractère courant et
 *  avance le curseur d'un pas dans la direction courante
 */
void one_step(matrix* prog, cursor* cur, stack* prog_stack);

#endif
