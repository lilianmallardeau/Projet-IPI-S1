#ifndef _INTERPRETER_H
#define _INTERPRETER_H

typedef enum direction direction;
enum direction {up, down, left, right, up_left, up_right, down_left, down_right};

typedef struct cursor cursor;
struct cursor {
  int x, y;
  enum direction dir;
};

void interpreter(matrix prog);

#endif
