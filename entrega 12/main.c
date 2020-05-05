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


int key_pressed (void)
{
  struct timeval timeout = { 0, 0 };
  fd_set rd_fdset;
  FD_ZERO(&rd_fdset);
  FD_SET(0, &rd_fdset);
  return select(1, &rd_fdset, NULL, NULL, &timeout) > 0;
}

int main ()
{
	struct timeval period = { 0, 150000 };
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
