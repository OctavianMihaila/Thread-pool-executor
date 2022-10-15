/* Mihăilă Octavian-Iulian */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct Stack_cell {
    void *data;
    struct Stack_cell *prev;
} stack_element, *stack;

stack push(stack *s, void *info);
stack pop(stack *s);