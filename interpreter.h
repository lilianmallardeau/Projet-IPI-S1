#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "stack.h"
#include "matrix.h"
#include "cursor.h"

void interpreter(matrix prog);

void one_step(matrix* prog, cursor* cur, stack* prog_stack);

#endif
