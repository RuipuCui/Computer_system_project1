#ifndef _QUEUE_H_
#define _QUEUE_H_

#define RUNNING 1
#define READY 0
#define FINISHED -1

#define MAX_LENGTH 8

typedef struct process_t{
	char name[MAX_LENGTH + 1];
    int arrive_time;
    int brust_time;
    int memory;
    int state;
	int start_time;
	int finish_time;
	int service_time;

	//task2
	int allocated;
	int memory_address;

	//task3
	int* pages;
	int page_count;

	//task4
	int allocated_count;
} process_t;

typedef struct node node_t;
struct node {
	process_t* process;
	node_t *next;
};

struct list {
	node_t *head;
	node_t *tail;
	int n;
};
typedef struct list queue_t;

queue_t* queueCreate();
void queueFree(queue_t* queue);
void enqueue(queue_t* queue, process_t* process);
void dequeue(queue_t* queue);
void move_to_tail(queue_t* queue);


#endif