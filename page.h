#ifndef _PAGE_H_

#define _PAGE_H_

#include "queue.h"
#include "lru.h"
#include "eviction.h"

#define TABLE_SIZE 512
#define PAGE_SIZE 4


typedef struct page_table_t
{
    int pages[TABLE_SIZE];
    int unused;
}page_table_t;



page_table_t* new_page_table();
void free_table(page_table_t* table);
int count_page(process_t* p);
void page_allocated(process_t* p, page_table_t* table, LRU_queue_t* queue,eviction_t* ev);
void page_deallocate(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev);
void page_allocate(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev);
void print_page(process_t* p);
double page_memory_usage(page_table_t* table);


#endif