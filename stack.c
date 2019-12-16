#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/* @requires: nothing
 * @assigns: nothing
 * @ensures: return an empty stack (a null pointer)
 */
stack create_stack() {
  return NULL;
}

/* @requires: l is a valid stack
 * @assigns: nothing
 * @ensures: return 1 if l is an empty stack, 0 otherwise
 */
int is_empty(stack l) {
  return l == NULL;
}

/* @requires: l is a valid stack
 * @assigns:
 * @ensures: add e to the beggining of l
 */
void push(elem e, stack* l) {
  stack new = (stack) malloc(sizeof(item));
  new->head = e;
  new->tail = *l;
  *l = new;
}

/* @requires: l is a valid stack
 * @assigns: modify the stack
 * @ensures: remove the first element of the stack and return it
 */
elem pop(stack* l) {
  elem e = (*l)->head;
  *l = (*l)->tail;
  return e;
}

/* @requires: l is a valid stack, index <= len(l)
 * @assigns: modify the stack
 * @ensures: insert the element e at the index-th place in the stack
 */
void insert(elem e, stack* l, int index) {
  stack cur = *l;
  for (int i = 0; i < index - 1; i++) {
    cur = cur->tail;
  }
  item* new = (item*) malloc(sizeof(item));
  new->head = e;
  if (index == 0) {
    new->tail = cur;
    *l = new;
  } else {
    new->tail = cur->tail;
    cur->tail = new;
  }
}

/* @requires: l is a valid stack, index <= len(l)
 * @assigns: nothing
 * @ensures: returns the index-th element of l
 */
elem get(stack l, int index) {
  for (int i = 0; i < index; i++) {
    l = l->tail;
  }
  return l->head;
}

/* @requires: l is a valid stack
 * @assigns: nothing
 * @ensures: returns the index of the first occurence of e, -1 if e is not in l
 */
int get_index(elem e, stack l) {
  int index = 0;
  while (l->head != e) {
    if (index > len(l))
      return -1;
    index++;
    l = l->tail;
  }
  return index;
}

/* @requires: l is a valid stack and doesn't loop
 * @assigns: nothing
 * @ensures: returns the length of l
 */
int len(stack l) {
  int i = 0;
  while (l != NULL) {
    l = l->tail;
    i++;
  }
  return i;
}

/* @requires: l is a valid stack and doesn't loop
 * @assigns: nothing
 * @ensures: prints the stack l in the format [elem1] -> [elem2] -> ... -> []
 */
void print_list(stack l) {
  while (l != NULL) {
    printf("[%d] -> ", l->head);
    l = l->tail;
  }
  printf("[]\n");
}
