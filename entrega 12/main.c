#include "fsm.h"
#include "task.h"
#include "alarma.h"
#include "led.h"
#include "kb.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <curses.h>
int main ()
{
	pthread_t alarma, led, kb;

	mutex_init (&m_botonLed, 2);
  mutex_init (&m_botonAlarma, 2);

	alarma = task_new ("alarma", f_alarma, 150, 150, 2, 1024);
	led = task_new ("led", f_led, 150, 150, 2, 1024);
  kb = task_new ("led", f_kb, 150, 150, 1, 1024);

	pthread_join (alarma, NULL);
}
