#ifndef _STACK_H
#define _STACK_H

typedef int elem;
typedef struct item item;
typedef item* stack;
struct item {
  elem head;
  stack tail;
};

stack create_stack();

int is_empty(stack);

void push(elem, stack*);

elem pop(stack*);

void insert(elem, stack*, int);

elem get(stack, int);

int get_index(elem, stack);

int len(stack);

void print_list(stack);

#endif
