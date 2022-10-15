/* Mihăilă Octavian-Iulian */
/* StackFunctions.c -- stack elements are stored using a generic list */

#include "stack.h"

stack push(stack *s, void *info) {
    stack new_elem = (stack)calloc(1, sizeof(stack_element));
    new_elem->prev = *s;

    new_elem->data = info;

    return new_elem;
}

stack pop(stack *s) {
    stack elem = *s;
    *s = (*s)->prev;

    return elem;
}