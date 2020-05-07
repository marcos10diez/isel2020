#include "fsm.h"
#include "alarma.h"
#include "led.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include "time_isel.h"
#include "kb.h"
#include <sys/select.h>


#define EJERCICIO12
// #define MEDICION_TIEMPOS

int key_pressed (void)
{
  struct timeval timeout = { 0, 0 };
  fd_set rd_fdset;
  FD_ZERO(&rd_fdset);
  FD_SET(0, &rd_fdset);
  return select(1, &rd_fdset, NULL, NULL, &timeout) > 0;
}

#ifdef EJERCICIO12
int main ()
{
	struct timeval period = { 0, 100 };
	struct timeval next;
	fsm_t* fsm_led = fsm_new_led();
	fsm_t* fsm_alarma = fsm_new_alarma();


	gettimeofday(&next, NULL);
	while (1) {
		if(key_pressed()){
			keyboard();
		}
		fsm_fire (fsm_led);
		fsm_fire (fsm_alarma);
		timeval_add (&next, &next, &period);
		delay_until (&next);
	}
}
#endif

#ifdef MEDICION_TIEMPOS
int main ()
{
    struct timeval tv[4];
	fsm_t* fsm_led = fsm_new_led();
	fsm_t* fsm_alarma = fsm_new_alarma();
  gettimeofday(&tv[0], NULL);
  fsm_fire(fsm_led);
  gettimeofday(&tv[1], NULL);
  fsm_fire(fsm_alarma);
  gettimeofday(&tv[2], NULL);
  if(key_pressed()){
    keyboard();
  }
  gettimeofday(&tv[3], NULL);
  time_t curtime;
  char buffer[30];
  for(int i = 0; i<4;i++){
    curtime=tv[i].tv_sec;
    strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
    printf("%s%ld\n",buffer,tv[i].tv_usec);
  }
}
#endif
