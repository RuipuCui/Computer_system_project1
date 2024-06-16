#ifndef _PROCESS_H_
#define _PROCESS_H_


process_t** readfile(FILE* f, int* n);
void free_array(process_t** processes, int num);
void scheduling_infinite(process_t** processes, int num, int quantum);
void scheduling_first_fit(process_t** processes, int num, int quantum);
void scheduling_page(process_t** processes, int num, int quantum);
void scheduling_virtual(process_t** processes, int num, int quantum);
double roundUpTwoDecimals(double number);
int roundUp(double number);

#endif