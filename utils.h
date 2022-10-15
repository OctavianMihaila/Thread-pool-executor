/* Mihăilă Octavian-Iulian */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
#include "queue.h"

#define null_terminator 1
#define add_tasks 1
#define get_task 2
#define get_thread 3
#define print_waiting 4
#define print_running 5
#define print_finished 6
#define run 7
#define finish 8
#define offset_add_task 9
#define offset_get_task 8
#define offset_run 3
#define offset_get_thread 10

typedef struct Thread {
    int id;
} Thread;

typedef struct {
    unsigned short id;
    struct Thread *thread;
    unsigned char priority;
    int time;
    int original_time;
} Task;

typedef struct list {
    unsigned short id;
    struct list *next;
} list;

// Argument for the switch that performs the parsing.
int encode(char *request);

// Initiates the thread_pool.
stack Init(FILE *input, int *quant, int *NrThreads); 

/* The add_tasks_waiting_q function performs the 
orderly insertion of tasks in waiting_q. */
void add_tasks_waiting_q(FILE *out_file, queue *waiting_q,
                        queue *start_waiting_q, int nr_tasks,
                        int exec_time, unsigned char priority,
                        int *k, list **id_list);

/* The function looks for a task by id in the 3 queues and provides 
the data about it according to the description in the statement. */
void get_task_from_queue(FILE *out_file, queue *waiting_q,
                        queue *start_waiting_q, queue *running_q,
                        queue *start_running_q, queue *finished_q,
                        queue *start_finished_q, int id);

/* The function searches for a thread in the thread_pool and 
through the queue of running tasks and returns its status. */
void get_thread_status(FILE *out_file, stack *thread_pool, queue *running_q,
                        queue *start_running_q, int id);

// The function prints the waiting queue.
void print_waiting_q(FILE *out_file, queue *waiting_q, queue *start_waiting_q);

// The function prints the running queue.
void print_running_q(FILE *out_file, queue *running_q, queue *start_running_q);

// The function prints the finished queue.
void print_finished_q(FILE *out_file, queue *finished_q,
                    queue *start_finished_q);

/* The function performs the parallel running
of the tasks over a period of time */
void run_process(queue *start_waiting_q, queue *running_q,
                queue *start_running_q, queue *finished_q,
                queue *start_finished_q, stack *thread_pool,
                int time, int quant, int *clock, list **id_list);

/* The function completes the process by
executing all the tasks in the waiting queue. */
void finish_process(FILE *out_file, queue *start_waiting_q,
                    queue *running_q, queue *start_running_q,
                    queue *finished_q, queue *start_finished_q,
                    stack *thread_pool, int quant, int clock);

// The function performs free on a queue.
void free_q(queue *start_q);

// The function performs free on a stack of threads.
void free_threads(stack *thread_pool, queue *start_running_q);

// The function performs free on a list.
void free_list(list **l);