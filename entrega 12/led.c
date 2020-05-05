#include "fsm.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "led.h"
#include "time_isel.h"


//Variables de entrada
int botonLed = 0;

static void timer_start (int ms) ;


//Variables de salida
static int led = 0;

//Variables
static struct timeval timer_endtime;
static int T = 3000;

//Se definen los estados
enum fsm_state{
	ON,
	OFF,
};


//FuncionesComprobacion
static int botonPulsado (fsm_t* this) {
	int ret;
	ret=botonLed;
	return ret;
}
static int finTiempo (fsm_t* this){
	struct timeval now;
	gettimeofday(&now, NULL);
	return timeval_less(&timer_endtime, &now);
}

//FuncionesTransicion
static void apagarLed(fsm_t* this){
	led = 0;
	printf("LED apagado \n");

}

static void encenderLed (fsm_t* this){
	botonLed=0;
	led = 1;
	printf("LED encendido \n");
	timer_start(T);
}

static void actualizarTimer(fsm_t* this){
	botonLed=0;
	printf("%d \n", led);
	timer_start(T);
}

static void timer_start (int ms) {
  struct timeval inc = { ms/1000, (ms % 1000) * 1000 };
  gettimeofday (&timer_endtime, NULL);
  timeval_add (&timer_endtime, &timer_endtime, &inc);
}



//Se definen la matriz de transiciones
//EstadoInicial/funciónComprobación/EstadoFinal/FuncionTransicion
fsm_t*
fsm_new_led ()
{
	static fsm_trans_t tt[] = {
		{ ON, finTiempo,OFF , apagarLed },
		{ ON,botonPulsado,ON , actualizarTimer  },
		{ OFF,botonPulsado,ON , encenderLed},
		{ -1, NULL, -1, NULL },
	};
	gettimeofday(&timer_endtime, NULL);
	return fsm_new (tt);
}
