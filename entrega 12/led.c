#include "fsm.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "led.h"

//Variables de entrada
int botonLed = 0;
pthread_mutex_t m_botonLed;

static void timer_start (int ms) ;


//Variables de salida
static int led = 0;

//Variables
static struct timeval timer_endtime;
static int T = 1000;

//Se definen los estados
enum fsm_state{
	ON,
	OFF,
};


//FuncionesComprobacion
static int botonPulsado (fsm_t* this) {
	int ret;
	pthread_mutex_lock (&m_botonLed);
	ret=botonLed;
	pthread_mutex_unlock (&m_botonLed);
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
}

static void encenderLed (fsm_t* this){
	pthread_mutex_lock (&m_botonLed);
	botonLed=0;
	pthread_mutex_unlock (&m_botonLed);
	led = 1;
	timer_start(T);
}

static void actualizarTimer(fsm_t* this){
	pthread_mutex_lock (&m_botonLed);
	botonLed=0;
	pthread_mutex_unlock (&m_botonLed);
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
		return fsm_new (tt);
}

void* f_led (void* arg)
{
	struct timeval next_activation;
	struct timeval now, timeout;
	fsm_t* fsm = fsm_new_led();

	gettimeofday (&next_activation, NULL);
	while (1) {
		struct timeval *period = task_get_period (pthread_self());
		timeval_add (&next_activation, &next_activation, period);
		gettimeofday (&now, NULL);
		timeval_sub (&timeout, &next_activation, &now);
		select (0, NULL, NULL, NULL, &timeout) ;

		fsm_fire (fsm);
	}
}
