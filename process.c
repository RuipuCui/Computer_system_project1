#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "queue.h"
#include "process.h"
#include "memory.h"
#include "page.h"
#include "task4.h"

process_t** readfile(FILE* f, int* n){
    int arrive_time;
    char name[MAX_LENGTH + 1];
    int brust_time;
    int memory;

    process_t** processes = (process_t**)malloc(sizeof(process_t*) * 10);
    assert(processes);
    int size = 10;
    int number = 0;

    while(fscanf(f, "%d %s %d %d", &arrive_time, name, &brust_time, &memory) == 4){
        process_t* process = (process_t*)malloc(sizeof(process_t));
        assert(process);
        process->arrive_time = arrive_time;
        strcpy(process->name, name);
        process->brust_time = brust_time;
        process->service_time = brust_time;
        process->memory = memory;
        process->allocated = 0;
        process->allocated_count = 0;
        if(number == size){
            size = size * 2;
            processes = realloc(processes, sizeof(process_t*) * size);
            assert(processes);
        }
        processes[number] = process;
        number ++;
    }

    *n = number;
    fclose(f);
    return processes;
}

void free_array(process_t** processes, int num){
    int i;
    for(i = 0; i < num; i++){
        free(processes[i]);
    }
    free(processes);
}

void scheduling_infinite(process_t** processes, int num, int quantum){
    queue_t* queue = queueCreate();
    int count;
    for(count = 0; count < num; count++){
        if(processes[count]->arrive_time == 0){
            processes[count]->state = READY;
            enqueue(queue, processes[count]);
        }else{
            break;
        }
    }

    int total_turnaround = 0;
    double total_overhead = 0;
    double max_overhead = 0;
    int time = 0;
    int blocks = 0;
    while(queue->n != 0 || count < num){

        if(queue->n > 1 && queue->head->process->state == RUNNING){
            queue->head->process->state = READY;
            move_to_tail(queue);
        }

        if(queue->head != NULL && queue->head->process->state == FINISHED){
            total_turnaround += queue->head->process->finish_time - queue->head->process->arrive_time;
            double overhead = (double)(queue->head->process->finish_time - queue->head->process->arrive_time) / queue->head->process->service_time;
            if(overhead > max_overhead){
                max_overhead = overhead;
            }
            total_overhead += overhead;
            dequeue(queue);
        }

        int i;
        for(i = 0; i < quantum; i++){
            if(queue->head == NULL){
                int j;
                for(j = 0; j < quantum; j++){
                    time++;
                    for(; count < num; count++){
                        if(processes[count]->arrive_time == time){
                            processes[count]->state = READY;
                            enqueue(queue, processes[count]);
                        }else{
                            break;
                        }
                    }
                }
                
                break;             
            }else  if(queue->head != NULL){
                if(queue->head->process->state == READY){
                    queue->head->process->state = RUNNING;
                    printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", time, queue->head->process->name, queue->head->process->brust_time);
                }
                queue->head->process->brust_time -= 1;
                time++;
            }
            for(; count < num; count++){
                if(processes[count]->arrive_time == time){
                    processes[count]->state = READY;
                    enqueue(queue, processes[count]);
                }else{
                    break;
                }
            }
        }

        if(queue->head != NULL && queue->head->process->brust_time <= 0){
            queue->head->process->state = FINISHED;
            queue->head->process->finish_time = blocks * quantum + quantum;
            printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", blocks * quantum + quantum, queue->head->process->name, queue->n - 1);
        }

        blocks++;
    }

    printf("Turnaround time %d\n", roundUp((double)total_turnaround / num));
    printf("Time overhead %.2lf %.2lf\n", roundUpTwoDecimals(max_overhead), roundUpTwoDecimals(total_overhead / num));
    printf("Makespan %d\n", time - quantum);

    queueFree(queue);
    return;
}


