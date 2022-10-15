/* Mihăilă Octavian-Iulian */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct Queue_cell {
    void *data;
    struct Queue_cell *next;
} queue_element, *queue;

queue InsertQ(queue q, queue *start_q, void *info);
queue ExtractQ(queue *q);