/* Mihăilă Octavian-Iulian */

#include "utils.h"

int encode(char *request) {
    char *token = NULL;
    token = strtok (request, " ");

    if (strcmp(token, "add_tasks") == 0) {
        free(request);
        return add_tasks;
    }
    else if (strcmp(token, "get_task") == 0) {
        free(request);
        return get_task;
    }
    else if (strcmp(token, "get_thread") == 0) {
        free(request);
        return get_thread;
    }
    else if ((strcmp(token, "print") == 0)) {
        token = strtok(NULL, " ");
        if ((strcmp(token, "waiting") == 0)) {
            free(request);
            return print_waiting;
        }

        if ((strcmp(token, "running") == 0)) {
            free(request);
            return print_running;
        }

        if ((strcmp(token, "finished") == 0)) {
            free(request);
            return print_finished;
        }
    }
    else if ((strcmp(token, "run") == 0)) {
        free(request);
        return run;
    }
    else if ((strcmp(token, "finish") == 0)) {
        free(request);
        return finish;
    }

    free(request);
    printf("Invalid request\n");
    return 0;
}

stack Init(FILE *input, int *quant, int *NrThreads) {
    int core, quant_aux, garbage, threads_aux;
    
    fscanf(input, "%d", &quant_aux);
    fscanf(input, "%d", &core);
    fscanf(input, "%d", &garbage);

    *quant = quant_aux;
    *NrThreads = 2 * core;
    threads_aux = *NrThreads;

    stack thread_pool = NULL;
    while (threads_aux) {
        Thread *thread = (Thread *)calloc(1, sizeof(Thread));
        thread->id = threads_aux - 1;
        thread_pool = push(&thread_pool, thread);
        threads_aux--;
    }

    return thread_pool;
}

/* The function checks if the task meets all the conditions
necessary to occupy the first position in waiting_q.
elem is the first element extracted from waiting_q. */
int check_first_in_queue_conditions(Task *task, queue elem) {
    if (task->priority > ((Task *)elem->data)->priority) return 1;

    if (task->priority == ((Task *)elem->data)->priority && 
        task->time < ((Task *)elem->data)->time) return 1;

    if ((task->priority == ((Task *)elem->data)->priority && 
        task->time == ((Task *)elem->data)->time) &&
        task->id < ((Task *)elem->data)->id) return 1;
    
    return 0;
}

/* FThe function checks if the task fulfills all the necessary 
conditions to occupy the last position in waiting_q */
int check_last_in_queue_conditions(Task *task, queue *waiting_q) {
    if (*waiting_q &&
        (((Task *)(*waiting_q)->data)->priority > task->priority)) 
        return 1;

    if (*waiting_q && 
        (((Task *)(*waiting_q)->data)->priority == task->priority &&
        ((Task *)(*waiting_q)->data)->time < task->time))
        return 1;

    if (*waiting_q && 
        ((((Task *)(*waiting_q)->data)->priority == task->priority &&
        ((Task *)(*waiting_q)->data)->time == task->time) &&
        (((Task *)(*waiting_q)->data)->id < task->id)))
        return 1;

    return 0;
}

unsigned short asign_id(list **id_list) {
    list *aux = *id_list;
    *id_list = (*id_list)->next;
    int id = aux->id;
    free(aux);

    // Returns the first id from the list of freed ids.
    return id;
}

