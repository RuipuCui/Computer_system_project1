#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

queue_t* queueCreate() {
	queue_t* queue = malloc(sizeof(*queue));
	assert(queue);
	queue->head = queue->tail = NULL;
	queue->n = 0;
	return queue;
}

void queueFree(queue_t* queue){
	assert(queue != NULL);
	node_t *curr = queue->head;
	while (curr) {
		node_t *tmp = curr;
		curr = curr->next; 
		free(tmp);
	}
	free(queue);
}

void enqueue(queue_t* queue, process_t* process){
	assert(queue);
	node_t* new = malloc(sizeof(*new));
	assert(new);
	new->process = process;
	new->next = NULL;

	if(queue->head == NULL && queue->tail == NULL){
		queue->head = new;
		queue->tail = new;
	}else{
		queue->tail->next = new;
		queue->tail = new;
	}
	queue->n += 1;

}

void dequeue(queue_t* queue){

	node_t* curr_head = queue->head;
	queue->head = queue->head->next;
	if(queue->head == NULL){
		queue->tail = NULL;
	}

	queue->n -= 1;
	free(curr_head);
}

void move_to_tail(queue_t *queue) {
    if (queue->head->next == NULL) {
        return;
    }

    node_t *curr_head = queue->head;
    queue->head = curr_head->next;
    curr_head->next = NULL;
    queue->tail->next = curr_head;
    queue->tail = curr_head;
}
