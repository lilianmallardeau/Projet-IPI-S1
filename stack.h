#ifndef _STACK_H
#define _STACK_H

typedef int stack_elem;
typedef struct item item;
typedef item* stack;
struct item {
  stack_elem head;
  stack tail;
};

stack create_stack();

int is_empty(stack);

void push(stack_elem, stack*);

stack_elem pop(stack*);

void insert(stack_elem, stack*, int);

stack_elem get(stack, int);

int get_index(stack_elem, stack);

int len(stack);

void print_list(stack);

#endif