void add_tasks_waiting_q(FILE *out_file, queue *waiting_q,
                        queue *start_waiting_q, int nr_tasks,
                        int exec_time, unsigned char priority,
                        int *k, list **id_list) {
    while (nr_tasks) {
        // The tasks are created.
        int assigned_from_k = 0;
        Task *task = (Task *)calloc(1, sizeof(Task));
        if (*id_list == NULL) {
            task->id = *k;
            assigned_from_k = 1;
        }
        else {
            task->id = asign_id(id_list);
        }
        task->priority = priority;
        task->time = exec_time;
        task->original_time = exec_time;

        /* The task is returning to the waiting queue.
        (insert ordered according to the 3 criteria)*/
                    
        if (*waiting_q == NULL) { // void queue case.
            *waiting_q = InsertQ(*waiting_q, start_waiting_q, task);
        } 
        else if (check_last_in_queue_conditions(task, waiting_q)) {
            /* the case for which the task to be inserted has the
            lowest priority or equal to the last one in the queue.*/
            *waiting_q = InsertQ(*waiting_q, start_waiting_q, task);
        } 
        else {
            queue aux_q = NULL, start_aux_q = NULL;
            queue elem = ExtractQ(start_waiting_q);

            if (check_first_in_queue_conditions(task, elem)) {
                /* the case for which the task to be inserted has a priority
                greater than or equal to that of the first one in the queue. */
                aux_q = InsertQ(aux_q, &start_aux_q, task);
                aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                free(elem);
            }
            else {
                aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                while (*start_waiting_q) {
                    queue elem = ExtractQ(start_waiting_q);
                                    
                    if (task->priority > ((Task *)elem->data)->priority) {
                        aux_q = InsertQ(aux_q, &start_aux_q, task);
                        aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                        free(elem);
                        break;
                    }
                    // Equal priorities case.
                    if (task->priority == ((Task *)elem->data)->priority) {
                        if (task->time < ((Task *)elem->data)->time) {
                            aux_q = InsertQ(aux_q, &start_aux_q, task);
                            aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                            free(elem);
                            break;  
                        }
                        // Equal execution time case.
                        else if (task->time == ((Task *)elem->data)->time) {
                            // Sorting by ID.
                            if (task->id < ((Task *)elem->data)->id) {
                            aux_q = InsertQ(aux_q, &start_aux_q, task);
                            aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                            break; 
                            } 
                            else {
                                aux_q = InsertQ(aux_q, &start_aux_q,
                                                elem->data);
                            }
                        } 
                        else {
                            aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                        }
                    }
                    else {
                        aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                    }
                    free(elem);
                }
                free(elem);
            }
            // Remaining elements in the queue.
            while (*start_waiting_q) {
                queue elem = ExtractQ(start_waiting_q);
                aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
                free(elem);
            }

            /* Finally, we have the queue updated in aux_q
            and we restore the connections with waiting_q. */
            *start_waiting_q = start_aux_q;
            *waiting_q = aux_q;
        }
        fprintf(out_file, "Task created successfully : ID %d.\n", task->id);
        nr_tasks--;
        if (assigned_from_k) *k += 1;
    }
}

/* The function searches for and returns the element
in the queue with the id sent as a parameter.
In the case of the finished queue, it returns
the first element found with that id. */
queue find_task_in_queue(FILE *out_file, queue *end_q,
                        queue *start_q, unsigned short id) {
    queue aux_q = NULL, start_aux_q = NULL, wanted_elem = NULL;
    int found = 0;

    while (*start_q) {
        queue elem = ExtractQ(start_q);
        aux_q = InsertQ(aux_q, &start_aux_q, elem->data);

        if ((((Task *)elem->data)->id == id) && found == 0) {
            wanted_elem = InsertQ(wanted_elem, &wanted_elem, elem->data);

            found = 1;
        }
        free(elem);
    }
    *start_q = start_aux_q;
    *end_q = aux_q;

    if (found) return wanted_elem;
    else return NULL;
}

void get_task_from_queue(FILE *out_file, queue *waiting_q,
                        queue *start_waiting_q, queue *running_q,
                        queue *start_running_q, queue *finished_q,
                        queue *start_finished_q, int id) {
    queue elem = NULL;

    elem = find_task_in_queue(out_file, waiting_q, start_waiting_q, id);
    if (elem) {
        fprintf(out_file, "Task %d is waiting (remaining_time = %d).\n",
            ((Task *)elem->data)->id, ((Task *)elem->data)->time);
        free(elem);
        return;
    }

    elem = find_task_in_queue(out_file, running_q, start_running_q, id);
    if (elem) {
        fprintf(out_file, "Task %d is running (remaining_time = %d).\n",
            ((Task *)elem->data)->id, ((Task *)elem->data)->time);
        free(elem);
        return;
    }

    elem = find_task_in_queue(out_file, finished_q, start_finished_q, id);
    if (elem) {
        fprintf(out_file, "Task %d is finished (executed_time = %d).\n",
            ((Task *)elem->data)->id, ((Task *)elem->data)->original_time);
        free(elem);
        return;
    }

    fprintf(out_file, "Task %d not found.\n", id);
    free(elem);
}

void get_thread_status(FILE *out_file, stack *thread_pool, queue *running_q,
                        queue *start_running_q, int id) {
    stack stack_elem = NULL, aux_s = NULL;
    int found = 0;

    // Looking for the thread in the thread_pool
    while (*thread_pool) {
        stack_elem = pop(thread_pool);
        if (((Thread *)stack_elem->data)->id == id) {
            fprintf(out_file, "Thread %d is idle.\n", id);
            *thread_pool = push(thread_pool, stack_elem->data);
            found = 1;
            free(stack_elem);
            break;
        }
        if (!found) {
            aux_s = push(&aux_s, stack_elem->data);
            free(stack_elem);
        }
    }

    // thread_pool stack is restored.
    while (aux_s) {
        stack_elem = pop(&aux_s);
        *thread_pool = push(thread_pool, stack_elem->data);
        free(stack_elem);
    }

    if (found) return;

    queue queue_elem = NULL, aux_q = NULL, start_aux_q = NULL;

    // If the thread was not found in thread_pool, it is searched in running_q.
    while (*start_running_q) {
        queue_elem = ExtractQ(start_running_q);
        if ((((Task *)queue_elem->data)->thread)->id == id) {
            int task_id = ((Task *)queue_elem->data)->id;
            int remaining_time = ((Task *)queue_elem->data)->time;
            fprintf(out_file,
                    "Thread %d is running task %d (remaining_time = %d).\n",
                    id, task_id, remaining_time);
        }
        aux_q = InsertQ(aux_q, &start_aux_q, queue_elem->data);
        free(queue_elem);
    }

    *start_running_q = start_aux_q;
    *running_q = aux_q;
}

