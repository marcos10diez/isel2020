#ifndef TASK_H
#define TASK_H

#include "time_isel.h"
#include <sys/time.h>

void delay_until (struct timeval* next_activation);
void timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b);
void timeval_add (struct timeval *res, struct timeval *a, struct timeval *b);
int timeval_less (struct timeval *a, struct timeval *b);
int timeval_get_ms (struct timeval *a);


#endif