void scheduling_first_fit(process_t** processes, int num, int quantum){
    queue_t* queue = queueCreate();
    int count;
    for(count = 0; count < num; count++){
        if(processes[count]->arrive_time == 0){
            processes[count]->state = READY;
            enqueue(queue, processes[count]);
        }else{
            break;
        }
    }

    int total_turnaround = 0;
    double total_overhead = 0;
    double max_overhead = 0;
    int time = 0;
    int blocks = 0;
    int RAM[2048] = {0};
    while(queue->n != 0 || count < num){

        if(queue->n > 1 && queue->head->process->state == RUNNING){
            queue->head->process->state = READY;
            move_to_tail(queue);
        }

        if(queue->head != NULL && queue->head->process->state == FINISHED){
            total_turnaround += queue->head->process->finish_time - queue->head->process->arrive_time;
            double overhead = (double)(queue->head->process->finish_time - queue->head->process->arrive_time) / queue->head->process->service_time;
            if(overhead > max_overhead){
                max_overhead = overhead;
            }
            total_overhead += overhead;
            deallocate(queue->head->process,RAM);
            dequeue(queue);
        }

        if(queue->head != NULL && queue->head->process->state == READY){
            while(!if_allocated(queue->head->process, RAM)){
                move_to_tail(queue);
            }
        }

        int i;
        for(i = 0; i < quantum; i++){
            if(queue->head == NULL){
                int j;
                for(j = 0; j < quantum; j++){
                    time++;
                    for(; count < num; count++){
                        if(processes[count]->arrive_time == time){
                            processes[count]->state = READY;
                            enqueue(queue, processes[count]);
                        }else{
                            break;
                        }
                    }
                }
                
                break;               
            }else  if(queue->head != NULL){
                if(queue->head->process->state == READY){
                    queue->head->process->state = RUNNING;
                    int usage = (int)round(memory_usage(RAM));
                    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, queue->head->process->name, queue->head->process->brust_time, usage, queue->head->process->memory_address);
                }
                queue->head->process->brust_time -= 1;
                time++;
            }
            for(; count < num; count++){
                if(processes[count]->arrive_time == time){
                    processes[count]->state = READY;
                    enqueue(queue, processes[count]);
                }else{
                    break;
                }
            }
        }

        if(queue->head != NULL && queue->head->process->brust_time <= 0){
            queue->head->process->state = FINISHED;
            queue->head->process->finish_time = blocks * quantum + quantum;
            printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", blocks * quantum + quantum, queue->head->process->name, queue->n - 1);
        }

        blocks++;
    }

    printf("Turnaround time %d\n", roundUp((double)total_turnaround / num));
    printf("Time overhead %.2lf %.2lf\n", roundUpTwoDecimals(max_overhead), roundUpTwoDecimals(total_overhead / num));
    printf("Makespan %d\n", time - quantum);

    queueFree(queue);
    return;
}

double roundUpTwoDecimals(double number){
    return round(number * 100) / 100;
}

int roundUp(double number){
    return (int)ceil(number);
}

void scheduling_page(process_t** processes, int num, int quantum){
    queue_t* queue = queueCreate();
    int count;
    for(count = 0; count < num; count++){
        if(processes[count]->arrive_time == 0){
            processes[count]->state = READY;
            enqueue(queue, processes[count]);
        }else{
            break;
        }
    }

    int total_turnaround = 0;
    double total_overhead = 0;
    double max_overhead = 0;
    int time = 0;
    int blocks = 0;

    page_table_t* page_table = new_page_table();
    LRU_queue_t* lru_queue = new_LRU_queue();
    eviction_t* evictions = new_eviction();


    while(queue->n != 0 || count < num){

        if(queue->n > 1 && queue->head->process->state == RUNNING){
            queue->head->process->state = READY;
            move_to_tail(queue);
        }

        if(queue->head != NULL && queue->head->process->state == FINISHED){

            total_turnaround += queue->head->process->finish_time - queue->head->process->arrive_time;
            double overhead = (double)(queue->head->process->finish_time - queue->head->process->arrive_time) / queue->head->process->service_time;
            if(overhead > max_overhead){
                max_overhead = overhead;
            }
            total_overhead += overhead;
            //printf("startdeallo");
            /*
            page_deallocate(queue->head->process, page_table, lru_queue, evictions);
            if(evictions->count != 0){
                printf("%d,", time);
                print_ev(evictions);
                clear_eviction(evictions);
            }*/
            dequeue(queue);
        }

        if(queue->head != NULL && queue->head->process->state == READY){
            //printf("startallo");
            page_allocated(queue->head->process, page_table, lru_queue, evictions);
            if(evictions->count != 0){
                printf("%d,", time);
                print_ev(evictions);
                clear_eviction(evictions);
            }
        }

        int i;
        for(i = 0; i < quantum; i++){
            if(queue->head == NULL){
                int j;
                for(j = 0; j < quantum; j++){
                    time++;
                    for(; count < num; count++){
                        if(processes[count]->arrive_time == time){
                            processes[count]->state = READY;
                            enqueue(queue, processes[count]);
                        }else{
                            break;
                        }
                    }
                }
                
                break;             
            }else  if(queue->head != NULL){
                if(queue->head->process->state == READY){
                    queue->head->process->state = RUNNING;
                    //printf("  %lf  ", page_memory_usage(page_table));
                    int usage = (int)roundUp(page_memory_usage(page_table));
                    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,", time, queue->head->process->name, queue->head->process->brust_time, usage);
                    print_page(queue->head->process);
                    fflush(stdout);
                }
                queue->head->process->brust_time -= 1;
                time++;
            }
            for(; count < num; count++){
                if(processes[count]->arrive_time == time){
                    processes[count]->state = READY;
                    enqueue(queue, processes[count]);
                }else{
                    break;
                }
            }
        }

        if(queue->head != NULL && queue->head->process->brust_time <= 0){
            queue->head->process->state = FINISHED;
            queue->head->process->finish_time = blocks * quantum + quantum;
            page_deallocate(queue->head->process, page_table, lru_queue, evictions);
            if(evictions->count != 0){
                printf("%d,", time);
                print_ev(evictions);
                clear_eviction(evictions);
            }

            printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", blocks * quantum + quantum, queue->head->process->name, queue->n - 1);
            fflush(stdout);
        }

        blocks++;
    }

    printf("Turnaround time %d\n", roundUp((double)total_turnaround / num));
    printf("Time overhead %.2lf %.2lf\n", roundUpTwoDecimals(max_overhead), roundUpTwoDecimals(total_overhead / num));
    printf("Makespan %d\n", time - quantum);
    fflush(stdout);

    free_eviction(evictions);
    free_LRU_queue(lru_queue);
    free_table(page_table);
    queueFree(queue);
    return;
}