void print_waiting_q(FILE *out_file, queue *waiting_q, queue *start_waiting_q) {
    queue aux_q = NULL, start_aux_q = NULL, elem = NULL;
    int ok = 0;

    fprintf(out_file, "====== Waiting queue =======\n[");
    while (*start_waiting_q) {
        if (ok) fprintf(out_file, ",\n");
        elem = ExtractQ(start_waiting_q);
        unsigned short id = ((Task *)elem->data)->id;
        unsigned char priority = ((Task *)elem->data)->priority;
        int remaining_time = ((Task *)elem->data)->time;
        fprintf(out_file, "(%d: priority = %d, remaining_time = %d)",
                id, priority, remaining_time);
        aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
        ok = 1;
        free(elem);
    }
    fprintf(out_file, "]\n");

    *start_waiting_q = start_aux_q;
    *waiting_q = aux_q;
}

void print_running_q(FILE *out_file, queue *running_q, queue *start_running_q) {
    queue aux_q = NULL, start_aux_q = NULL, elem = NULL;
    int ok = 0;

    fprintf(out_file, "====== Running in parallel =======\n[");
    while (*start_running_q) {
        if (ok) fprintf(out_file, ",\n");
        elem = ExtractQ(start_running_q);
        unsigned short id = ((Task *)elem->data)->id;
        unsigned char priority = ((Task *)elem->data)->priority;
        int remaining_time = ((Task *)elem->data)->time;
        int running_thread_id = ((Thread *)(((Task *)elem->data)->thread))->id;
        fprintf(out_file,
                "(%d: priority = %d, remaining_time = %d, running_thread = %d)",
               id, priority, remaining_time, running_thread_id);
        aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
        ok = 1;
        free(elem);
    }
    fprintf(out_file, "]\n");

    *start_running_q = start_aux_q;
    *running_q = aux_q;
}

void print_finished_q(FILE *out_file, queue *finished_q,
                    queue *start_finished_q) {
    queue aux_q = NULL, start_aux_q = NULL, elem = NULL;
    int ok = 0;

    fprintf(out_file, "====== Finished queue =======\n[");
    while (*start_finished_q) {
        if (ok) fprintf(out_file, ",\n");
        elem = ExtractQ(start_finished_q);
        unsigned short id = ((Task *)elem->data)->id;
        unsigned char priority = ((Task *)elem->data)->priority;
        int original_time = ((Task *)elem->data)->original_time;
        fprintf(out_file, "(%d: priority = %d, executed_time = %d)",
                id, priority, original_time);
        aux_q = InsertQ(aux_q, &start_aux_q, elem->data);
        ok = 1;
        free(elem);
    }
    fprintf(out_file, "]\n");

    *start_finished_q = start_aux_q;
    *finished_q = aux_q;
}

void recycle_id(list **id_list, unsigned short id) {
    list *id_cell = (list *)calloc(1, sizeof(list));
    id_cell->id = id;
    id_cell->next = NULL;

    // Void list case.
    if (*id_list == NULL) {
        *id_list = id_cell;
        return;
    }

    // Insertion if front of an element from the list.
    list *p = *id_list, *ant = NULL;
    while (p != NULL) {
        if (p->id > id) {
            if (ant) ant->next = id_cell;
            else *id_list = id_cell;
            id_cell->next = p;
            return;
        }
        ant = p;
        p = p->next;
    }

    // Insertion at the end of the list.
    ant->next = id_cell;
    return;
}

