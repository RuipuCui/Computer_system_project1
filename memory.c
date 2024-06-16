#include "memory.h"

// int memory[2048];
//array of memory declare outside with int memory[2048]
//declare of array automatically malloc 2048 address.
//using int instead of bool cause waste of memory
//task2

//public
int if_allocated(process_t* p, int* memory){
    if (p->allocated)
    {
        return 1;   //allocated, return 1
    }
    else{
        return(allocate(p,memory));    //logic for not already allocated
    }
}

void deallocate(process_t* p, int* memory){  //remove process and free memory
    int i = p->memory_address;
    for (int j = 0; j < p->memory; j++)
    {
        memory[i+j] = 0;    //switch a block of memory to 0 automatically merge
    }
    
}

//private
int allocate(process_t* p, int* memory){
    return find_space(p,memory);
}

int find_space(process_t* p, int* memory){
    int i =0;
    while (i < MEMORY_SIZE)
    {
        int flag = 0;
        if (!memory[i])
        {
            int j = 0;
            //int flag = 1;
            while (j < p->memory && !memory[i+j])
            {
                j++;   
            }
            
            if (j == p->memory)
            {
                //allocate the process if found
                for (int k = 0; k < j;k++)
                {
                    memory[i+k] = 1;
                }
                p->memory_address = i;
                p->allocated = 1;

                return 1;
            }   
            i += j;
        }
        if (!flag){
            i++;
        }
        
    }
    return 0;
}

double memory_usage(int* memory){
    int i;
    double usage = 0;
    for(i = 0; i < MEMORY_SIZE; i++){
        if(memory[i] == 1){
            usage++;
        }
    }
    double usage_percentage = usage * 100 / MEMORY_SIZE;
    return usage_percentage;
}