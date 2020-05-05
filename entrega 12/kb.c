#include "fsm.h"
#include "task.h"
#include "alarma.h"
#include "led.h"
#include "kb.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

void* f_kb (void* arg)
{
	struct timeval next_activation;
	struct timeval now, timeout;
  int ch;

	gettimeofday (&next_activation, NULL);
	while (1) {
		struct timeval *period = task_get_period (pthread_self());
		timeval_add (&next_activation, &next_activation, period);
		gettimeofday (&now, NULL);
		timeval_sub (&timeout, &next_activation, &now);
		select (0, NULL, NULL, NULL, &timeout) ;
    // Stores the pressed key in ch
    scanf("%d",&ch);
    // Terminates the loop
    // when escape is pressed
    if (ch == 1){
      pthread_mutex_lock (&m_botonAlarma);
    	botonAlarma=1;
    	pthread_mutex_unlock (&m_botonAlarma);
    }
    if (ch == 2){
      pthread_mutex_lock (&m_botonLed);
      botonLed=1;
      pthread_mutex_unlock (&m_botonLed);

    }

	}
}