void run_process(queue *start_waiting_q, queue *running_q,
                queue *start_running_q, queue *finished_q,
                queue *start_finished_q, stack *thread_pool,
                int time, int quant, int *clock, list **id_list) {

    stack stack_elem = NULL;
    queue queue_elem = NULL; 
    int exec_time;

    while (time > 0) {
        if (time < quant) exec_time = time;
        else exec_time = quant;
        int processed = 0;
        int maxRemaining = 0;
        
        queue aux_q = NULL, start_aux_q = NULL;

        /* The remaining running time for each task in the running queue is
        updated and they are moved to the processed tasks in the finished queue */
        while (*start_running_q) {
            queue_elem = ExtractQ(start_running_q);
            ((Task *)(queue_elem->data))->time -= exec_time;
            
            if (((Task *)(queue_elem->data))->time <= 0) {
                // Orderly insertion in the list of IDs.
                recycle_id(id_list, ((Task *)(queue_elem->data))->id);

                *finished_q = InsertQ(*finished_q, start_finished_q,
                                    queue_elem->data);
                Thread *thread = ((Task *)(queue_elem->data))->thread;
                *thread_pool = push(thread_pool, thread);

                int remainingTime = exec_time + 
                                    ((Task *)(queue_elem->data))->time;
                if (remainingTime < quant && remainingTime > maxRemaining) {
                    maxRemaining = remainingTime;
                }
            }
            else {
                maxRemaining = quant; // blocking remainingTime
                aux_q = InsertQ(aux_q, &start_aux_q, queue_elem->data);
            }
            free(queue_elem);
            processed = 1;
        }
 
         // Running queue is updated.
        *start_running_q = start_aux_q;
        *running_q = aux_q;

        /* One thread is assigned to the tasks in waiting_q,
        within the limit of the number in the thread_pool. */
        while (*thread_pool && *start_waiting_q) {
            stack_elem = pop(&(*thread_pool));
            queue_elem = ExtractQ(start_waiting_q);
            ((Task *)queue_elem->data)->thread = (Thread *)(stack_elem->data);
            *running_q = InsertQ(*running_q, start_running_q, queue_elem->data);
            free(stack_elem);
            free(queue_elem);
        }

        if (processed) {
            time -= quant;
            if (maxRemaining != 0 && maxRemaining < quant) {
                *clock += maxRemaining;
            }
            else {
                *clock += exec_time; // Running time is counted.
            } 
        }
        
        if (!(*start_running_q) && !(*start_waiting_q)) break;
    }
}

void finish_process(FILE *out_file, queue *start_waiting_q,
                    queue *running_q, queue *start_running_q,
                    queue *finished_q, queue *start_finished_q,
                    stack *thread_pool, int quant, int clock) {
    int total_time_executed = 0;
    stack stack_elem = NULL;
    queue queue_elem = NULL;

    while (*start_waiting_q || *start_running_q) {
        queue aux_q = NULL, start_aux_q = NULL;

        /* The remaining running time for each task in the running queue is
        updated and they are moved to the processed tasks in the finished queue */
        while (*start_running_q) {
            queue_elem = ExtractQ(start_running_q);
            if (((Task *)(queue_elem->data))->time > total_time_executed)
                total_time_executed = ((Task *)(queue_elem->data))->time;
            ((Task *)(queue_elem->data))->time -= quant;
            
            if (((Task *)(queue_elem->data))->time <= 0) {
                *finished_q = InsertQ(*finished_q, start_finished_q,
                                    queue_elem->data);
                Thread *thread = ((Task *)(queue_elem->data))->thread;
                *thread_pool = push(thread_pool, thread);
            }
            else {
                aux_q = InsertQ(aux_q, &start_aux_q, queue_elem->data);
            }
            free(queue_elem);
        }

         // Running queue is updated.
        *start_running_q = start_aux_q;
        *running_q = aux_q;

        /* One thread is assigned to the tasks in waiting_q, 
        within the limit of the number in the thread_pool. */
        while (*thread_pool && *start_waiting_q) {
            stack_elem = pop(&(*thread_pool));
            queue_elem = ExtractQ(start_waiting_q);
            ((Task *)queue_elem->data)->thread = (Thread *)(stack_elem->data);
            *running_q = InsertQ(*running_q, start_running_q, queue_elem->data);
            free(stack_elem);
            free(queue_elem);
        }
    }
    fprintf(out_file, "Total time: %d", (total_time_executed + clock));
}

void free_q(queue *start_q) {
    queue elem = NULL;

    while (*start_q) {
        elem = ExtractQ(start_q);
        free((Task *)elem->data);
        free(elem);
    }
}

void free_threads(stack *thread_pool, queue *start_running_q) {
    stack stack_elem = NULL;

    while (*thread_pool) {
        stack_elem = pop(thread_pool);
        free((Thread *)stack_elem->data);
        free(stack_elem);
    }

    queue queue_elem = NULL, aux_q = NULL, start_aux_q = NULL;
    int k = 0;

    while (*start_running_q) {
        k++;
        queue_elem = ExtractQ(start_running_q);
        free(((Task *)queue_elem->data)->thread);
        aux_q = InsertQ(aux_q, &start_aux_q, queue_elem->data);
        free(queue_elem);
    }
    *start_running_q = start_aux_q;
}

void free_list(list **l) {
    list *aux = NULL;
    while (*l) {
        aux = *l;
        *l = (*l)->next;
        free(aux);
    }
}