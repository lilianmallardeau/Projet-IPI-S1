#ifndef _STACK_H
#define _STACK_H

typedef int stack_elem;
typedef struct item item;

/* A stack is defined as a pointer to an item */
typedef item* stack;

/* An item in the stack is defined as a struct containing the value of the current item (head)
 *  and a pointer to the next item in the stack (tail)
 * An item is considered to be the last of the stack if its tail is the null pointer.
 */
struct item {
  stack_elem head;
  stack tail;
};


/* @requires: nothing
 * @assigns: nothing
 * @ensures: returns an empty stack (ie. a null pointer)
 */
stack create_stack();

/* @requires: s is a valid stack
 * @assigns: nothing
 * @ensures: returns 1 if s is an empty stack, 0 otherwise
 */
int is_stack_empty(stack s);

/* @requires: s is a valid stack
 * @assigns: creates a new stack and copies each element of s in the new stack, in the same order
 * @ensures: returns a copy of the stack s
 */
stack copy_stack(stack s);

/* @requires: *s is a valid stack
 * @assigns: modifies s
 * @ensures: adds e to the begginning of s
 */
void push(stack_elem e, stack* s);

/* @requires: *s is a valid stack
 * @assigns: modifies s
 * @ensures: removes the first element of the stack and returns it
 */
stack_elem pop(stack* s);

/* @requires: *s is a valid stack
 * @assigns: modifies the stack
 * @ensures: adds e at the end of *s
 */
void push_last(stack_elem e, stack* s);

/* @requires: *s is a valid stack
 * @assigns: modifies the stack
 * @ensures: removes the last element of the stack and returns it
 */
stack_elem pop_last(stack* s);

/* @requires: *s is a valid stack, 0 <= index <= stack_len(*s)
 * @assigns: modifies the stack
 * @ensures: inserts the stack_element e at the index-th place in the stack
 */
void insert_stack_elem(stack_elem e, int index, stack* s);

/* @requires: s is a valid stack, 0 <= index < stack_len(s)
 * @assigns: nothing
 * @ensures: returns the index-th stack_element of s
 */
stack_elem get_stack_elem(stack s, int index);

/* @requires: s is a valid stack
 * @assigns: nothing
 * @ensures: returns the last element of the stack
 */
stack_elem get_last_stack_elem(stack s);

/* @requires: *s is a valid stack, 0 <= index < stack_len(*s)
 * @assigns: changes the index-th element of the stack to e
 * @ensures: changes the index-th element of the stack to e
 */
void set_stack_elem(stack *s, stack_elem e, int index);

/* @requires: s is a valid stack
 * @assigns: nothing
 * @ensures: returns the index of the first occurence of e in s, or -1 if e is not in s
 */
int get_stack_elem_index(stack_elem e, stack s);

/* @requires: *s is a valid stack, 0 <= index < stack_len(*s)
 * @assigns: modifies the stack
 * @ensures: removes the index-th element of the stack and frees the memory previously allocated to it
 */
void remove_stack_elem(stack *s, int index);

/* @requires: s is a valid stack and doesn't loop
 * @assigns: nothing
 * @ensures: returns the length of s (ie. the number of elements it contains)
 */
int stack_len(stack s);

/* @requires: s is a valid stack and doesn't loop
 * @assigns: nothing
 * @ensures: prints the stack s in the format [stack_elem1] -> [stack_elem2] -> ... -> []
 */
void print_stack(stack s);


#endif
