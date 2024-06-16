#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "queue.h"
#include "process.h"

int main(int argc, char* argv[]) {

    char* file;
    char* memory_management;
    int quantum;
    int i;
    for(i = 1; i < argc; i++){
        if(strcmp(argv[i], "-f") == 0){
            file = argv[i+1];
        }else if(strcmp(argv[i], "-q") == 0){
            quantum = atoi(argv[i+1]);
        }else if(strcmp(argv[i], "-m") == 0){
            memory_management =  argv[i+1];
        }
    }

    FILE* f = fopen(file, "r");
    process_t** processes;
    int num;
    processes = readfile(f, &num);
    if(strcmp(memory_management, "infinite") == 0){
        scheduling_infinite(processes, num, quantum);
    }else if(strcmp(memory_management, "first-fit") == 0){
        scheduling_first_fit(processes, num, quantum);
    }else if(strcmp(memory_management, "paged") == 0){
        scheduling_page(processes, num, quantum);
    }
    else if(strcmp(memory_management, "virtual") ==0){
        scheduling_virtual(processes, num, quantum);
    }

    free_array(processes, num);
    return 0;
}
