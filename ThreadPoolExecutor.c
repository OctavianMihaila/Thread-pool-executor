/* Mihăilă Octavian-Iulian */

#include "utils.h"

int main(int argc, char **argv) {
    char *request = NULL, *token = NULL, *aux_request = NULL;
    int quant, NrThreads, k = 1, clock = 0;

    FILE *in_file, *out_file;

    in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("no such file.");
        return 0;
    }

    out_file = fopen(argv[2], "w");
    if (out_file == NULL) {
        printf("no such file.");
        return 0;
    }

    stack thread_pool = Init(in_file, &quant, &NrThreads);
    queue waiting_q = NULL, running_q = NULL, finished_q = NULL;
    queue start_waiting_q = NULL, start_running_q = NULL;
    queue start_finished_q = NULL;
    list *id_list = NULL;
    
    request = calloc(100, sizeof(char));
    if (!request) {
        printf("allocation failed for request.");
        return 0;
    }

    while (fscanf(in_file, "%[^\n] ", request) != EOF) {
        aux_request = calloc(strlen(request) + null_terminator, sizeof(char));
        memcpy(aux_request, request, strlen(request) + null_terminator);
        if (!aux_request) {
            printf("allocation failed for aux_request.");
            return 0;
        }
        switch (encode(aux_request)) {
            case add_tasks:
                token = strtok(request + offset_add_task, " ");
                int nr_tasks = atoi(token);
                
                token = strtok(NULL, " ");
                int exec_time = atoi(token);

                token = strtok(NULL, " ");
                unsigned char priority = atoi(token);

                add_tasks_waiting_q(out_file, &waiting_q, &start_waiting_q,
                                    nr_tasks, exec_time, priority,
                                    &k, &id_list);

                break;
            
            case get_task:
                token = strtok(request + offset_get_task, " ");
                int task_id = atoi(token);
                get_task_from_queue(out_file, &waiting_q, &start_waiting_q,
                                    &running_q, &start_running_q, &finished_q,
                                    &start_finished_q, task_id);
                break;

            case get_thread:
                token = strtok(request + offset_get_thread, " ");
                int thread_id = atoi(token);
                get_thread_status(out_file, &thread_pool, &running_q,
                                &start_running_q, thread_id);
                break;

            case print_waiting:
                print_waiting_q(out_file, &waiting_q, &start_waiting_q);
                break;
            
            case print_running:
                print_running_q(out_file, &running_q, &start_running_q);
                break;
            
            case print_finished:
                print_finished_q(out_file, &finished_q, &start_finished_q);
                break;
            
            case run:
                token = strtok(request + offset_run, " ");
                int time = atoi(token);
                run_process(&start_waiting_q, &running_q, &start_running_q,
                            &finished_q, &start_finished_q, &thread_pool,
                            time, quant, &clock, &id_list);
                if (!start_waiting_q) waiting_q = NULL;
                fprintf(out_file, "Running tasks for %d ms...\n", time);
                break;

            case finish:
                finish_process(out_file, &start_waiting_q, &running_q,
                                &start_running_q, &finished_q,
                                &start_finished_q, &thread_pool, quant, clock);
                break;
            
            default:
                printf("Invalid request\n");
                break;
        }
    }
    free(request);
    free_q(&start_waiting_q);
    free_q(&start_finished_q);
    free_threads(&thread_pool, &start_running_q);
    free_q(&start_running_q);
    free_list(&id_list);
    fclose(in_file);
    fclose(out_file);

    return 0;
}