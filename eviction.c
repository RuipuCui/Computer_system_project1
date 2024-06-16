#include "eviction.h"
#include <stdlib.h>
#include <stdio.h>


eviction_t* new_eviction(){
    eviction_t* ev = malloc (sizeof(eviction_t));
    ev->evictions = malloc(sizeof(int) * TABLE_SIZE);

    int i;
    for(i = 0; i < TABLE_SIZE; i++){
        ev->evictions[i] = 0;
    }
    ev->count = 0;
    return ev;
}


void add_eviction(eviction_t* ev, int page){
    ev->evictions[(ev->count)++] = page;
}

void evict_process(eviction_t* ev, process_t* p){
    for (int i = 0; i < p->page_count; i++)
    {
        ev->evictions[i+ev->count] = p->pages[i];
    }
    ev->count += p->page_count;
}


void free_eviction(eviction_t* ev){
    free(ev->evictions);
    free(ev);
}

void print_ev(eviction_t* ev){
    int i;
    printf("EVICTED,evicted-frames=[");
    fflush(stdout);
    for(i = 0; i < ev->count; i++){
        if(i == ev->count - 1){
            printf("%d", ev->evictions[i]);
        }
        else{
            printf("%d,", ev->evictions[i]);
        }
        fflush(stdout);
    }
    printf("]\n");
    
}

void clear_eviction(eviction_t* ev){
    int i;
    for(i = 0; i < TABLE_SIZE; i++){
        ev->evictions[i] = 0;
    }
    ev->count = 0;
}