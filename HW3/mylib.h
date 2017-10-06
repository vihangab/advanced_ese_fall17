#ifndef MYLIB_H_
#define MYLIB_H_ 

/*
Description - Library file to declare global clock_t variables to store start and end time clock tick 
*/

#include <time.h>
clock_t start_time, end_time1, cpu_time1, end_time2, cpu_time2;
clock_t start_time_pthread;
clock_t end_time_pthread;
clock_t cpu_time_pthread;
#endif 