void scheduling_virtual(process_t** processes, int num, int quantum){
    queue_t* queue = queueCreate();
    int count;
    for(count = 0; count < num; count++){
        if(processes[count]->arrive_time == 0){
            processes[count]->state = READY;
            enqueue(queue, processes[count]);
        }else{
            break;
        }
    }

    int total_turnaround = 0;
    double total_overhead = 0;
    double max_overhead = 0;
    int time = 0;
    int blocks = 0;

    page_table_t* page_table = new_page_table();
    LRU_queue_t* lru_queue = new_LRU_queue();
    eviction_t* evictions = new_eviction();


    while(queue->n != 0 || count < num){


        if(queue->n > 1 && queue->head->process->state == RUNNING){
            queue->head->process->state = READY;
            move_to_tail(queue);
        }

        if(queue->head != NULL && queue->head->process->state == FINISHED){

            total_turnaround += queue->head->process->finish_time - queue->head->process->arrive_time;
            double overhead = (double)(queue->head->process->finish_time - queue->head->process->arrive_time) / queue->head->process->service_time;
            if(overhead > max_overhead){
                max_overhead = overhead;
            }
            total_overhead += overhead;
            //printf("startdeallo");
            /*
            page_deallocate(queue->head->process, page_table, lru_queue, evictions);
            if(evictions->count != 0){
                printf("%d,", time);
                print_ev(evictions);
                clear_eviction(evictions);
            }*/
            dequeue(queue);
        }

        if(queue->head != NULL && queue->head->process->state == READY){
            //printf("startallo");
           
            if_allocated4(queue->head->process, page_table, lru_queue, evictions);
            if(evictions->count != 0){
                printf("%d,", time);
                print_ev(evictions);
                clear_eviction(evictions);
            }

        }

        int i;
        for(i = 0; i < quantum; i++){
            if(queue->head == NULL){
                int j;
                for(j = 0; j < quantum; j++){
                    time++;
                    for(; count < num; count++){
                        if(processes[count]->arrive_time == time){
                            processes[count]->state = READY;
                            enqueue(queue, processes[count]);
                        }else{
                            break;
                        }
                    }
                }
                
                break;             
            }else  if(queue->head != NULL){
                if(queue->head->process->state == READY){
                    queue->head->process->state = RUNNING;
                    //printf("  %lf  ", page_memory_usage(page_table));
                    int usage = (int)roundUp(page_memory_usage(page_table));
                    
                    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,", time, queue->head->process->name, queue->head->process->brust_time, usage);
                    print_page_virtual(queue->head->process);
                    fflush(stdout);
                }
                queue->head->process->brust_time -= 1;
                time++;
            }
            for(; count < num; count++){
                if(processes[count]->arrive_time == time){
                    processes[count]->state = READY;
                    enqueue(queue, processes[count]);
                }else{
                    break;
                }
            }
        }

        if(queue->head != NULL && queue->head->process->brust_time <= 0){
            queue->head->process->state = FINISHED;
            queue->head->process->finish_time = blocks * quantum + quantum;
            page_deallocate_virtual(queue->head->process, page_table, lru_queue, evictions);
            if(evictions->count != 0){
                printf("%d,", time);
                print_ev(evictions);
                clear_eviction(evictions);
            }

            printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", blocks * quantum + quantum, queue->head->process->name, queue->n - 1);
            fflush(stdout);
        }

        blocks++;
    }

    printf("Turnaround time %d\n", roundUp((double)total_turnaround / num));
    printf("Time overhead %.2lf %.2lf\n", roundUpTwoDecimals(max_overhead), roundUpTwoDecimals(total_overhead / num));
    printf("Makespan %d\n", time - quantum);
    fflush(stdout);

    free_eviction(evictions);
    free_LRU_queue(lru_queue);
    free_table(page_table);
    queueFree(queue);
    return;
}