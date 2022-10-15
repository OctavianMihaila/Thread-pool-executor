/* Mihăilă Octavian-Iulian */
/* QueueFunctions.c -- queue elements are stored using a generic list */

#include "queue.h"

queue InsertQ(queue q, queue *start_q, void *info) {
    queue new_elem = (queue)calloc(1, sizeof(queue_element));
    new_elem->data = info;

    if (q == NULL){
        *start_q = new_elem;
    } else {
        q->next = new_elem;
    }

    return new_elem;
}

queue ExtractQ(queue *q) {
    queue elem = *q;
    if (*q) *q = (*q)->next;

    return elem;
}
