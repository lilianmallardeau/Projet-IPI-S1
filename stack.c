#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/* @requires: nothing
 * @assigns: nothing
 * @ensures: returns an empty stack (ie. a null pointer)
 */
stack create_stack() {
  return NULL;
}

/* @requires: s is a valid stack
 * @assigns: nothing
 * @ensures: returns 1 if s is an empty stack, 0 otherwise
 */
int is_stack_empty(stack s) {
  return s == NULL;
}

/* @requires: s is a valid stack
 * @assigns: creates a new stack and copies each element of s in the new stack, in the same order
 * @ensures: returns a copy of the stack s
 */
stack copy_stack(stack s) {
  stack new_stack = create_stack();
  for (int i = 0; i < stack_len(s); i++) {
    push_last(get_stack_elem(s, i), &new_stack);
  }
  return new_stack;
}

/* @requires: *s is a valid stack
 * @assigns: modifies s
 * @ensures: adds e to the begginning of s
 */
void push(stack_elem e, stack* s) {
  stack new = (stack) malloc(sizeof(item));
  new->head = e;
  new->tail = *s;
  *s = new;
}

/* @requires: *s is a valid stack
 * @assigns: modifies s
 * @ensures: removes the first element of the stack and returns it
 */
stack_elem pop(stack* s) {
  if (is_empty(*s))
    return 0;
  stack_elem e = (*s)->head;
  stack tmp = *s;
  *s = (*s)->tail;
  free(tmp);
  return e;
}

/* @requires: *s is a valid stack
 * @assigns: modifies the stack
 * @ensures: adds e at the end of *s
 */
void push_last(stack_elem e, stack* s) {
  insert_stack_elem(e, stack_len(*s), s);
}

/* @requires: *s is a valid stack
 * @assigns: modifies the stack
 * @ensures: removes the last element of the stack and returns it
 */
stack_elem pop_last(stack* s) {
  stack_elem e = get_last_stack_elem(*s);
  remove_stack_elem(s, stack_len(*s) - 1);
  return e;
}

/* @requires: *s is a valid stack, 0 <= index <= stack_len(*s)
 * @assigns: modifies the stack
 * @ensures: inserts the stack_element e at the index-th place in the stack
 */
void insert_stack_elem(stack_elem e, int index, stack* s) {
  stack cur = *s;
  for (int i = 0; i < index - 1; i++) {
    cur = cur->tail;
  }
  item* new = (item*) malloc(sizeof(item));
  new->head = e;
  if (index == 0) {
    new->tail = cur;
    *s = new;
  } else {
    new->tail = cur->tail;
    cur->tail = new;
  }
}

/* @requires: s is a valid stack, 0 <= index < stack_len(s)
 * @assigns: nothing
 * @ensures: returns the index-th stack_element of s
 */
stack_elem get_stack_elem(stack s, int index) {
  for (int i = 0; i < index; i++) {
    s = s->tail;
  }
  return s->head;
}

/* @requires: s is a valid stack
 * @assigns: nothing
 * @ensures: returns the last element of the stack
 */
stack_elem get_last_stack_elem(stack s) {
  return get_stack_elem(s, stack_len(s) - 1);
}

/* @requires: *s is a valid stack, 0 <= index < stack_len(*s)
 * @assigns: changes the index-th element of the stack to e
 * @ensures: changes the index-th element of the stack to e
 */
void set_stack_elem(stack *s, stack_elem e, int index) {
  stack l = *s;
  for (int i = 0; i < index; i++) {
    l = l->tail;
  }
  l->head = e;
}

/* @requires: s is a valid stack
 * @assigns: nothing
 * @ensures: returns the index of the first occurence of e in s, or -1 if e is not in s
 */
int get_stack_elem_index(stack_elem e, stack s) {
  if (is_stack_empty(s))
    return -1;
  int index = 0;
  while (s->head != e) {
    if (index > stack_len(s))
      return -1;
    index++;
    s = s->tail;
  }
  return index;
}

/* @requires: *s is a valid stack, 0 <= index < stack_len(*s)
 * @assigns: modifies the stack
 * @ensures: removes the index-th element of the stack and frees the memory previously allocated to it
 */
void remove_stack_elem(stack *s, int index) {
  if (index == 0) {
    pop(s);
  } else {
    stack cur = *s;
    for (int i = 0; i < index - 1; i++) {
      cur = cur->tail;
    }
    stack tmp = cur->tail->tail;
    free(cur->tail);
    cur->tail = tmp;
  }
}

/* @requires: s is a valid stack and doesn't loop
 * @assigns: nothing
 * @ensures: returns the length of s (ie. the number of elements it contains)
 */
int stack_len(stack s) {
  int i = 0;
  while (s != NULL) {
    s = s->tail;
    i++;
  }
  return i;
}

/* @requires: s is a valid stack and doesn't loop
 * @assigns: nothing
 * @ensures: prints the stack s in the format [stack_elem1] -> [stack_elem2] -> ... -> []
 */
void print_stack(stack s) {
  while (s != NULL) {
    printf("[%d] -> ", s->head);
    s = s->tail;
  }
  printf("[]\n");
}
