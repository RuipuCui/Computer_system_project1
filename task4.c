#include "task4.h"

void if_allocated4(process_t* p, page_table_t* table, LRU_queue_t* queue,eviction_t* ev){
    //movelru
    //static process_t* p2;
    if (!p->allocated)
    {
        allocate4(p,table,queue,ev);
    }
    move_LRU_node(queue,p);
    return ;
}

void allocate4(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev){
    p->page_count = count_page(p);
    //<minimum 4 or < page_count when pagecount <= 3
    while (table->unused < MINIMUM || (table->unused < p->page_count && p->page_count < MINIMUM))
    {
        //LRU_node_t* temp= LRU(queue);
        evict(queue->tail->p,table,ev,queue);
    }

    //perform allocation
    int i =0;

    if (table->unused >= p->page_count)
    {
        /* code */
        p->pages = malloc(sizeof(int)* p->page_count);

    }
    else{
        p->pages = malloc(sizeof(int)* table->unused);

    }
    
    while (table->unused > 0 && p->allocated_count < p->page_count && i < TABLE_SIZE)
    {
        //printf("i:%d\n",i);
        if(!table->pages[i]){
            table->pages[i]= 1;
            p->pages[p->allocated_count ++] = i;
            table->unused--;
        }
        i++;
    }
    //p->pages = realloc(p->pages,sizeof(int)* p->allocated_count);
    
    p->allocated = 1;
    //table->unused -= p->allocated_count;
    return;
}

//maybe can use page.c

void evict(process_t* p, page_table_t* table, eviction_t* ev,LRU_queue_t* queue){
    int to_be_evict = p->pages[0];
    p->allocated_count--;
    //for a large p 
    if(p->page_count >= MINIMUM ){
        if (p->allocated_count < MINIMUM)
        {
            p->allocated = 0;
        }
    }
    //is small p
    else{
        p->allocated = 0;   //not allocated when not all
    }
    if (!p->allocated_count)
    {
        free(p->pages);
        dequeue_LRU(queue,p);
    }
    else{
        remove_page(p,queue, table);
    }
    ev->evictions[ev->count++] = to_be_evict;
    table->pages[to_be_evict] = 0;
    table->unused++;
}

void remove_page(process_t* p, LRU_queue_t * queue, page_table_t* table){
    
    
    for(int i = 0; i < p->allocated_count;i++){
        p->pages[i] = p->pages[i+1];
    }


    queue->tail->page = p->pages[0];

}





void print_page_virtual(process_t* p){
    fflush(stdout);
    printf("mem-frames=[");
    int i;
    for(i = 0; i < p->allocated_count; i++){
        if(i == p->allocated_count - 1){
            printf("%d", p->pages[i]);
        }else{
            printf("%d,", p->pages[i]);
        }
    }
    printf("]\n");
}


void page_deallocate_virtual(process_t* p, page_table_t* table,LRU_queue_t* queue,eviction_t* ev){  //remove process and free memory



    int i = p->allocated_count;

    if(p->page_count > 0){

        for (int j = 0; j < i; j++)
            {
                table->pages[(p->pages[j])] = 0;    //switch a block of memory to 0 automatically merge
            }
    }

    evict_process_virtual(ev,p);

    p->allocated = 0;
    table->unused += p->allocated_count;
    free(p->pages);

    //if process is kicked by lru, already not in queue. so only finish will cause the lru to remove
    dequeue_LRU(queue,p);

}

void evict_process_virtual(eviction_t* ev, process_t* p){
    for (int i = 0; i < p->allocated_count; i++)
    {
        ev->evictions[i+ev->count] = p->pages[i];
    }
    ev->count += p->allocated_count;
}