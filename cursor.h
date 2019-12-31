#ifndef _CURSOR_H
#define _CURSOR_H

typedef enum direction direction;
enum direction {up = 0, up_right, right, down_right, down, down_left, left, up_left};

typedef struct cursor cursor;
struct cursor {
  int x, y;
  enum direction dir;
};

void rotate_cw(cursor* cur);

void rotate_ccw(cursor* cur);

void set_random_dir(cursor* cur);

void move_forward(cursor* cur, int n, int x_max, int y_max);

/* static void _move_forward(cursor* cur); */

#endif
