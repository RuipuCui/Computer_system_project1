#ifndef _TASK_4_

#define _TASK_4_

#include "page.h"
#include "eviction.h"

#define MINIMUM 4




void if_allocated4(process_t* p, page_table_t* table, LRU_queue_t* queue,eviction_t* ev);
void allocate4(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev);
void evict(process_t* p, page_table_t* table, eviction_t* ev,LRU_queue_t* queue);
void remove_page(process_t* p, LRU_queue_t * queue, page_table_t* table);
void print_page_virtual(process_t* p);
void page_deallocate_virtual(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev);
void evict_process_virtual(eviction_t* ev, process_t* p);

#endif