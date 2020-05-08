
/**
 *	Archivo pml que modela y verifica los ejercicios 12, 13 y 14 de ISEL2020.
 *
 *
 *
 *
 *	Entradas:
 *		btn_led : boton para encender el led
 *		btn_alarma : boton para encender la alarma
 *		T_led : Temporizador del led
 *		T_alarma : Temporizador de la alarma
 *
 *  Variables:
 *    state_alarma : estado actual de la alarma
 *    state_led : estado actual de la alarma
 *    d1,d2,d3 : digitos de la alarma
 *
 *	Salidas:
 *	 alarma
 *	 led
**/

ltl led_on {
  [](btn_led -> <>(led && T_led);
}

ltl led_time {
  [](led -> (led U T_led));
}

ltl alarma1 {
  []((estado_alarma==) && btn_alarma->);
}

mtype = {LED_ON, LED_OFF, ALARMA_IDLE, ALARMA_ST1, ALARMA_ST2, ALARMA_ST3}

#define C1 1
#define C2 2
#define C3 3

int state_led;
int state_alarma;
int btn_led;
int time;
int led;
int T_led;
int btn_alarma;
int d1;
int d2;
int d3;
int alarma;
int T_alarma;




active proctype FSMled() {
	state_led = LED_ON;
	do
	:: (state_led == LED_OFF) -> atomic {
		if
		:: (btn_led)-> btn_led =0; state_led = LED_ON; led = 1; T_led= time + T;
		printf("led %d, timerLed %d\n", led, T_led);
		fi
	}
	:: (state_led == LED_ON) -> atomic {
		if
		:: (btn_led)-> btn_led = 0; T_led= time + T;
		 printf("led %d, timerLed %d\n", led, T_led);
		fi
		if
		:: (time >= T_led)->  led =0; state_led = LED_OFF;
		 printf("led %d, timerLed %d\n", led, T_led);
		fi
	}
	od
}

active proctype FSMalarma() {
	state_alarma = ALARMA_IDLE;
	do
	:: (state_alarma == ALARMA_IDLE) -> atomic {
		if
		:: (btn_alarma) -> btn_alarma = 0; d1++; T_alarma= time + T; state_alarma = ALARMA_ST1;
		fi

	}
	:: (state_alarma == ALARMA_ST1 ) -> atomic {
		if
		:: (btn_alarma) -> btn_alarma = 0; d1++;T_alarma= time + T;
		fi
		if
		:: (time >= T_alarma || d1 ==10)->  state_alarma = ALARMA_ST2;
		fi
	}
	:: (state_alarma == ALARMA_ST2) -> atomic {
		if
		:: (btn_alarma) -> btn_alarma = 0; d2++;T_alarma= time + T;
		fi
		if
		:: (time >= T_alarma || d2 == 10)->   state_alarma = ALARMA_ST3;
		fi
	}
	:: (state_alarma == ALARMA_ST3) -> atomic {
		if
		:: (btn_alarma) -> btn_alarma = 0; d3++;T_alarma= time + T;
		fi
		if
		:: ((time >= T_alarma || d3 == 10)&&((d1!=C1)||(d2!=C2)||(d3!=C3)))-> state_alarma = ALARMA_IDLE;
		fi
    if
    :: ((time >= T_alarma || d3 == 10)&&((d1==C1)&& (d2==C2)&&(d3==C3)) &&(alarma==1))-> state_alarma = ALARMA_IDLE; alarma=0;
    fi
    if
    :: ((time >= T_alarma || d3 == 10)&&((d1==C1)&& (d2==C2)&&(d3==C3)) &&(alarma==0))-> state_alarma = ALARMA_IDLE; alarma=1;
    fi
	}
	od
}
