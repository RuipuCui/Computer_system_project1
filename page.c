#include "page.h"
#include <stdlib.h>


//enque in head and pop tail

page_table_t* new_page_table(){
    page_table_t* table = malloc(sizeof(page_table_t));
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table->pages[i] = 0;
    }
    
    table->unused = TABLE_SIZE;
    return table;
}

void free_table(page_table_t* table){
    free(table);
}



//get the memory needed for a process
int count_page(process_t* p){
    if (p->memory % PAGE_SIZE > 0) {
        return p->memory/PAGE_SIZE +1;
    } 
    return p->memory/PAGE_SIZE;
}

//void, pass eviction to and alter
//return a 
void page_allocated(process_t* p, page_table_t* table, LRU_queue_t* queue, eviction_t* ev){
    if (!p -> allocated)
    {
        //printf("allocate");
        page_allocate(p,table,queue,ev);
    }
    //do move lru
    move_LRU_node(queue,p);
    return;
}



void page_deallocate(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev){  //remove process and free memory
    //printf("StartDeallocate\n");
    if(p->page_count){    
        //printf("1111");
    }
    


    int i = p->page_count;
    //printf("1");
    if(p->page_count > 0){
        //printf("page = 0");
        for (int j = 0; j < i; j++)
            {
                table->pages[(p->pages[j])] = 0;    //switch a block of memory to 0 automatically merge
            }
    }
    //printf("2");
    evict_process(ev,p);
    //printf("3");
    p->allocated = 0;
    table->unused += p->page_count;
    free(p->pages);
    //printf("4");
    //if process is kicked by lru, already not in queue. so only finish will cause the lru to remove
    dequeue_LRU(queue,p);
    //printf("finishDeallocate");
}




void page_allocate(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev){
    p->page_count = count_page(p);
    //printf("while");
    while (table->unused < p->page_count)
        {
            process_t* temp = LRU(queue);
            page_deallocate(temp,table,queue,ev);
        }
    p->allocated= 1;
    p->allocated_count = 0;
    p->pages = malloc(sizeof(int) * p->page_count);
    table->unused -= p->page_count;
    //int flag = 0;
    int i = 0;
    //printf("%d %d",p->allocated_count,p->page_count);
    while (p->allocated_count < p->page_count){
        //printf("table:%d %d alloc%d  %d\n",i,table->pages[i],p->allocated_count,p->page_count);
        if (table->pages[i] == 0){
            table->pages[i] = 1;
            p->pages[p->allocated_count] = i;
            p->allocated_count++;
            
          //  p->allocated_count ++;
        }
        i++;
    }
    
}

void print_page(process_t* p){
    fflush(stdout);
    printf("mem-frames=[");
    int i;
    for(i = 0; i < p->page_count; i++){
        if(i == p->page_count - 1){
            printf("%d", p->pages[i]);
        }else{
            printf("%d,", p->pages[i]);
        }
    }
    printf("]\n");
}

double page_memory_usage(page_table_t* table){
    double usage = TABLE_SIZE - table->unused;
    double usage_percentage = usage * 100 / TABLE_SIZE;
    return usage_percentage;
}


