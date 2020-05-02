#ifndef LED_H
#define LED_H

#include "fsm.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

extern int botonLed;
extern pthread_mutex_t m_botonLed;
fsm_t* fsm_new_alarma ();
void* f_alarma (void* arg);
#endif
