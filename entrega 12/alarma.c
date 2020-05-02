#include "fsm.h"
#include "task.h"
#include "alarma.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

//Variables de entrada
int botonAlarma = 0;
pthread_mutex_t m_botonAlarma;

//No se muy bien cuando se llama a la función
//static void boton_Alarma(void){
//	botonAlarma = 1;
//}

static void timer_start (int ms);

//Variables de salida
static int alarma = 0;

//Variables
static struct timeval timer_endtime;
static int T = 1000;
static int d1 = 0;
static int d2 = 0;
static int d3 = 0;
static int pulsacionMax;

//10 means 0
const int c1 = 5;
const int c2 = 2;
const int c3 = 6;

//Se definen los estados
enum fsm_state{
	idle,
	st1,
	st2,
	st3,
};



//FuncionesComprobacion
static int botonPulsado (fsm_t* this) {
	pthread_mutex_lock (&m_botonAlarma);
	int ret=botonAlarma;
	pthread_mutex_unlock (&m_botonAlarma);
	return ret;
}
static int finTiempoOPulsaciones (fsm_t* this){
	struct timeval now;
	gettimeofday(&now, NULL);
	switch(this->current_state){
		case st1:
			if(d1 == 10){
				pulsacionMax = 1;
			}
			else{
				pulsacionMax = 0;
			}
			break;
		case st2:
			if(d2 == 10){
				pulsacionMax = 1;
			}
			else{
				pulsacionMax = 0;
			}
			break;
		case st3:
			if(d3 == 10){
				pulsacionMax = 1;
			}
			else{
				pulsacionMax = 0;
			}
			break;
//		case default:
//			break;
	}
	if((timeval_less(&timer_endtime, &now)==1)||(pulsacionMax==1)){
		return 1;
	}
	else{
		return 0;
	}
}




//FuncionesTransicion
static void incrementar(fsm_t* this){
	pthread_mutex_lock (&m_botonAlarma);
	botonAlarma=0;
	pthread_mutex_unlock (&m_botonAlarma);
	switch(this->current_state){
		case idle:
			d1++;
			break;
		case st1:
			d1++;
			break;
		case st2:
			d2++;
			break;
		case st3:
			d3++;
			break;
//		case default:
//			break;
	}
	timer_start(T);
}

static void comprobarCodigo(fsm_t* this){
	if((d1 == c1) && (d2 == c2) && (d3 == c3)){
		 if( alarma == 1){
		 	alarma =0;
		 }
		 else{
		 	alarma = 1;
		 }
	}
	d1 = 0;
	d2 = 0;
	d3 = 0;
}

static void timer_start (int ms) {
  struct timeval inc = { ms/1000, (ms % 1000) * 1000 };
  gettimeofday (&timer_endtime, NULL);
  timeval_add (&timer_endtime, &timer_endtime, &inc);
}


//Se definen la matriz de transiciones
//EstadoInicial/funciónComprobación/EstadoFinal/FuncionTransicion
fsm_t* fsm_new_alarma ()
{
	static fsm_trans_t tt[] = {
		{ idle,botonPulsado,st1,incrementar},
		{ st1,botonPulsado,st1,incrementar },
		{ st1,finTiempoOPulsaciones,st2,NULL },
		{ st2,botonPulsado,st2,incrementar },
		{ st2,finTiempoOPulsaciones,st3,NULL},
		{ st3,botonPulsado,st3,incrementar },
		{ st3,finTiempoOPulsaciones,idle,comprobarCodigo },
		{ -1, NULL, -1, NULL },
	};
	return fsm_new (tt);
}

void* f_alarma (void* arg)
{
	struct timeval next_activation;
	struct timeval now, timeout;
	fsm_t* fsm = fsm_new_alarma();

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
