#ifndef _EVICTION_H_
#define _EVICTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"


#define ALL 0
#define ONE 1
#define TABLE_SIZE 512

typedef struct
{
    int* evictions;
    int count;
} eviction_t;

//typedef struct eviction eviction_t;

eviction_t* new_eviction();
void add_eviction(eviction_t* ev, int page);
void evict_process(eviction_t* ev, process_t* p);
void free_eviction(eviction_t* ev);
void print_ev(eviction_t* ev);
void clear_eviction(eviction_t* ev);

#endif