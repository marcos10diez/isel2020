#include "alarma.h"
#include "led.h"
#include "kb.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "time_isel.h"


void keyboard(){
	int ch;
	scanf("%d",&ch);
	if (ch == 1){
		botonAlarma=1;
	}
	if (ch == 2){
		botonLed=1;
	}
}
