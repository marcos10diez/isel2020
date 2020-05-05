#ifndef ALARMA_H
#define ALARMA_H

#include "fsm.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

extern int botonAlarma;
extern pthread_mutex_t m_botonAlarma;
fsm_t* fsm_new_led ();
void* f_led (void* arg);
#endif
