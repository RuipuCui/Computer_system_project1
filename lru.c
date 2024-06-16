#include "lru.h"
#include <stdlib.h>
#include <stdio.h>
LRU_queue_t* new_LRU_queue(){
    LRU_queue_t* temp = malloc(sizeof(LRU_queue_t));
    //assert(temp);
    temp->head = NULL;
    temp->tail = NULL;
    temp->length = 0;
    return temp;
}

void free_LRU_queue(LRU_queue_t* queue) {
    LRU_node_t* current = queue->head;
    while (current != NULL) {
        LRU_node_t* next = current->next;
        free(current); // Free the memory of the current node
        current = next; // Move to the next node
    }
    //queue->head = NULL;
    //queue->tail = NULL;
    free(queue);
}

void dequeue_LRU(LRU_queue_t* queue, process_t* p) {
    LRU_node_t* node = traverse(queue, p);

    if (node == NULL) {
        return;
    }

    if (node == queue->head) {
        queue->head = node->next;
        if (queue->head) {
            queue->head->prev = NULL;
        }
    } else {
        node->prev->next = node->next;
    }

    if (node == queue->tail) {
        queue->tail = node->prev; 
        if (queue->tail) {
            queue->tail->next = NULL;
        }
    } else {
        node->next->prev = node->prev;
    }

    node->next = NULL;
    node->prev = NULL;
    free(node);

    queue->length--;
}

void move_LRU_node(LRU_queue_t* queue, process_t* p) {
   
    LRU_node_t* node = traverse(queue, p);
   
    if (!node) {
        node = new_LRU_node(p);
      
        if (queue->head == NULL) {
            queue->head = node;
            queue->tail = node;
        } else {
            //new node and is head
            node->next = queue->head;
            queue->head->prev = node;
            queue->head = node;
        }
        queue->length++;
        //not head
    } else if (node != queue->head) {
        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        }
        if (node == queue->tail) {
            queue->tail = node->prev;
        }
        
        node->next = queue->head;
        queue->head->prev = node;
        node->prev = NULL;
        queue->head = node;
    }
  
}


LRU_node_t* new_LRU_node(process_t* p){
    LRU_node_t* node = (LRU_node_t*)malloc(sizeof(LRU_node_t));
    //assert(node);
    node->page = p->pages[0];
    node->p = p;
    node->prev = NULL;
    node->next = NULL;
    return node;
}


LRU_node_t* traverse(LRU_queue_t* queue, process_t* process) {
    LRU_node_t* current = queue->head;
  
    
    while (current != NULL) {
       
     
        
        if (current->p == process) {
            return current; // Process found, return the node
        }
       // printf("after IF\n");
        current = current->next;
    }
    //maybe can return a new node
    return NULL; // Process not found, return NULL
}

//function to pop tail
//add print poppings here
//function to get the tail of the queue, dequeue elsewhere
process_t* LRU(LRU_queue_t* queue){

   

    LRU_node_t* temp_node = queue->tail;

    process_t* p = queue->tail->p;

    if(queue->tail->prev){  
        queue->tail->prev->next = NULL;
    }
    else{
        queue->head = NULL;
        queue->tail = NULL;
    }
    free(temp_node);

    return p;
}


LRU_node_t* LRU_virtual(LRU_queue_t* queue){

   

    LRU_node_t* temp_node = queue->tail;

    //process_t* p = queue->tail->p;

    if(queue->tail->prev){  
        queue->tail->prev->next = NULL;
    }
    else{
        queue->head = NULL;
        queue->tail = NULL;
    }
    //free(temp_node);

    return temp_node;
}