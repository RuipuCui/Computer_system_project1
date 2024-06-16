#ifndef _MEMORY_H_

#define _MEMORY_H_

#include "queue.h"

#define MEMORY_SIZE 2048



//public
int if_allocated(process_t* p, int* memory);
void deallocate(process_t* p, int* memory);

//private
int allocate(process_t* p, int* memory);
int find_space(process_t* p, int* memory);
double memory_usage(int* memory);

#endif