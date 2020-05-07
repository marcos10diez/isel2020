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
#include "reactor.h"

fsm_t* fsm_led;
fsm_t* fsm_alarma;


int key_pressed (void)
{
  struct timeval timeout = { 0, 0 };
  fd_set rd_fdset;
  FD_ZERO(&rd_fdset);
  FD_SET(0, &rd_fdset);
  return select(1, &rd_fdset, NULL, NULL, &timeout) > 0;
}


static void alarma_func(struct event_handler_t* this){
  static const struct timeval period={ 0, 100};
  fsm_fire(fsm_alarma);
  timeval_add(&this->next_activation,&this->next_activation,&period);
}

static void led_func(struct event_handler_t* this){
  static const struct timeval period={ 0, 100};
  fsm_fire(fsm_led);
  timeval_add(&this->next_activation,&this->next_activation,&period);
}

static void kb_func(struct event_handler_t* this){
  static const struct timeval period={ 0, 100};
  if(key_pressed()){
    keyboard();
  }
  timeval_add(&this->next_activation,&this->next_activation,&period);
}


int main ()
{
  EventHandler eh_alarma, eh_led, eh_kb;
  reactor_init ();

  fsm_alarma = fsm_new_alarma();
  fsm_led = fsm_new_led();

  event_handler_init (&eh_alarma, 3, alarma_func);
  reactor_add_handler (&eh_alarma);

  event_handler_init (&eh_kb, 2, kb_func);
  reactor_add_handler (&eh_kb);

  event_handler_init (&eh_led, 1, led_func);
  reactor_add_handler (&eh_led);

  while (1) {
    reactor_handle_events ();
  }
}
