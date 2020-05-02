#include "fsm.h"
#include "task.h"
#include <stdio.h>

int moneda = 10;
int boton = 1;

pthread_mutex_t m_dinero;
int dinero;

static void sumar (fsm_t* this) {
	pthread_mutex_lock (&m_dinero);
	dinero += moneda;
	printf ("dinero = %d\n", dinero);
	pthread_mutex_unlock (&m_dinero);
}

static int boton_pulsado (fsm_t* this) {
	int ret;

	pthread_mutex_lock (&m_dinero);
	ret = boton && (dinero >= 50);
	pthread_mutex_unlock (&m_dinero);
	return ret;
}

static void hacer_cafe (fsm_t* this) {
	printf ("cafe\n");
	pthread_mutex_lock (&m_dinero);
	dinero -= 50;
	pthread_mutex_unlock (&m_dinero);
}

int true (fsm_t* this) { return 1; }

fsm_t*
fsm_new_monedero ()
{
	static fsm_trans_t tt[] = {
		{ 0, true, 0, sumar },
		{ -1, NULL, -1, NULL },
	};
	return fsm_new (tt);
}

fsm_t*
fsm_new_mcafe ()
{
	static fsm_trans_t tt[] = {
		{ 0, boton_pulsado, 0, hacer_cafe },
		{ -1, NULL, -1, NULL },
	};
	return fsm_new (tt);
}

void* f_monedero (void* arg)
{
	struct timeval next_activation;
	struct timeval now, timeout;
	fsm_t* fsm = fsm_new_monedero();

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

void* f_mcafe (void* arg)
{
	struct timeval next_activation;
	struct timeval now, timeout;
	fsm_t* fsm = fsm_new_mcafe();

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

int main () 
{
	pthread_t monedero, mcafe;

	mutex_init (&m_dinero, 2);
	monedero = task_new ("monedero", f_monedero, 150, 150, 1, 1024);
	mcafe = task_new ("mcafe", f_mcafe, 150, 150, 2, 1024);
	pthread_join (monedero, NULL);
}

