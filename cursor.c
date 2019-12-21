#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cursor.h"

/******** Static functions prototypes ********/
static void _move_forward(cursor* cur);
/****** Static functions prototypes END ******/


/* @requieres: *cur is a valid cursor
 * @assigns: changes cur->dir
 * @ensures: rotates the cursor's direction from 45°
 */
void rotate_cw(cursor* cur) {
  cur->dir = (cur->dir + 1) % 8;
}

/* @requieres: *cur is a valid cursor
 * @assigns: changes cur->dir
 * @ensures: rotates the cursor's direction from -45°
 */
void rotate_ccw(cursor* cur) {
  cur->dir = (cur->dir - 1) % 8;
}

/* @requieres: *cur is a valid cursor
 * @assigns: changes cur->dir
 * @ensures: sets a random direction
 */
void set_random_dir(cursor* cur) {
  srand(time(NULL));
  cur->dir = rand() % 8;
}

/* @requieres: *cur is a valid cursor
 * @assigns: changes cursor coordinates cur->x and cur->y
 * @ensures: moves n times the cursor forward according to the current direction
 */
void move_forward(cursor* cur, int n, int x_max, int y_max) {
  for (int i = 0; i < n; i++) {
    _move_forward(cur);
    cur->x = cur->x % x_max;
    cur->y = cur->y % y_max;
  }
}

/* This function is not meant to be called manually. It is called
 * by 'move_forward' to move the cursor of one step. It moves the cursor
 * of one step according to the current direction and doesn't check if the
 * new coordinates are out of range.
 * @requieres: *cur is a valid cursor
 * @assigns: changes cur->x and/or cur->y according to the current direction
 * @ensures: changes cur->x and/or cur->y according to the current direction
 */
static void _move_forward(cursor* cur) {
  switch (cur->dir) {
    case right:
      cur->x = (cur->x + 1);
      break;
    case down_right:
      cur->x = (cur->x + 1);
      cur->y = (cur->y + 1);
      break;
    case down:
      cur->y = (cur->y + 1);
      break;
    case down_left:
      cur->x = (cur->x - 1);
      cur->y = (cur->y + 1);
      break;
    case left:
      cur->x = (cur->x - 1);
      break;
    case up_left:
      cur->x = (cur->x - 1);
      cur->y = (cur->y - 1);
      break;
    case up:
      cur->y = (cur->y - 1);
      break;
    case up_right:
      cur->x = (cur->x + 1);
      cur->y = (cur->y - 1);
      break;
  }
}
