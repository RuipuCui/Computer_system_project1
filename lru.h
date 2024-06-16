#ifndef _LRU_H_
#define _LRU_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"



typedef struct LRU_node LRU_node_t;
typedef struct 
{
    LRU_node_t* head;
    LRU_node_t* tail;
    //using for debuging, remove later

    int length;
}LRU_queue_t;



struct LRU_node
{
    int page;
    process_t* p;
    LRU_node_t* prev;
    LRU_node_t* next;

};

void dequeue_LRU(LRU_queue_t* queue, process_t* p);
LRU_queue_t* new_LRU_queue();
void free_LRU_queue(LRU_queue_t* queue) ;
void move_LRU_node(LRU_queue_t* queue, process_t* p);
LRU_node_t* new_LRU_node(process_t* p);
LRU_node_t* traverse(LRU_queue_t* queue, process_t* process);
process_t* LRU(LRU_queue_t* queue);
LRU_node_t* LRU_virtual(LRU_queue_t* queue);
#endif